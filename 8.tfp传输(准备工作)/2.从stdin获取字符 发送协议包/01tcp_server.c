#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

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
	struct sockaddr_in client1;
	int confd1 = confd;
	
	client1 = client;
	socklen_t len1 = sizeof(client1);

	uint8_t buf[__PAGE_MAX_LEN] = {0};
	uint8_t file_name[32] = {0};
	int ret ;
	uint8_t cmd;
	MyProtMsg *msg;

	printf("当前线程进入任务\n");
	while(1)
	{
		ret = recv(confd1,buf,sizeof(buf),0);
		if(ret > 0)
		{
			printf("Recv From [IP : %s] [PORT : %d]\n",inet_ntoa(client1.sin_addr),ntohs(client1.sin_port));

			/******* 解包并判断 ******/
			cmd = analysis_FromClient(buf);
			if (cmd == __CMD_LS)/* 如果是要文件列表 */
			{
				msg = server_Head_Init(__CMD_LS, 3);
				uint8_t *file_name_p = (uint8_t *)&msg->file_haed.data;

				//char *p2 = "aa.txt";
				//add_FileNameToData(p2, &file_name_p);

				ret = send(confd1, (void *)msg,sizeof(MyProtMsg), 0);
				printf("ls\n");
			}
			else if (cmd == __CMD_GET)/* 如果是要获取文件 */
			{
				memset(file_name, 0, sizeof(file_name));
				

				printf("get\n");
				/* 如果文件存在,进行打包 不存在,返回通知客户端没有文件 */
				
			}
			else if (cmd == __CMD_PUT)/* 如果是要上传文件 */
			{
				memset(file_name, 0, sizeof(file_name));

				
				printf("put\n");
				/* 发送客户端,告知保存的文件名是合法,接着进行传输 */
				
			}
			else if (cmd == __CMD_BYE)/* 如果是要拜拜 */
			{
				printf("haha byebyr\n");
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

