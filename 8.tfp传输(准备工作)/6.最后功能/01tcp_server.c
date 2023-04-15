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

typedef struct client_msg
{
	int cnd; // 存储已连接的套接字
	struct sockaddr_in pthread_addr; // 存储客户端的信息
}Client_Msg;


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

	// 设置端口复用
	int opt = 1;
	int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//2.为服务器端绑定IP地址
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(server_port);
	saddr.sin_addr.s_addr = inet_addr(server_ip);

	ret = bind(fd,(struct sockaddr *)&saddr,sizeof(saddr));
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
	Client_Msg *client_p = (Client_Msg *)arg;	// 接收来自主线程的客户信息
	struct sockaddr_in client_addr = client_p->pthread_addr;
	int confd = client_p->cnd;
	
	uint8_t buf_tcp[__PAGE_MAX_LEN] = {0};			// 用来存储接收到tcp包
	uint8_t buf_data[__DATA_SIZE] = {0};			// 数据包(发送用)
	uint8_t file_name[__FILE_NAME_LEN] = {0};	    // 用来存储接收到的文件名
	MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));	//发送文件的tcp包
	int ret; // 接收各种函数的返回值

	printf("当前线程进入任务\n");
	while(1)
	{
		memset(buf_tcp, 0, sizeof(buf_tcp));
		ret = recv(confd, buf_tcp, sizeof(buf_tcp),0);
		if (ret <= 0) // 如果客户端意外退出
		{
			break;
		}
		else
		{
			printf("\nRecv From [IP : %s] [PORT : %d]\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		/* 1.解包并判断 */
			uint8_t cmd = analysis_FromClient(buf_tcp);
			if (cmd == __CMD_LS) /* 如果是要文件列表 */
			{
printf("cmd = ls\n\n");

			/* 1.获取文件名,存入到队列中 */
				Queue *qu_filename = NULL;
				Queue *qu_pathname = get_dir_path(__FTP_FILE_PATH, &qu_filename); // 获取路径 文件名
				uint8_t *file_name_p = (uint8_t *)&msg->file_haed.file_date; // 强转结构体类型
//getpath_Print(qu_filename);


			/* 2.将文件名拷贝到data里面 并且发送 */
				server_Head_Init(msg);
				Node * ptr = qu_filename ->front;
				while(ptr)
				{
					//char *buf = (char *) ptr ->data;
					add_FileNameToData((char *)(ptr ->data), &file_name_p);
					ptr = ptr ->next;
				}
				
				msg->pag_head.file = __FILE_LS;
				msg->pag_head.result = __RESULT_SUCCESS;
				msg->file_haed.file_end = __FILE_ISEND;
				msg->file_haed.file_num = qu_filename->num;
				ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
				if (-1 == ret) // 如果发送失败
				{
					perror("ls send error");
					queue_Destroy(&qu_filename,NULL);
					queue_Destroy(&qu_pathname,NULL);
					break;
				}

			/* 3.销毁创建的两条链表 */
				queue_Destroy(&qu_filename,NULL);
				queue_Destroy(&qu_pathname,NULL);

			}
			else if (cmd == __CMD_GET) /* 如果是要获取文件 */
			{
				printf("cmd = get\n\n");
				server_Head_Init(msg);	// 服务器包的初始化
				memset(buf_data, 0, sizeof(buf_data)); // 
				memset(file_name, 0, sizeof(file_name)); //
				
				
			/* 1.解析一下是要获取什么文件 */
				MyProtMsg *ptr = (MyProtMsg *)buf_tcp;
				uint32_t file_num = ptr->file_haed.file_num;
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				for (int i = 0; i < file_num; i++)	// 显示一下要获取的文件
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
				printf("\n");
				
				
				uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型

				/* 如果想要获取的文件不存在 */
				int fd = open(p[0],O_RDONLY);
				if (fd == -1) 
				{
					printf("客户端所需文件不存在!\n");
					msg->pag_head.result = __RESULT_FAIL; // 服务端失败
					//memcpy(data_p, "没有文件了", sizeof("没有文件了"));
					ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
				}
				else
				{
					int send_num = 0;	// 发送次数
					msg->pag_head.result = __RESULT_SUCCESS; // 服务端成功
					while ( (ret = read(fd, buf_data, __DATA_SIZE)) > 0 )
					{
						send_num ++;
						memset(data_p, 0, __DATA_SIZE);
						msg->file_haed.file_size = ret;

//printf("ret = %d\n",ret);
						if (ret == __DATA_SIZE)	// 估计后面还有数据
						{	
							msg->pag_head.file = __FILE_HAVE;
							msg->file_haed.file_end = __FILE_NOEND;

							/* 将读出的数据发送到客户端 */
							memcpy(data_p, buf_data, ret);
printf("第 %d 次发送,发送了 %d 个字节\n", send_num, ret);
							ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
							if (-1 == ret)
							{
								break;
							}
							
							/* 阻塞接收来自客户端的数据 */
							memset(buf_tcp, 0, sizeof(buf_tcp));
							ret = recv(confd,buf_tcp,sizeof(buf_tcp),0);
							if (ret <= 0) // 客户端意外退出
							{
								break;
							}
							if (analysis_FromClient(buf_tcp) != __CMD_ERROR) //如果具备可读性
							{
								ptr = (MyProtMsg *)buf_tcp;
								if (ptr->file_haed.file_recv != __FILE_ISRECV)
								{
									printf("传输出错!\n");
									break;
								}
							}
							else // 可读性都不具备 直接退出
							{
								break;
							}
							//printf("还没有读完\n");
						}
						else // 运行到这里 文件就快读完了 剩余字节数 < __DATA_SIZE
						{
							msg->pag_head.file = __FILE_HAVE;
							msg->file_haed.file_end = __FILE_ISEND;

							memcpy(data_p, buf_data, ret);
							ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0); // 最后一个次发送
							printf("读完了\n");
							break;
						}
						memset(buf_data, 0, sizeof(buf_data));
					}
					close(fd);
				}
				
				/************* 开辟的堆区空间释放 **********/
				free_FileNameData(p, file_num);
				
			}
			else if (cmd == __CMD_PUT)/* 如果是要上传文件 */
			{
				memset(file_name, 0, sizeof(file_name));
				memset(buf_data , 0, sizeof(buf_data));

			/* 1.获取要上传的文件名,并且创建这个文件 */
				MyProtMsg *ptr = (MyProtMsg *)buf_tcp;
				uint32_t file_num = ptr->file_haed.file_num;
				//↓↓↓↓↓↓下面出错必须返回
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				if(p == NULL)
				{
					printf("analysis_FileNameData error\n");
					break;
				}
				for (int i = 0; i < file_num; i++) // 显示一下要上传的文件
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
				
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
				ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0);
				if (-1 == ret)
				{
					close(fd);
					break;
				}

			/* 3.接收客户端发送的数据包 */
				memset(buf_tcp, 0, sizeof(buf_tcp));
				ret = recv(confd, buf_tcp, sizeof(buf_tcp), 0);// 接收回信
				if (ret <= 0)
				{
					close(fd);
					break;
				}

			/* 4. */
				if (analysis_FromClient(buf_tcp) != __CMD_ERROR) // 如果具备可读性 
				{
					while(1) // 这个while循环 里面的break 后都具备关闭 fd 文件描述符的能力
					{
						
					/* 1.将读到的数据写入 */
						ptr = (MyProtMsg *)buf_tcp;
						memset(buf_data, 0, sizeof(buf_data));
						memcpy(buf_data, ptr->file_haed.file_date, ptr->file_haed.file_size);

						ret = write(fd, buf_data, ptr->file_haed.file_size);
						printf("写入了 %d 个字节\n",ret);

						ptr = (MyProtMsg *)buf_tcp;
						if (ptr->file_haed.file_end == __FILE_ISEND) // 结束了就退出
						{
							break;
						}

					/* 2.初始化返回数据包,告知发送端数据无误,接收成功 */
						server_Head_Init(msg);
						msg->file_haed.file_recv= __FILE_ISRECV;
						ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0);
						if (-1 == ret)
						{
							break;
						}

					/* 3.继续接收来自发送端发来的数据包,并对其进行判断 */
						memset(buf_tcp, 0, sizeof(buf_tcp)); // 存储数据清空
						ret = recv(confd, buf_tcp, sizeof(buf_tcp), 0); // 接收回信
						if (ret <= 0)
						{
							break;
						}
						ptr = (MyProtMsg *)buf_tcp;
						if (analysis_FromClient(buf_tcp) != __CMD_ERROR) 	//如果具备可读性
						{
							if (ptr->file_haed.file_end == __FILE_NOEND)	//还没有结束
							{
								continue;
							}
							printf("要退出了\n");
							memset(buf_data, 0, sizeof(buf_data));
							memcpy(buf_data, ptr->file_haed.file_date, ptr->file_haed.file_size);
							ret = write(fd, buf_data, ptr->file_haed.file_size);
							printf("写入了 %d 个字节\n",ret);
							break;
						}
						else //如果不具备可读性 告诉服务端 出错了
						{
							msg->file_haed.file_recv= __FILE_NORECV;
							ret = send(confd, (void *)msg,sizeof(MyProtMsg), 0);
							if (-1 == ret)
							{
								break;
							}
							printf("数据包错误!\n");
							break;
						}
						}
					close(fd);
				}
				else
				{
					close(fd);
					printf("客户端上传文件出错!\n");
				}
			}
			else if (cmd == __CMD_BYE)/* 如果是要拜拜 */
			{
				/* 断开连接准备退出 */
				printf("haha byebye\n\n");
				
				break;
			}
			else if (cmd == __CMD_ERROR)/* 如果解析失败 */
			{
				printf("cant analysis this package!\n");
				break;
			}

		}
	}

	if (msg != NULL)
	{
		free(msg);
	}
	close(confd);//关闭连接
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
    Threadpool* pool = creat_Pthread( 1, 2, 1);//最大任务数20 最大响应数10 最低在线数2

	
	while(1)
	{
		//陷入阻塞等待客户端与我进行连接
		//每来一个客户端则创建一个子进程负责与客户端进行通信
		//而父进程则重新阻塞等待新的客户端进行连接

		struct sockaddr_in client_addr; // 接收客户端的信息
		confd = accept(sockfd,(struct sockaddr *)&client_addr,&len);
		if(confd == -1)
		{
			perror("连接失败");
			break;
		}
		
		//运行到这里就是有任务了  来活了  给线程池添加任务
		Client_Msg *clien_msg = (Client_Msg *)calloc(1, sizeof(Client_Msg));
		clien_msg->cnd = confd;
		clien_msg->pthread_addr = client_addr;
		
		thread_Add(pool, func, (void *)clien_msg);
		printf("线程池添加任务");
	}

	close(sockfd);//关闭监听
	thread_Destroy(pool);//销毁线程池
}

