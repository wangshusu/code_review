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

	//创建一个普通套接字
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("创建套接字失败");
		return -1;
	}

	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	//与服务器进行连接
	int ret = connect(sockfd,(struct sockaddr *)&server,sizeof(server));
	if(ret == -1)
	{
		perror("连接服务器失败");
		return -1;
	}

	struct timeval timeout = {TIMEOUT,0};
	unsigned char Cmd[256];
	uint8_t buf[__PAGE_MAX_LEN] = {0};			// 用来存储接收到tcp头文件
	uint8_t buf1[__DATA_SIZE] = {0};			// 数据包(发送用)
	uint8_t file_name[__FILE_NAME_LEN] = {0};	// 用来存储接收到的文件名
	MyProtMsg *ptr;
	//2.设置等待时间
	//setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));//MSG_WAITALL

	//与服务器进行数据的收发
	MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));	//发送文件的tcp包
	uint8_t cmd;

	while(1)
	{
		printf("ftp->:");
		memset(Cmd, 0, sizeof(Cmd));
		fgets(Cmd,sizeof( Cmd), stdin);	/* 从键盘获取输入 */
		Cmd[strlen(Cmd) - 1]=0;			/* 确保最后一个是空的 */

		/* 接下来进入判断 */
		if (strncmp(Cmd, "bye", strlen("bye")) == 0)/* 请求拜拜 */
		{
			client_Head_Init(msg);			 // 客户端 bye 初始化
			msg->pag_head.cmdno = __CMD_BYE;

			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);

			printf("1\n");
			break;
		}
		else if(strncmp(Cmd, "get", strlen("get")) == 0)/* 请求获取文件 */
		{
			printf("get\n");
			char *p = Cmd;
			p += strlen("get") + 1;
			
			memset(buf, 0, sizeof(buf));			 // 存储数据清空
			memset(file_name, 0, sizeof(file_name)); // 文件名清空
			
			memcpy(file_name, p, strlen(p));		 // 将文件名拷贝到 file_name
			printf("file_name = %s\n",file_name);

			client_Head_Init(msg);
			msg->pag_head.cmdno = __CMD_GET;
			msg->pag_head.id = __ID_CLIENT;
			msg->file_haed.file_end = __FILE_ISEND;
			msg->file_haed.file_num = 1;

			uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型
			add_FileNameToData(file_name, &data_p);
		
			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
			
			ret = recv(sockfd, buf, sizeof(buf), 0); 		// 接收回信
			if (analysis_FromServer(buf) != __CMD_ERROR)	//如果具备可读性
			{
				ptr = (MyProtMsg *)buf;
				if (ptr->pag_head.result == __RESULT_FAIL)  // 没有这个文件
				{
					printf("meiyouwenjian\n");
					printf("%s\n",data_p);
				}
				else if (ptr->pag_head.result == __RESULT_SUCCESS)	// 如果有文件
				{
					char buf_name[128] = {0};
					printf("请输入保存的地址和期望的文件名:");
					scanf("%s",buf_name);
					int fd = open(buf_name, O_RDWR | O_CREAT | O_TRUNC); // 可读可写 没有创建 有清除
					if (fd == -1)	// 打开失败
					{
						perror("Open Filed!");
						break;
					}
					else
					{
						while(1)
						{
							/* 1.将读到的数据写入 */
							memset(buf1, 0, sizeof(buf1));
							memcpy(buf1, ptr->file_haed.file_date, ptr->file_haed.file_size);
							ret = write(fd, buf1, ptr->file_haed.file_size);
							printf("写入了 %d 个字节\n",ret);

							ptr = (MyProtMsg *)buf;
							if (ptr->file_haed.file_end == __FILE_ISEND)	//结束了就退出
							{
								break;
							}

							/* 2.初始化返回数据包,告知发送端数据无误,接收成功 */
							client_Head_Init(msg);
							msg->file_haed.file_recv= __FILE_ISRECV;
							ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);

							/* 3.继续接收来自发送端发来的数据包,并对其进行判断 */
							memset(buf, 0, sizeof(buf)); // 存储数据清空
							ret = recv(sockfd, buf, sizeof(buf), 0); // 接收回信
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
								ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
								printf("数据包错误!\n");
								break;
							}
						}
					}
					close(fd);
				}
			}
			printf("2\n");
		}	
		else if(strncmp(Cmd, "put", strlen("put")) == 0)/* 请求上传文件 */
		{
			/* 1.获取要上传的文件名,发送个服务端 */
			printf("3\n");

			char *p = Cmd;
			p += strlen("get") + 1;
			
			memset(buf, 0, sizeof(buf));			 // 存储数据清空
			memset(file_name, 0, sizeof(file_name)); // 文件名清空
			
			memcpy(file_name, p, strlen(p));		 // 将文件名拷贝到 file_name
			printf("%s\n",file_name);

			client_Head_Init(msg);
			msg->pag_head.cmdno = __CMD_PUT;
			msg->pag_head.id = __ID_CLIENT;
			msg->file_haed.file_end = __FILE_ISEND;
			msg->file_haed.file_num = 2;
			uint8_t *data_p = (uint8_t *)&msg->file_haed.file_date;//强转结构体类型

			printf("输入要保存在客户端的路劲加文件名:");
			char buf_name[128] = {0};
			scanf("%s",buf_name);

			add_FileNameToData(file_name, &data_p);
			add_FileNameToData(buf_name, &data_p);
			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
printf("1\n");


			/* 2.接收来自服务端的回信,打开文件,进行发送 */
			ret = recv(sockfd,buf,sizeof(buf),0);
			if (analysis_FromServer(buf) == __CMD_ERROR)	//如果不具备可读性
			{
				break;
			}
			int fd = open(file_name,O_RDONLY);
			if (fd == -1) // 如果文件不存在
			{
				printf("客户端要上传的文件不存在!\n");
				// msg->pag_head.cmdno = __CMD_ERROR;
				// memcpy(data_p, "哈哈,我骗你的", sizeof("哈哈,我骗你的"));
				// ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
				break;
			}
			msg->pag_head.result = __RESULT_SUCCESS; // 服务端成功
printf("4\n");

			/* 3.开始读取文件内容,进行发送 */
			while ( (ret = read(fd, buf1, __DATA_SIZE)) > 0 )
			{
				client_Head_Init(msg);
				msg->pag_head.cmdno = __CMD_PUT;
				memset(data_p, 0, __DATA_SIZE);
				msg->file_haed.file_size = ret;
				
				printf("ret = %d\n",ret);
				if (ret == __DATA_SIZE)	// 估计后面还有数据
				{	
					msg->pag_head.file = __FILE_HAVE;
					msg->file_haed.file_end = __FILE_NOEND;

					memcpy(data_p, buf1, ret);
					
					ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
printf("5\n");
					/* 接收返回的数据包 */
					memset(buf, 0, sizeof(buf));
					ret = recv(sockfd,buf,sizeof(buf),0);
printf("8\n");					
					if (analysis_FromServer(buf) != __CMD_ERROR)	//如果具备可读性
					{
						ptr = (MyProtMsg *)buf;
						//head_Print(ptr);
						if (ptr->file_haed.file_recv != __FILE_ISRECV)
						{
							//printf("%#x\n",ptr->file_haed.file_recv);
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
					printf("%s\n",data_p);
					ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0); // 发送包
					printf("读完了\n");
					break;
				}
				memset(buf1, 0, sizeof(buf1));
			}
			close(fd);

		}
		else if(strncmp(Cmd, "ls", strlen("ls")) == 0)/* 请求列出文件列表 */
		{
			client_Head_Init(msg);
			msg->pag_head.cmdno = __CMD_LS;
			msg->pag_head.id = __ID_CLIENT;
			msg->file_haed.file_end = __FILE_ISEND;

			ret = send(sockfd, (void *)msg,sizeof(MyProtMsg), 0);
			
			printf("4\n");

			//1.等待接收 Serve 发来的数据
			//ret = recv(sockfd,buf,sizeof(buf),MSG_WAITALL); //没有数据 -1 没有数据就跟服务端断开连接
			ret = recv(sockfd,buf,sizeof(buf), 0); //没有数据 -1 没有数据就跟服务端断开连接
			cmd = analysis_FromServer(buf);
			if (cmd == __FILE_EMPTY)//没有文件
			{

			}
			else if (cmd == __FILE_HAVE)//有文件要进行传输
			{

			}
			else if (cmd == __FILE_LS)// 包里有文件名
			{

				//解析包名
				MyProtMsg *ptr = (MyProtMsg *)buf;
				uint32_t file_num = ptr->file_haed.file_num;
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.file_date, file_num);
				printf("\n");
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


			
			

			//4.接收到文件 解析文件 列出文件名

		}
		else//如果都不是
		{
			printf("stdin error!\n");
			printf("Useage:ls         展示当下文件\n");
			printf("       get 1.txt  下载1.txt\n");
			printf("       put 1.txt  上传1.txt\n");
			printf("       bye        退出客户端\n");
			printf("   \n");
		}
		
	}
	close(sockfd);
}


