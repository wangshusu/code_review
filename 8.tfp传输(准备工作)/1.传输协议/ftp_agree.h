#ifndef __FTP_AGREE__
#define __FTP_AGREE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   uint8_t;    /* 无符号8位数据  */
typedef signed char     int8_t;     /* 有符号8位数据  */
typedef unsigned short  uint16_t;   /* 无符号16位数据 */
typedef signed short    int16_t;    /* 有符号16位数据 */
typedef unsigned int    uint32_t;   /* 无符号32位数据 */
typedef signed int      int32_t;    /* 有符号32位数据 */

/*** 全局变量区 ***/
//uint8_t *file_name_p;

#define TIMEOUT 3   //超时的标准
#define __PAGE_MAX_LEN 1200 // 整个数据包最大的字节数
#define __FILE_LEN 128

#define __MAGIC_HEAD  0xEB90 //头包
#define __MAGIC_END   0x90EB //尾包

#define __CMD_LS  0x01 //cmd命令
#define __CMD_GET 0x02 
#define __CMD_PUT 0x03 
#define __CMD_BYE 0x04 
#define __CMD_ERROR 0XFF

#define __ID_CLIENT 0x01 //发送方
#define __ID_SERVER 0x02 

#define __RESULT_SUCCESS  0x00  //服务器执行命令是否成功
#define __RESULT_FAIL     0x01

#define __FILE_EMPTY 0x00 //文件状态
#define __FILE_HAVE  0x01
#define __FILE_LS    0x02

typedef struct myprot_haed // myprot 头文件的大小 = file_haed.file_size + 9 字节
{
 
    uint16_t magic_head;  //校对头    0xEB 0x90   
    uint8_t  cmdno;  //命令 
    uint8_t  id;     //客户端发送 1  服务端发送 2
    uint8_t  result; //服务端执行命令是否成功
    uint32_t pkglen; //总长度  
    uint8_t  file;  //文件发送状态  0没有文件 1有文件 2文件名(ls专用)
    uint16_t magic_end;  //校对尾    0xEB 0x90  
}MyProt_Head;

typedef struct myfile_head //  file头的字节大小 = 12 + file_size 
{
    uint32_t file_name;//文件名称
    uint32_t remain_pagnum;//剩余数据包的数量
    uint32_t file_size;//数据包的大小 (字节)   //规定最大不超过 1024    也就是数据包整体的大小不超过 1024 + 12 + 9 
    uint8_t  data[1024];
}MyFile_Head;

typedef struct myprotmsg //TCP 数据包头
{
    MyProt_Head pag_head;//tcp数据包头
    MyFile_Head file_haed;//file数据包头
}MyProtMsg;

/***************** MyFile_Head **************/
/*
    如果是ls 或者 get  那么data 里面的数据就是  长度 + 文件名 的形式 中间没有空格
    "0x051.txt0x052.txt0x053.txt"
*/

/*
    head_Print:检测tcp包头函数
*/
void head_Print(uint8_t *p);

/*
    client_Head_Init:对客户端的头进行初始化
        @cmdon:传入指令( 1.ls 2.get file 3.put file 4.bye )
        @返回值:tcp头包
*/
MyProtMsg *client_Head_Init(uint8_t cmdon);

/*
    server_Head_Init:对服务端的头进行初始化
        @cmdon:传入指令
        @返回值:tcp头包
*/
MyProtMsg *server_Head_Init(uint8_t cmdon, uint32_t remain_pagnum);

//解析来自客户端的数据
uint8_t analysis_FromClient(uint8_t *p);

//解析来自服务端的数据包
uint8_t analysis_FromServer(uint8_t *p);

/* 
    add_data:将文件名按照指定的格式进行写入
        @file_nameL:文件名
        注意:该函数必须配合全局变量 file_name_p 使用
        file_name_p 指向你要写入的数组的地址
        in: char buf[128]; file_name_p = buf;
*/
uint8_t *add_FileNameToData(uint8_t *file_name, uint8_t **file_name_p);

/*
    analysis_FileNameData:分析只带有文件名的 data 的数据包
        @ptr:data的首地址
        @remain_pagnum:文件名称的数量
*/
uint8_t **analysis_FileNameData(uint8_t *ptr, uint32_t remain_pagnum);


#endif
