#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
	unsigned char buf[__PAGE_MAX_LEN] = {0};

	//2.设置等待时间
	//setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));//MSG_WAITALL

	//与服务器进行数据的收发
	MyProtMsg *meg;
	uint8_t cmd;
	while(1)
	{
		printf("ftp->:");
		fgets(Cmd,sizeof( Cmd), stdin);	/* 从键盘获取输入 */
		Cmd[strlen(Cmd) - 1]=0;			/* 确保最后一个是空的 */

		/* 接下来进入判断 */
		if (strncmp(Cmd, "bye", strlen("bye")) == 0)/* 请求拜拜 */
		{
			meg = client_Head_Init(__CMD_BYE);
			ret = send(sockfd, (void *)meg,sizeof(MyProtMsg), 0);
			free(meg);
			printf("1\n");
			break;
		}
		else if(strncmp(Cmd, "get", strlen("get")) == 0)/* 请求获取文件 */
		{
			printf("get\n");
			char *p = Cmd;
			p += strlen("get") + 1;
			
			char file_name[32] = {0};
			memcpy(file_name, p, strlen(p));
			printf("%s\n",file_name);

			meg = client_Head_Init(__CMD_GET);
			uint8_t *file_name_p = (uint8_t *)&meg->file_haed.data;//强转结构体类型
			meg->file_haed.remain_pagnum = 1;
			add_FileNameToData(file_name, &file_name_p);

			ret = send(sockfd, (void *)meg,sizeof(MyProtMsg), 0);
			free(meg);

			memset(buf, 0, sizeof(buf));
			ret = recv(sockfd,buf,sizeof(buf), 0); //没有数据 -1 没有数据就跟服务端断开连接
			MyProtMsg *ptr = (MyProtMsg *)buf;
			if (ptr->pag_head.result == 0)//没有这个文件
			{
				printf("%s\n",(char *)ptr->file_haed.data);
			}

			printf("2\n");
		}	
		else if(strncmp(Cmd, "put", strlen("put")) == 0)/* 请求上传文件 */
		{
			printf("3\n");
		}
		else if(strncmp(Cmd, "ls", strlen("ls")) == 0)/* 请求列出文件列表 */
		{
			meg = client_Head_Init(__CMD_LS);
			ret = send(sockfd, (void *)meg,sizeof(MyProtMsg), 0);
			
			free(meg);
			printf("4\n");

			//1.等待接收 Serve 发来的数据
			//ret = recv(sockfd,buf,sizeof(buf),MSG_WAITALL); //没有数据 -1 没有数据就跟服务端断开连接
			ret = recv(sockfd,buf,sizeof(buf), 0); //没有数据 -1 没有数据就跟服务端断开连接
			if (ret == -1)
			{
				//3.超过20s 客户端发出错误信息 向服务器发送bye
				meg = client_Head_Init(__CMD_BYE);
				ret = send(sockfd, (void *)meg,sizeof(MyProtMsg), 0);
				printf("超时了\n");
				printf("没有接收到数据!\n");
				free(meg);
				break;
			}

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
				uint32_t remain_pagnum = ptr->file_haed.remain_pagnum;
				uint8_t **p = analysis_FileNameData((uint8_t *)&ptr->file_haed.data, remain_pagnum);
				printf("\n");
				for (int i = 0; i < remain_pagnum; i++)
				{
					printf("%s\n",(uint8_t *)p[i]);
				}
				printf("\n");
				
				for (int i = 0; i < remain_pagnum; i++)
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


