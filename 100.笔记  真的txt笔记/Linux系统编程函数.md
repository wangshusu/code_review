## 0.0 标准IO图示

![image-20230203221248225](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20230203221248.png)



## 0.1 标准IO 和 系统IO(文件IO)

![image-20230203221330441](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20230203221330.png)





## 0.2 虚拟地址空间

![image-20230203221655598](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20230203221655.png)





## 0.3 文件描述符

![image-20230203221726040](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20230203221726.png)







##1.0 fcntl 函数

```c
#include <unistd.h> 
#include <fcntl.h> 
int fcntl(int fd, int cmd, ... /* arg */); 
    功能：改变已打开的文件性质，fcntl针对描述符提供控制。 
    参数：
        fd ：操作的文件描述符 
        cmd：操作方式 
        arg：针对cmd的值，fcntl能够接受第三个参数int arg。 新的描述符的最小值
    返回值：
    	成功：返回某个其他值 
    	失败：-1
```

==fcntl函数有5种功能：==

* 1) 复制一个现有的描述符（cmd = F_DUPFD） 
* 2) 获得／设置文件描述符标记(cmd = F_GETFD或F_SETFD)  改状态下 arg 为 fd 的新的状态标记( 1 为关闭 ,  0 为打开  )  
* **3) 获得／设置文件状态标记(cmd = F_GETFL或F_SETFL)**
* 4) 获得／设置异步I/O所有权(cmd = F_GETOWN或F_SETOWN) 
* 5) 获得／设置记录锁(cmd = F_GETLK, F_SETLK或F_SETLKW) 

在修改文件描述符标志或文件状态标志时必须谨慎，先要取得现在的标志值，然后按照希望修改它，最后设置新标志值。不能只是执行F_SETFD或F_SETFL命令，这样会关闭以前设置的标志位。 

详情参考

* [(53条消息) fcntl函数详解_这儿的天空的博客-CSDN博客_fcntl函数](https://blog.csdn.net/qq_36754075/article/details/107625050?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522165717661916782350865930%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=165717661916782350865930&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-2-107625050-null-null.142^v31^control,185^v2^control&utm_term=fcntl&spm=1018.2226.3001.4187) 

* [(53条消息) fcntl函数的用法总结_rotation ㅤ   的博客-CSDN博客_fcntl](https://blog.csdn.net/fengxinlinux/article/details/51980837?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522165717661916782350865930%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=165717661916782350865930&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-51980837-null-null.142^v31^control,185^v2^control&utm_term=fcntl&spm=1018.2226.3001.4187) 



## 1.1  open (文件io)

```c
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 

int open(const char *pathname, int flags); 
int open(const char *pathname, int flags, mode_t mode); 
	功能：
		打开文件，如果文件不存在则可以选择创建。 
	参数：
	pathname：文件的路径及文件名 
	flags：	 打开文件的行为标志，必选项 O_RDONLY, O_WRONLY, O_RDWR
	mode：    这个参数，只有在文件不存在时有效，指新建文件时指定文件的权限 
	返回值：
		成功：成功返回打开的文件描述符 
		失败：-1
```

```C++
int fd = open("share/workfile", O_RDONLY);
if (fd == -1) {
    printf("open error!\n");
}
```



## 1.2 read (文件io)

```C++
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```



## 1.3 write (文件io)

```C++
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```



## 1.4 close (文件io)

```C++
#include <unistd.h>

int close(int fd);
```





## 1.5 fopen(标准io)

```C++
#include <stdio.h>

FILE *fopen(const char *path, const char *mode);
参数：
    const  char *path 打开指定路径下的文件。
    const char *mode 表示文件的打开方式：
    mode	含义
r	只读方式打开，文件位置指针位于文件的起始处【文件必须事先存在，否则出错】
r+	只读写方式打开，文件位置指针位于文件的起始处【文件必须事先存在，否则出错】
w	有则清空，无则创建。文件位置指针位于文件的起始处
w+	有则清空，无则创建。文件位置指针位于文件的起始处
a	以附加的方式打开，文件位置指针位于未见末尾处
a+	以读或附加的方式打开，文件位置指针取决于用户的操作方式。读则在起始处，附加则在末尾处。
     返回值：打开成功则返回FILE 类型指针，失败则返回空指针，并写入errno值。
```

```C++
	#include <stdio.h>    
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    int main ()
    {
    FILE *f = NULL;
    f = fopen("tmp","w");
    if(f == NULL)
    {
    //fprintf(stderr,"fopen() failed ! errno = %d\n",errno);
    //查看出错原因。去：/usr/include/asm-generic/errno-bash.h
    //使用perro函数
    //perror("fopen()");
    //另外一个好用的函数
    fprintf(stderr,"fopen():%s\n",strerror(errno));
    exit(1);
    }
    puts("ok");
    fclose(f);
    //exit(0);
    }
```



## 1.6 fread(标准io)

```C++
#include <stdio.h> //作用：二进制流的输入输出

 size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

	注意：size是每一个块的大小，nmemb是块的个数。
	返回值：读取或写入的字节个数【正确】。读完或出错返回0值
```



## 1.7 fwrite(标准io)

```C++
#include <stdio.h> //作用：二进制流的输入输出

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
	注意：size是每一个块的大小，nmemb是块的个数。
	返回值：读取或写入的字节个数【正确】。读完或出错返回0值
```



> fread 与 fwrite 结合 实现文件复制

```C++
#include <stdio.h>

#define MAX_BUF_SIZE 1024
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage....\n");
        exit(1);
    }
    FILE *fsp, fdp;
    char buf[MAX_BUF_SIZE];
    fsp = fopen("\share\1.txt", "r");
    if (fsp == NULL) {
        //fprintf(stderr, "fopen error! %s", ster);
        fprintf(stderr,"fopen():%s\n",strerror(errno));
        exit(1);
    }
    
    fdp = fopen("\share\2.txt", "w");
    if (fdp == NULL) {
        //fprintf(stderr, "fopen error! %s", ster);
        fprintf(stderr,"fopen():%s\n",strerror(errno));
        fclose(fsp);
        exit(1);
    }
    
    int n, res;
    while ((n = fread(buf, 1, MAX_BUF_SIZE, fsp))) {
        res = fwrite(buf, 1, n, fdp);
        n = n - res;
        while (n > 0) {
            res = fwrite(buf, 1, n, fdp);
            n = n - res;
        }
    }
    exit(0);
}
```



## 1.8 fclose(标准io)

```C++
#include <stdio.h>

int fclose(FILE *fp);
```

