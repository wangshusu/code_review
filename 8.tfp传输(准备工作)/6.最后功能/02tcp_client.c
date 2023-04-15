#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>

#include "ftp_agree.h"

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("Usage : %s <SERVER_IP> <SERVER_PROT>!\n",argv[0]);
		return -1;
	}

	int ret; // ret 用来接收各种函数的返回值

/* 1.创建一个普通套接字 */
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("创建套接字失败");
		return -1;
	}


/* 2.与服务器进行连接 */
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(sockfd,(struct sockaddr *)&server,sizeof(server));
	if(ret != 0)
	{
		perror("连接服务器失败");
		close(sockfd);
		return -1;
	}

	//初始化
	uint8_t cmd;								// 指令	
	MyProtMsg *ptr;								// 指向强转后的tcp数据包
	uint8_t Cmd[__CMD_MAX_LEN] = {0};			// 接收指令
	uint8_t buf_date[__DATA_SIZE] = {0};		// 数据包(发送用)
	uint8_t buf_tcp[__PAGE_MAX_LEN] = {0};		// 用来存储接收到tcp头文件
	uint8_t file_name[__FILE_NAME_LEN] = {0};	// 用来存储接收到的文件名
	
	//2.设置等待时间
	//struct timeval timeout = {TIMEOUT,0};
	//setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));//MSG_WAITALL

/* 3.与服务器进行数据的收发 */
	MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));	//发送文件的tcp包
	while(1)
	{
		printf("ftp->:");
		client_Head_Init(msg);			 // 客户端 bye 初始化
		memset(Cmd, 0, sizeof(Cmd));	 // 清空 Cmd 里面的内容
		fgets(Cmd,sizeof( Cmd), stdin);	 /* 从键盘获取输入 */
		Cmd[strlen(Cmd) - 1]=0;			 /* 确保最后一个是空的 */

	/* 1.进入判断 */
		if (strncmp(Cmd, "bye", strlen("bye")) == 0) /* 请求拜拜 */
		{
			msg->pag_head.cmdno = __CMD_BYE;
			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
			if (ret == -1)
			{
				printf("__CMD_BYE is failed!\n");
			}
			break;
		}
		else if (strncmp(Cmd, "get", strlen("get")) == 0) /* 请求获取文件 */
		{
			printf("get\n");

			char *p = Cmd;
			p += strlen("get");
			while (*p == ' ')	// 将指针定位到下一个参数
			{
				p++;
			}
			
		/* 1.将获取到的文件名发送给服务端 */
			memset(buf_tcp, 0, sizeof(buf_tcp));	 // 存储数据清空
			memset(file_name, 0, sizeof(file_name)); // 文件名清空
			memcpy(file_name, p, strlen(p));		 // 将文件名拷贝到 file_name

			msg->pag_head.id = __ID_CLIENT;
			msg->pag_head.cmdno = __CMD_GET;
			msg->file_haed.file_num = __ONE_FILE;	 // 文件名的个数
			msg->file_haed.file_end = __FILE_ISEND;

			uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date; // 强转结构体类型 指向data
			add_FileNameToData(file_name, &data_p);
		
			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
			if (ret == -1)
			{
				printf("send failed!\n");
				break;
			}
		
		/* 2.接收服务端的回信,开始判断 */
			ret = recv(sockfd, buf_tcp, sizeof(buf_tcp), 0); // 接收回信
			if (ret <= 0)
			{
				printf("读取失败 || 客户端意外退出!\n");
				break;
			}
			if (analysis_FromServer(buf_tcp) != __CMD_ERROR) //如果具备可读性
			{
				ptr = (MyProtMsg *)buf_tcp;
				if (ptr->pag_head.result == __RESULT_FAIL) // 没有这个文件
				{
					printf("The server can't find this File!\n");
				}
				else if (ptr->pag_head.result == __RESULT_SUCCESS)	// 如果有文件
				{
					char buf_name[__FILE_LEN] = {0}; // 保存的地址和期望的文件名
					int fd = -1; // 文件描述符
					do
					{
						memset(buf_name, 0, sizeof(buf_name));
						printf("\n请输入保存的地址和期望的文件名:");
						scanf("%s",buf_name);
						fd = open(buf_name, O_RDWR | O_CREAT | O_TRUNC); // 可读可写 没有创建 有清除
						if (fd != -1)	// 打开成功 那么就开始写入数据
						{
							break;
						}
						perror("Open this file failed!");
					} while (1);
					
					while(1)
					{

					/* 1.将读到的数据写入 */
						memset(buf_date, 0, sizeof(buf_date));
						memcpy(buf_date, ptr->file_haed.file_date, ptr->file_haed.file_size);
						ret = write(fd, buf_date, ptr->file_haed.file_size);
						printf("写入了 %d 个字节\n",ret);

						ptr = (MyProtMsg *)buf_tcp;
						if (ptr->file_haed.file_end == __FILE_ISEND)	//如果第一次就写完了 那么就退出吧
						{
							printf("接收完毕!\n");
							break;
						}

					/* 2.初始化返回数据包,告知发送端数据无误,接收成功 */
						client_Head_Init(msg);
						msg->file_haed.file_recv= __FILE_ISRECV;
						ret = send(sockfd, (void *)msg, sizeof(MyProtMsg), 0);
						if (ret == -1)
						{
							printf("send failed!\n");
							break;
						}

					/* 3.继续接收来自发送端发来的数据包,并对其进行判断 */
						memset(buf_tcp, 0, sizeof(buf_tcp)); // 存储数据清空
						ret = recv(sockfd, buf_tcp, sizeof(buf_tcp), 0); // 接收回信
						if (ret <= 0)
						{
							printf("recv failed!\n");
							break;
						}

						ptr = (MyProtMsg *)buf_tcp;
						if (analysis_FromServer(buf_tcp) != __CMD_ERROR) //如果具备可读性
						{
							if (ptr->file_haed.file_end == __FILE_NOEND) //还没有结束
							{
								continue;
							}
							printf("要退出了\n");
							memset(buf_date, 0, sizeof(buf_date));
							memcpy(buf_date, ptr->file_haed.file_date, ptr->file_haed.file_size);
							ret = write(fd, buf_date, ptr->file_haed.file_size);
							printf("写入了 %d 个字节\n",ret);
							break;
						}
						else //如果不具备可读性 告诉服务端 出错了
						{
							msg->file_haed.file_recv= __FILE_NORECV;
							ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
							printf("数据包错误!\n");
							break;
						}
					}
					
					close(fd);
				}
			}
		}	
		else if(strncmp(Cmd, "put", strlen("put")) == 0) /* 请求上传文件 */
		{
		/* 1.获取要上传的文件名,发送个服务端 */
			printf("3\n");

			char *p = Cmd;
			p += strlen("get");
			while (*p == ' ')	// 将指针定位到下一个参数
			{
				p++;
			}

			memset(buf_tcp, 0, sizeof(buf_tcp));     // 存储数据清空
			memset(file_name, 0, sizeof(file_name)); // 文件名清空
			memcpy(file_name, p, strlen(p));		 // 将文件名拷贝到 file_name
			
			int fd = -1;
			do
			{
				
				fd = open(file_name,O_RDONLY);
				if (fd != -1) // 如果文件存在
				{
					break;
				}
				printf("文件不存在!\n");
				printf("请重新输入要上传的文件名:");
				memset(file_name, 0, sizeof(file_name)); // 文件名清空
				scanf("%s",file_name);
			} while (1);
			
			
			msg->pag_head.id = __ID_CLIENT;
			msg->pag_head.cmdno = __CMD_PUT;
			msg->file_haed.file_end = __FILE_ISEND;
			msg->file_haed.file_num = 2;
			uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型

			printf("输入要保存在客户端的路劲加文件名:");
			char buf_name[128] = {0};
			scanf("%s",buf_name);

			add_FileNameToData(file_name, &data_p);
			add_FileNameToData(buf_name, &data_p);
			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
			if (ret == -1)
			{
				printf("send failed!\n");
				break;
			}


		/* 2.接收来自服务端的回信进行发送 */
			ret = recv(sockfd,buf_tcp,sizeof(buf_tcp),0);
			if (ret <= 0)
			{
				perror("recv faild");
				break;
			}
			if (analysis_FromServer(buf_tcp) == __CMD_ERROR) //如果不具备可读性
			{
				//这里应该还要进行一下判断的 但是考虑到服务端只有接收成功才会发这个消息 
				//否则都是断开连接 所以这里的判断就省略了
				printf("can't understand what you doing!\n");
				break;
			}

			
			//msg->pag_head.result = __RESULT_SUCCESS;

			/* 3.开始读取文件内容,进行发送 */
			memset(buf_date, 0, sizeof(buf_date));
			data_p = (uint8_t *)&msg->file_haed.file_date;
			int send_num = 0;
			while ( (ret = read(fd, buf_date, __DATA_SIZE)) > 0 )
			{
				send_num++;
				memset(data_p, 0, __DATA_SIZE);
				msg->file_haed.file_size = ret;

				if (ret == __DATA_SIZE)	// 估计后面还有数据
				{
					msg->pag_head.file = __FILE_HAVE;
					msg->file_haed.file_end = __FILE_NOEND;

					memcpy(data_p, buf_date, ret);
printf("这是第 %d 次发送,发送了 %d 个字节\n",ret,send_num);
					ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包

					memset(buf_tcp, 0, sizeof(buf_tcp));
					ret = recv(sockfd,buf_tcp,sizeof(buf_tcp),0);
					if (analysis_FromServer(buf_tcp) != __CMD_ERROR) // 如果具备可读性
					{
						ptr = (MyProtMsg *)buf_tcp;
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
				}
				else	// 估计读完了
				{
					msg->pag_head.file = __FILE_HAVE;
					msg->file_haed.file_end = __FILE_ISEND;

					memcpy(data_p, buf_date, ret);
printf("这是第 %d 次发送,发送了 %d 个字节\n",ret,send_num);
					ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
					printf("读完了\n");
					break;
				}
				memset(buf_date, 0, sizeof(buf_date));
				
			}
			close(fd);

		}
		else if(strncmp(Cmd, "ls", strlen("ls")) == 0) /* 请求列出文件列表 */
		{
			//client_Head_Init(msg);
			msg->pag_head.cmdno = __CMD_LS;
			msg->pag_head.id = __ID_CLIENT;
			msg->file_haed.file_end = __FILE_ISEND;

			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);

		//1.等待接收 Serve 发来的数据
			//ret = recv(sockfd,buf_tcp,sizeof(buf_tcp),MSG_WAITALL); //没有数据 -1 没有数据就跟服务端断开连接
			ret = recv(sockfd,buf_tcp,sizeof(buf_tcp), 0); //没有数据 -1 没有数据就跟服务端断开连接
			cmd = analysis_FromServer(buf_tcp);
			if (cmd == __FILE_LS)// 包里有文件名
			{

			//1.接收到文件 解析文件 列出文件名
				MyProtMsg *ptr = (MyProtMsg *)buf_tcp;
				uint32_t file_num = ptr->file_haed.file_num;
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				
				printf("\n文件名如下:\n");
				for (int i = 0; i < file_num; i++)
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
				printf("\n");
				
				for (int i = 0; i < file_num; i++)
				{
					free(p[i]);
				}
				free(p);
				p = NULL;
			}	

		}
		else //如果都不是 打印输入功能列表
		{
			printf("stdin error!\n");
			printf("Useage:ls         展示当下文件\n");
			printf("       get 1.txt  下载1.txt\n");
			printf("       put 1.txt  上传1.txt\n");
			printf("       bye        退出客户端\n");
			printf("   \n");
		}
		
	}

	if (msg != NULL)
	{
		free(msg);
	}
	close(sockfd);
}


