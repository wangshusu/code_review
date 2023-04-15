#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h> 
#include <fcntl.h>

#include "threadpool.h"
#include "ftp_agree.h"
#include "getpath.h"

/********** 全局变量区 ********/
struct sockaddr_in client;
socklen_t len = sizeof(client);
#define __FTP_FILE_PATH  "./"

int confd;
/*
	Create_ListenSock:创建一个监听套接字
		@server_ip : 服务器端的IP地址字符串
		@server_port:服务器端的端口号
		@return	: 成功返回创建好的监听套接字,失败返回-1
*/
int Create_ListenSock(char *server_ip,unsigned short server_port)
{
	//1.创建流式套接字
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1)
	{
		perror("创建普通套接字失败");
		return -1;
	}

	//2.为服务器端绑定IP地址
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(server_port);
	saddr.sin_addr.s_addr = inet_addr(server_ip);

	int ret = bind(fd,(struct sockaddr *)&saddr,sizeof(saddr));
	if(ret == -1)
	{
		perror("绑定IP地址和端口号失败");
		return -1;
	}
	else
	{
		printf("绑定IP地址和端口号成功!\n");
	}

	//3.监听套接字
	ret = listen(fd,10);
	if(ret == -1)
	{
		perror("监听失败");
		return -1;
	}
	else
	{
		printf("创建监听套接字成功!\n");
	}

	return fd;
}

/*
	Handle_TcpClient:负责与客户端的通信
		@confd : 连接套接字
		
*/
void func(void *arg)
{
	printf("开始任务\n");

	struct sockaddr_in client1;	// 接收来自主线程的客户信息
	int confd1 = confd;
	client1 = client;
	socklen_t len1 = sizeof(client1);

	uint8_t buf[__PAGE_MAX_LEN] = {0};			// 用来存储接收到tcp包
	uint8_t buf1[__DATA_SIZE] = {0};			// 数据包(发送用)
	uint8_t file_name[__FILE_NAME_LEN] = {0};	// 用来存储接收到的文件名
	MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));	//发送文件的tcp包
	int ret ;

	printf("当前线程进入任务\n");
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		ret = recv(confd1, buf, sizeof(buf),0);
		if(ret > 0)
		{
			printf("Recv From [IP : %s] [PORT : %d]\n",inet_ntoa(client1.sin_addr),ntohs(client1.sin_port));

			/******* 解包并判断 ******/
			uint8_t cmd = analysis_FromClient(buf);

			if (cmd == __CMD_LS)/* 如果是要文件列表 */
			{
				printf("cmd = ls\n");
				Queue *qu_filename = NULL;
				Queue *qu_pathname = get_dir_path(__FTP_FILE_PATH, &qu_filename);//获取路径 文件名
				uint8_t *file_name_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型
				server_Head_Init(msg);

				/************** 将文件名拷贝到data里面 并且发送 ***********/
				Node * ptr = qu_filename ->front;
				while(ptr)
				{
					char *buf = (char *) ptr ->data;
					add_FileNameToData(buf, &file_name_p);
					ptr = ptr ->next;
				}

				msg->pag_head.file = __FILE_LS;
				msg->pag_head.result = __RESULT_SUCCESS;
				msg->file_haed.file_end = __FILE_ISEND;
				msg->file_haed.file_num = qu_filename->num;

				//head_Print(msg);
				ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0); // 发送包

				/*********** 记得要销毁 *********/
				queue_Destroy(&qu_filename,NULL);
				queue_Destroy(&qu_pathname,NULL);

			}
			else if (cmd == __CMD_GET)/* 如果是要获取文件 */
			{
				printf("cmd = get\n");
				memset(file_name, 0, sizeof(file_name));
				memset(buf1, 0, sizeof(buf1));

				MyProtMsg *ptr = (MyProtMsg *)buf;
				uint32_t file_num = ptr->file_haed.file_num;
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				for (int i = 0; i < file_num; i++)	// 显示一下要获取的文件
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
				printf("\n");
				
				server_Head_Init(msg);	// 服务器包的初始化
				uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型

				int fd = open(p[0],O_RDONLY);
				if (fd == -1) // 如果文件不存在
				{
					printf("客户端所需文件不存在!\n");
					msg->pag_head.result = __RESULT_FAIL; // 服务端失败
					memcpy(data_p, "没有文件了", sizeof("没有文件了"));
					ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
				}
				else
				{
					msg->pag_head.result = __RESULT_SUCCESS; // 服务端成功

					while ( (ret = read(fd, buf1, __DATA_SIZE)) > 0 )
					{
						memset(data_p, 0, __DATA_SIZE);
						msg->file_haed.file_size = ret;

						printf("ret = %d\n",ret);
						if (ret == __DATA_SIZE)	// 估计后面还有数据
						{	
							msg->pag_head.file = __FILE_HAVE;
							msg->file_haed.file_end = __FILE_NOEND;
	
							memcpy(data_p, buf1, ret);
							ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
							
							/*  */
							memset(buf, 0, sizeof(buf));
							ret = recv(confd1,buf,sizeof(buf),0);
							if (analysis_FromClient(buf) != __CMD_ERROR)	//如果具备可读性
							{
								ptr = (MyProtMsg *)buf;
								if (ptr->file_haed.file_recv != __FILE_ISRECV)
								{
									printf("传输出错!\n");
									break;
								}
							}
							else
							{
								break;
							}
							//printf("还没有读完\n");
						}
						else	// 估计读完了
						{
							msg->pag_head.file = __FILE_HAVE;
							msg->file_haed.file_end = __FILE_ISEND;

							memcpy(data_p, buf1, ret);
							ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
							printf("读完了\n");
							break;
						}
						memset(buf1, 0, sizeof(buf1));
					}
				}
				close(fd);

				/************* 开辟的堆区空间释放 **********/
				free_FileNameData(p, file_num);

				
			}
			else if (cmd == __CMD_PUT)/* 如果是要上传文件 */
			{
				memset(file_name, 0, sizeof(file_name));
				memset(buf1, 0, sizeof(buf1));

				/* 1.获取要上传的文件名,并且创建这个文件 */
				MyProtMsg *ptr = (MyProtMsg *)buf;
				uint32_t file_num = ptr->file_haed.file_num;
				//↓↓↓↓↓↓下面出错必须返回
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				for (int i = 0; i < file_num; i++) // 显示一下要上传的文件
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
printf("2\n");
				

				int fd = open(p[1], O_RDWR | O_CREAT | O_TRUNC); // 可读可写 没有创建 有清除
				if (fd == -1)	// 打开失败
				{
					perror("Open Filed!");
					break;
				}

				/* 2.发送客户端,告知保存的文件名是合法,接着进行传输 */
				server_Head_Init(msg);
				msg->pag_head.cmdno = __CMD_PUT;
				msg->file_haed.file_recv= __FILE_ISRECV;
				ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0);
printf("3\n");


				/* 3.将读取到的数据包进行写入 */
				memset(buf, 0, sizeof(buf));
				ret = recv(confd1, buf, sizeof(buf), 0); 		// 接收回信
printf("6\n");
				if (analysis_FromClient(buf) != __CMD_ERROR) 	//如果具备可读性
				{
					while(1)
					{
						/* 3.1 将读到的数据写入 */
						memset(buf1, 0, sizeof(buf1));
						memcpy(buf1, ptr->file_haed.file_date, ptr->file_haed.file_size);
						ret = write(fd, buf1, ptr->file_haed.file_size);
						printf("写入了 %d 个字节\n",ret);

						ptr = (MyProtMsg *)buf;
						if (ptr->file_haed.file_end == __FILE_ISEND)	//结束了就退出
						{
							break;
						}

						/* 3.2 初始化返回数据包,告知发送端数据无误,接收成功 */
						server_Head_Init(msg);
						msg->file_haed.file_recv= __FILE_ISRECV;
						//printf("%#x\n",ptr->file_haed.file_recv);
						ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0);
printf("7\n");
						/* 3.继续接收来自发送端发来的数据包,并对其进行判断 */
						memset(buf, 0, sizeof(buf)); // 存储数据清空
						ret = recv(confd1, buf, sizeof(buf), 0); // 接收回信
						ptr = (MyProtMsg *)buf;
						if (analysis_FromServer(buf) != __CMD_ERROR) 	//如果具备可读性
						{
							if (ptr->file_haed.file_end == __FILE_NOEND)	//还没有结束
							{
								continue;
							}
							printf("要退出了\n");
							memset(buf1, 0, sizeof(buf1));
							memcpy(buf1, ptr->file_haed.file_date, ptr->file_haed.file_size);
							ret = write(fd, buf1, ptr->file_haed.file_size);
							printf("写入了 %d 个字节\n",ret);
							break;
						}
						else //如果不具备可读性 告诉服务端 出错了
						{
							msg->file_haed.file_recv= __FILE_NORECV;
							ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0);
							printf("数据包错误!\n");
							break;
						}
					}
					close(fd);
				}
				else
				{
					printf("客户端上传文件出错!\n");
					close(fd);
				}

			
				printf("put\n");
			}
			else if (cmd == __CMD_BYE)/* 如果是要拜拜 */
			{
				printf("haha byebye\n");
				/* 断开连接准备退出 */
				break;
			}
			else if (cmd == __CMD_ERROR)/* 如果解析失败 */
			{
				printf("cant analysis this package!\n");
				break;
			}
			//unsigned char resp[256] = {"我已经接收到你的数据啦!\n"};
			//send(confd1,resp,strlen(resp),0);	//回应客户端
		}
	}

	close(confd1);//关闭连接
}

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("Usage : %s <SERVER_IP> <SERVER_PROT>!\n",argv[0]);
		return -1;
	}

	//创建一个监听套接字
	int sockfd = Create_ListenSock(argv[1],atoi(argv[2]));
	if(sockfd == -1)
	{
		return -1;
	}

	/* 创建线程池并初始化 */
    Threadpool* pool = creat_Pthread( 20 ,10, 4);//最大任务数20 最大响应数10 最低在线数2


	while(1)
	{
		//陷入阻塞等待客户端与我进行连接
		//每来一个客户端则创建一个子进程负责与客户端进行通信
		//而父进程则重新阻塞等待新的客户端进行连接
		confd = accept(sockfd,(struct sockaddr *)&client,&len);
		if(confd == -1)
		{
			perror("连接失败");
			break;
		}
		
		//运行到这里就是有任务了  来活了  给线程池添加任务
		printf("线程池添加任务");
		thread_Add(pool, func, NULL);

	}

	close(sockfd);//关闭监听
	thread_Destroy(pool);//销毁线程池
}

