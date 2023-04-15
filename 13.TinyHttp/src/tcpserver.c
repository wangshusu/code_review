/*
 * @Author: 阿果果
 * @Date: 2023-01-29 22:49:47
 * @LastEditors: your Name
 * @LastEditTime: 2023-01-31 14:20:53
 * @Description: 
 */
/*
 * @Author: zhenxingwang
 * @Date: 2023-01-29 22:49:47
 * @LastEditors: your Name
 * @LastEditTime: 2023-01-31 10:56:48
 * @Description: 
 */
#include <stdio.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>


#define SERVER_PORT 80


void heep()
{
    unsigned short aasdf = 0x12;
    htons();
}



static int debug = 1;

void do_http_request(int cli_sock);
int get_line(int cli_sock, char *buf, int size_buf);
void do_http_responsel(int cli_sock);
void not_found(int cli_sock);
void do_http_responsed(int cli_sock, const char *path);
int headers(int cli_sock, FILE *resource);
int body(int cli_sock, FILE *resource);
void innet_error(int cli_sock);
void not_found(int cli_sock);    //404
void unimplemented(int cli_sock);//500
void bad_request(int cli_sock);  //400

int main(int argc, char *argv[]) {

    int ret = -1;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("socket error");
        return -1;
    }

    int on = 1;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (0 != ret)
    {
        perror("setsockopt error");
        return -1;
    }


    struct sockaddr_in sev_addr;
    bzero(&sev_addr, sizeof(sev_addr));
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(SERVER_PORT);
    sev_addr.sin_addr.s_addr = htonl(INADDR_ANY); //listen all ip local
    ret = bind(sockfd, (struct sockaddr *)&sev_addr, sizeof(sev_addr));
    if (-1 == ret)
    {
        perror("bind error");
        return -1;
    }
    if (debug) printf("bind port 80 succ\n");

    ret = listen(sockfd, 128);
    if (-1 == ret) 
    {
        perror("listen error");
        return -1;
    }

    while (1) 
    {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (clientfd < 0) 
        {
            perror("accept error");
            break;
        }
        
        char ip[16] = "\0";
        unsigned short port = 0;
        inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, ip, 16);
        port = ntohs(cli_addr.sin_port);
        if(debug) printf("client: %s %d\n", ip, port);
        
        do_http_request(clientfd);
        
        close(clientfd);
    }

    close(sockfd);
    return 0;

}

void do_http_request(int cli_sock) 
{
    char buf[256];
    char url[256];
    char method[256];
    char path[512];
    struct stat st;

    int len = get_line(cli_sock, buf, sizeof(buf));
    if (debug) printf("get_line : %s", buf);


    if (len > 0) // 如果读到了请求行
    {
        int i = 0;
        int j = 0;
        while(!isspace(buf[j]) && (i < sizeof(method) - 1))
        {
            method[i] = buf[j];
            j++;
            i++;
        }
        method[i] = '\0';
        //if (debug) printf("method : %s", method);

        if (0 == strncasecmp(method, "GET", i)) // 处理get请求
        {
            while(!isspace(buf[j++]));
            i = 0;

            while(!isspace(buf[j]) && (i < sizeof(url) - 1))
            {
                url[i] = buf[j];
                j++;
                i++;
            }
            if (debug) printf("method : %s\n", method);
            if (debug) printf("url : %s\n", url);

            char *pos = strchr(url, '?');
            if (pos) 
            {
                *pos = '\0';
                if (debug) printf("real url : %s\n", url);
            }

            // 执行http响应
            // 文件存在 响应 200 OK 发送html 
           
            sprintf(path, "\n./html_docs/%s", url);
            
            if (-1 == stat(path, &st))  // 不存在   响应 404 
            {
                not_found(cli_sock);
            }
            else
            {
                if (S_ISDIR(st.st_mode) == 1) // 如果是目录
                {
                    strcat(path, "/index.html");
                }
                //do_http_responsel(cli_sock);
                do_http_responsed(cli_sock, path);
            }

            
        }
        else // 别的请求
        {
            fprintf(stderr, "warning! other request[%s]\n", method); // 这句话没用
        }
        
        do
        {
            len = get_line(cli_sock, buf, sizeof(buf));
        } while (len > 0);
    }
    else // 如果没有读到
    {

    }

    
    return;
}

void do_http_responsed(int cli_sock, const char *path)
{
    int ret = 0;
    FILE *resource = NULL;

    resource = fopen(path, "r");
    if (resource == NULL) // 不存在就退出
    {
        not_found(cli_sock);
        return;
    }

    //发送http头部
    headers(cli_sock, resource);
    //发送http内容
    body(cli_sock, resource);

    fclose(resource);
}

void not_found(int cli_sock)
{
    const char * reply = "HTTP/1.0 404 NOT FOUND\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>NOT FOUND</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
	<P>文件不存在！\r\n\
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.\r\n\
</BODY>\r\n\
</HTML>";

	int len = write(cli_sock, reply, strlen(reply));
	if(debug) fprintf(stdout, "%s",reply);//--------------这里有过报错
	
	if(len <=0){
		fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
	}
}

void do_http_responsel(int cli_sock)
{
    const char *main_header = "HTTP/1.0 200 OK\r\nServer: Martin Server\r\nContent-Type: text/html\r\nConnection: Close\r\n";
    const char * welcome_content = "\
<html lang=\"zh-CN\">\n\
<head>\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\n\
<title>This is a test</title>\n\
</head>\n\
<body>\n\
<div align=center height=\"500px\" >\n\
<br/><br/><br/>\n\
<h2>大家好，欢迎来到奇牛学院VIP 试听课！</h2><br/><br/>\n\
<form action=\"commit\" method=\"post\">\n\
尊姓大名: <input type=\"text\" name=\"name\" />\n\
<br/>芳龄几何: <input type=\"password\" name=\"age\" />\n\
<br/><br/><br/><input type=\"submit\" value=\"提交\" />\n\
<input type=\"reset\" value=\"重置\" />\n\
</form>\n\
</div>\n\
</body>\n\
</html>";
    int len = write(cli_sock, main_header, strlen(main_header));
    if (debug) fprintf(stdout, "\n\n...do_heep_response...\n");
    if (debug) fprintf(stdout, "write[%d]:%s\n", len, main_header);

    char send_buf[64];
    int wc_len = strlen(welcome_content);
    len = snprintf(send_buf, sizeof(send_buf), "Content-Length: %d\r\n\r\n", wc_len);
    len = write(cli_sock, send_buf, len);

    if(debug) fprintf(stdout, "write[%d]: %s", len, send_buf);
	
	len = write(cli_sock, welcome_content, wc_len);
	if(debug) fprintf(stdout, "write[%d]: %s", len, welcome_content);
}

int get_line(int cli_sock, char *buf, int size_buf)
{
    int len = 0;
    int cout = 0;
    char ch = '\0';
    while(cout < size_buf - 1) 
    {
        len = read(cli_sock, &ch, 1);
        if (0 != len)
        {
            if ('\r' == ch)
            {
                continue;
            } 
            else if ('\n' == ch)
            {
                buf[cout] = '\0';
            }
            buf[cout] = ch;
            //if(debug) printf("read = %c\n", ch);
            cout++;
        }
        else if (-1 == len)
        {
            perror("read error");
            break;
        }
        else if (0 == len)
        {
            perror("client close");
            break;
        }
    }
    if (cout > 0) 
        buf[cout] = '\0';
    
    return cout;
}

int headers(int cli_sock, FILE *resource)
{
    struct stat st;
    int fileid = 0;
    fileid = fileno(resource);
    if (-1 == fstat(fileid, &st))
    {
        innet_error(cli_sock);
        return -1;
    }

    char temp[64] = {0};
    char buf[1024] = {0};
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
	strcat(buf, "Server: Martin Server\r\n");
	strcat(buf, "Content-Type: text/html\r\n");
	strcat(buf, "Connection: Close\r\n");

    snprintf(temp, 64, "Content-Length: %ld\r\n\r\n", st.st_size);
    strcat(buf, temp);

    if (debug) fprintf(stdout, "header : %s\n", buf);

    if (send(cli_sock, buf, strlen(buf), 0) < 0)
    {
        fprintf(stdout, "send failed.. data:%s  reason: %s\n", buf, strerror(errno));
        return -1;
    }
    return 0;
}

int body(int cli_sock, FILE *resource)
{
    char buf[1024];
    fgets(buf, sizeof(buf), resource);

    while (!feof(resource))
    {
        int len = write(cli_sock, buf, strlen(buf));
        if (len < 0)
        {
            fprintf(stderr, "send body error : reason %s\n", strerror(errno));
            break;
        }

        if (debug) fprintf(stdout, "\n%s", buf);
        fgets(buf, sizeof(buf), resource);
    }
    
}

void innet_error(int cli_sock)
{
    const char * reply = "HTTP/1.0 500 Internal Sever Error\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>Inner Error</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
    <P>服务器内部出错.\r\n\
</BODY>\r\n\
</HTML>";

	int len = write(cli_sock, reply, strlen(reply));
	if(debug) fprintf(stdout, "%s",reply);
	
	if(len <=0){
		fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
	}
}

void not_found(int cli_sock)
{
    const char * reply = "HTTP/1.0 404 NOT FOUND\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>NOT FOUND</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
	<P>文件不存在！\r\n\
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.\r\n\
</BODY>\r\n\
</HTML>";

	int len = write(cli_sock, reply, strlen(reply));
	if(debug) fprintf(stdout, "%s", reply);
	
	if(len <=0){
		fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
	}
}

void unimplemented(int cli_sock)
{
    const char * reply = "HTTP/1.0 404 NOT FOUND\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>NOT FOUND</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
	<P>文件不存在！\r\n\
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.\r\n\
</BODY>\r\n\
</HTML>";

	int len = write(cli_sock, reply, strlen(reply));
	if(debug) fprintf(stdout, "%s",reply);
	
	if(len <=0){
		fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
	}
}
