#include "ftp_agree.h"


//检测函数
void head_Print(uint8_t *p)
{
    MyProtMsg *ptr = (MyProtMsg *)p;
    // printf("%#x\n",(uint8_t *)p);
    // printf("%#x\n",(uint8_t *)&ptr->magic);
    // printf("%#x\n",(uint8_t *)&ptr->cmdno);
    // printf("%#x\n",(uint8_t *)&ptr->id);
    // printf("%#x\n",(uint8_t *)&ptr->result);
    // printf("%#x\n",(uint8_t *)&ptr->file);

    printf("\n");
    printf("%#x\n",(ptr->pag_head.magic_head));
    printf("%#x\n",(ptr->pag_head.cmdno));
    printf("%#x\n",(ptr->pag_head.id));
    printf("%#x\n",(ptr->pag_head.result));
    printf("%#x\n",(ptr->pag_head.file));
    printf("%#x\n",(ptr->pag_head.magic_end));
}

/*
    client_Head_Init:对客户端的头进行初始化
        @cmdon:传入指令( 1.ls 2.get file 3.put file 4.bye )
        @返回值:tcp头包
*/
MyProtMsg *client_Head_Init(uint8_t cmdon)
{
    /******** 开辟的堆区空间释放说明 *****/
    /*
        开辟的堆区空间要再发送完毕之后进行释放
        没有二次开辟 一次释放就可以了
        free(msg);  
    */
    MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));//tcp 数据包  client ---> server
    //MyProtMsg msg;
    //uint8_t *ptr = (uint8_t *)msg;

    //校对头 2字节
    *(uint16_t *)&(msg->pag_head.magic_head) = __MAGIC_HEAD;

    //命令
    *(uint8_t *)&(msg->pag_head.cmdno) = cmdon;

    //标志发送人
    *(uint8_t *)&(msg->pag_head.id) = __ID_CLIENT;

    //服务端执行命令是否成功  这里是客户端 随便了
    *(uint8_t *)&(msg->pag_head.result) = __RESULT_SUCCESS;

    //文件发送状态  这里是客户端 随便了
    *(uint8_t *)&(msg->pag_head.file) = __FILE_EMPTY;

    //校对尾 2字节
    *(uint16_t *)&(msg->pag_head.magic_end) = __MAGIC_END;

    return msg;
}

/*
    server_Head_Init:对服务端的头进行初始化
        @cmdon:传入指令
        @返回值:tcp头包
*/
MyProtMsg *server_Head_Init(uint8_t cmdon, uint32_t remain_pagnum)
{
    /******** 开辟的堆区空间释放说明 *****/
    /*
        开辟的堆区空间要再发送完毕之后进行释放
        没有二次开辟 一次释放就可以了
        free(msg);  
    */
    MyProtMsg *msg = (MyProtMsg *)calloc(1,sizeof(MyProtMsg));//tcp 数据包  client ---> server
    //MyProtMsg msg;
    //uint8_t *ptr = (uint8_t *)msg;

    //校对头 2字节
    *(uint16_t *)&(msg->pag_head.magic_head) = __MAGIC_HEAD;

    //命令
    *(uint8_t *)&(msg->pag_head.cmdno) = cmdon;

    //标志发送人
    *(uint8_t *)&(msg->pag_head.id) = __ID_SERVER;

    //服务端执行命令是否成功  这里是客户端 随便了
    *(uint8_t *)&(msg->pag_head.result) = __RESULT_SUCCESS;

    //文件发送状态  这里是客户端 随便了
    *(uint8_t *)&(msg->pag_head.file) = __FILE_LS;

    //校对尾 2字节
    *(uint16_t *)&(msg->pag_head.magic_end) = __MAGIC_END;

    msg->file_haed.remain_pagnum = remain_pagnum;

    return msg;
}

//解析来自客户端的数据包
uint8_t analysis_FromClient(uint8_t *p)
{
    MyProtMsg *ptr = (MyProtMsg *)p;
    //注意 这里需要取值的话 *(uint16_t *)&(ptr->pag_head.magic)
                        // *(uint8_t *)&(ptr->pag_head.cmdno)  采用这种形式

    /* 对文件格式转换之后进行判断 */
    if ((ptr->pag_head.magic_head) == __MAGIC_HEAD && 
        (ptr->pag_head.magic_end) == __MAGIC_END &&
        (ptr->pag_head.id) == __ID_CLIENT)//如果是客户端发来的包 还能解析
    {
        return (ptr->pag_head.cmdno);
    }
    else
    {
        printf("cant analysis this package!\n");
        return __CMD_ERROR;
    }

}

//解析来自服务端的数据包
uint8_t analysis_FromServer(uint8_t *p)
{
    MyProtMsg *ptr = (MyProtMsg *)p;
    //注意 这里需要取值的话 *(uint16_t *)&(ptr->pag_head.magic)
                        // *(uint8_t *)&(ptr->pag_head.cmdno)  采用这种形式

    /* 对文件格式转换之后进行判断 */
    if ((ptr->pag_head.magic_head) == __MAGIC_HEAD && 
        (ptr->pag_head.magic_end) == __MAGIC_END &&
        (ptr->pag_head.id) == __ID_SERVER)//服务端来的包  并且能够解析
    {
        return (ptr->pag_head.file);
    }
    else
    {
        printf("cant analysis this package!\n");
        return __CMD_ERROR;
    }
}

/* 
    add_data:将文件名按照指定的格式进行写入
        @file_nameL:文件名
        注意:该函数必须配合全局变量 file_name_p 使用
        file_name_p 指向你要写入的数组的地址
        in: char buf[128]; file_name_p = buf;
*/
uint8_t *add_FileNameToData(uint8_t *file_name, uint8_t **file_name_p)//
{
    uint32_t len = strlen(file_name);   /* 计算文件名的长度 */
    *(uint32_t *)*file_name_p = len;     /* 将长度先拷入 */
    *file_name_p += 4;

    memcpy(*file_name_p, file_name, len);/* 拷入文件名 */
//printf("%s\n",*file_name_p);    
    *file_name_p += len;
//printf("p = %#x\n",file_name_p);
//printf("len = %s\n",file_name_p);
//printf("加入成功!~\n");
}

/*
    analysis_FileNameData:分析只带有文件名的 data 的数据包
        @ptr:data的首地址
        @remain_pagnum:文件名称的数量
*/
uint8_t **analysis_FileNameData(uint8_t *ptr, uint32_t remain_pagnum)
{
printf("开始解析!\n");
    
    uint8_t **p = (uint8_t **)calloc(remain_pagnum, sizeof(uint8_t *));//开辟对应空间的二维数组
    for(int i = 0; i < remain_pagnum; i++)
    {
        p[i] = (uint8_t *)calloc(1, 32);//32为单个文件名的长度
    }

    uint32_t len = 0;
    for(int i = 0; i < remain_pagnum; i++)//按照个数进行解析
    {
        len = *(uint32_t *)ptr;
        ptr += 4;
    
        memcpy(p[i], ptr, len);
        ptr += len;

//printf("%s\n",(char *)p[i]);
    }
    printf("解析成功\n");

    /** 释放这里的堆区空间 后面用完了释放 **/
    /*
        for(int i = 0; i < remain_pagnum; i++)
        {
            free(p[i]);
        }
        free(p);
        p = NULL;
    */
    
    return p;
}

void free_FileNameData(uint8_t **p, uint32_t remain_pagnum)
{
    for(int i = 0; i < remain_pagnum; i++)
    {
        free(p[i]);
    }
    free(p);
    p = NULL;
}