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
#define __FILE_NAME_LEN 32  // 文件名的长度
#define __FILE_LEN 128  // 文件名的长度
#define __DATA_SIZE 1024 // 数据包的大小

#define __MAGIC_HEAD  0xEB90 //头包
#define __MAGIC_END   0x90EB //尾包

#define __ID_CLIENT 0x01 //发送方 客户端发送 1  服务端发送 2
#define __ID_SERVER 0x02 

#define __CMD_LS  0x01 //cmd命令 1.ls 2.get file 3.put file 4.bye
#define __CMD_GET 0x02 
#define __CMD_PUT 0x03 
#define __CMD_BYE 0x04 
#define __CMD_ERROR 0XFF

#define __RESULT_SUCCESS  0x00  // result 服务器执行命令是否成功
#define __RESULT_FAIL     0x01

#define __FILE_EMPTY 0x00 // file 文件状态
#define __FILE_HAVE  0x01
#define __FILE_LS    0x02

#define __FILE_NOEND 0x00 // 文件还没有传完
#define __FILE_ISEND 0x01 // 文件已经传完

#define __FILE_NORECV 0x00
#define __FILE_ISRECV 0x01 //接收到了


#define __FILE_DATE 1024  //数据包date的最大字节数

typedef struct myprot_haed // myprot 头文件的大小 = file_haed.file_size + 9 字节
{
    uint16_t magic_head;  //校对头    0xEB 0x90
    uint8_t  id;     //客户端发送 1  服务端发送 2

    //客户端
    uint8_t  cmdno;  //命令       1.ls 2.get file 3.put file 4.bye
    
    //服务端
    uint8_t  result; //服务端执行命令是否成功  0失败 1成功
    uint8_t  file;  //文件发送状态  0没有文件 1发送文件 2名单列表(ls专用)

    uint16_t magic_end;  //校对尾    0xEB 0x90  
}MyProt_Head;

typedef struct myfile_head //  file头的字节大小 = 12 + file_size 
{
    //通用 数据是否传送完毕
    uint32_t file_end;// 0没有传完  1传完了

    //命令ls专用
    uint32_t file_num;//文件名的个数

	//传文件专用
    uint32_t file_recv;
    uint32_t file_size;//数据包里内容的字节数
    uint8_t  file_date[__DATA_SIZE];//数据正文

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
void head_Print(MyProtMsg *ptr);

/*
    client_Head_Init:对客户端的头进行初始化
*/
void client_Head_Init(MyProtMsg *msg);

/*
    server_Head_Init:对服务端的头进行初始化
*/
void server_Head_Init(MyProtMsg *msg);

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

void free_FileNameData(uint8_t **p, uint32_t remain_pagnum);
#endif
