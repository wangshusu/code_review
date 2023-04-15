#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


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

	unsigned char cmd[256] = {0};
	//与服务器进行数据的收发
	while(1)
	{
		printf("ftp->:");
		fgets(cmd,sizeof( cmd), stdin);	/* 从键盘获取输入 */
		cmd[strlen(cmd) - 1]=0;			/* 确保最后一个是空的 */

		/* 接下来进入判断 */
		if (strncmp(cmd, "bye", strlen("bye")) == 0)/* 如果要拜拜了 */
		{
			printf("1\n");
			break;
		}
		else if(strncmp(cmd, "get", strlen("get")) == 0)/* 如果要获取文件 */
		{
			printf("2\n");
		}	
		else if(strncmp(cmd, "put", strlen("put")) == 0)/* 如果要上传文件 */
		{
			printf("3\n");
		}
		else if(strncmp(cmd, "ls", strlen("ls")) == 0)/* 如果要上传文件 */
		{
			printf("4\n");
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


