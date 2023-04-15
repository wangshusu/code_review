#include "ftp_agree.h"


/* 
    head_Print:检测函数
*/
void head_Print(MyProtMsg *ptr)
{
    //MyProtMsg *ptr = (MyProtMsg *)p;
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
*/
void client_Head_Init(MyProtMsg *msg)
{
    memset(msg, 0, sizeof(MyProtMsg)); // 清空
    msg->pag_head.magic_head = __MAGIC_HEAD; //校对头
    msg->pag_head.magic_end  = __MAGIC_END;  //校对尾
    msg->pag_head.id = __ID_CLIENT; // 客户端
}

/*
    server_Head_Init:对服务端的头进行初始化
*/
void server_Head_Init(MyProtMsg *msg)
{
    memset(msg, 0, sizeof(MyProtMsg)); // 清空
    msg->pag_head.magic_head = __MAGIC_HEAD; // 校对头
    msg->pag_head.magic_end  = __MAGIC_END;  // 校对尾
    msg->pag_head.id = __ID_SERVER; // 服务器端
}

/* 
    analysis_FromClient:解析来自客户端的数据包
        @p:接收到的数据 buf_tcp 的地址
        @返回值:
            成功: 返回值对应的指令
            失败: 返回 __CMD_ERROR
*/
uint8_t analysis_FromClient(uint8_t *p)
{
    MyProtMsg *ptr = (MyProtMsg *)p; // 强转为Tcp数据包类型

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

/* 
    analysis_FromServer:解析来自服务端的数据包
        @p:接收到的数据 buf_tcp 的地址
        @返回值:
            成功: 返回值对应的指令
            失败: 返回 __CMD_ERROR
*/
uint8_t analysis_FromServer(uint8_t *p)
{
    MyProtMsg *ptr = (MyProtMsg *)p; // 强转为Tcp数据包类型

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
    add_FileNameToData:将文件名按照指定的格式进行写入
        @file_nameL:文件名
        @file_name_p:二级指针 原方向指向 data 首元素地址 ↓↓↓↓↓↓↓↓
        注意:该函数的 file_name_p 为二级指针,最开始是指向 data 的首元素位置
             在使用完成之后一定要将它 复位!!!!
*/
uint8_t *add_FileNameToData(uint8_t *file_name, uint8_t **file_name_p)//
{
    uint32_t len = strlen(file_name); /* 计算文件名的长度 */
    *(uint32_t *)*file_name_p = len;  /* 将长度先拷入 */
    *file_name_p += 4;

    memcpy(*file_name_p, file_name, len); /* 拷入文件名 */
    *file_name_p += len;
//printf("p = %#x\n",file_name_p);
//printf("len = %s\n",file_name);
//printf("加入成功!~\n");
}

/*
    analysis_FileNameData:分析只带有文件名的 data 的数据包
        @ptr:data的首地址
        @remain_pagnum:文件名称的数量
        @返回值:
            成功: 返回开辟的二维数组的头指针
            失败: 返回 NULL
*/
uint8_t **analysis_FileNameData(uint8_t *ptr, uint32_t remain_pagnum)
{
//printf("开始解析!\n");
    uint8_t **p = (uint8_t **)calloc(remain_pagnum, sizeof(uint8_t *));//开辟对应空间的二维数组
    if (p == NULL)
    {
        printf("analysis_FileNameData calloc error!\n");
        return NULL;
    }

    for(int i = 0; i < remain_pagnum; i++)
    {
        p[i] = (uint8_t *)calloc(1, __FILE_NAME_LEN); // 32为单个文件名的长度
        if (p[i] == NULL)
        {
            printf("analysis_FileNameData calloc error!\n");
            return NULL;
        }
    }

    uint32_t len = 0;
    for(int i = 0; i < remain_pagnum; i++)//按照个数进行解析
    {
        len = *(uint32_t *)ptr;
        ptr += 4;
    
        memcpy(p[i], ptr, len);
        ptr += len;
//printf("file_name = %s\n",p[i]);
    }

    return p;
}

/*
    free_FileNameData:将开辟的二维数组销毁
        @p:二维数组的指针
        @remain_pagnum:开辟的一维数组的个数
*/
void free_FileNameData(uint8_t **p, uint32_t remain_pagnum)
{
    for(int i = 0; i < remain_pagnum; i++)
    {
        free(p[i]);
    }
    free(p);
    p = NULL;
}