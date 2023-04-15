#include "ftp_agree.h"
#include <string.h>

#define TEST 0

/************ 这个文件中要实现对数据包的完全分析 **********/

//这里要解决对接收到的tcp包的解析问题
int main(int argc, char const *argv[])
{
    #if TEST
    printf("sizeof = %ld\n",sizeof(MyProtMsg));
    char *buf = calloc(1,__PAGE_MAX_LEN);
    MyProtMsg *p = client_Head_Init(0x01);
    head_Print((uint8_t *)p);

    memcpy(buf, p, __PAGE_MAX_LEN);
    head_Print(buf);

    #else

    char buf[128] = "";
    
    uint8_t *file_name_p = buf;//add喊
    char *p2 = "aa.txt";
    char *p3 = "bb.txt";
    char *p4 = "cc.txt";

    add_FileNameToData(p2, &file_name_p);
    add_FileNameToData(p3, &file_name_p);
    add_FileNameToData(p4, &file_name_p);
    //printf("add_data = %#x\n",*(int *)buf);

    uint8_t **pa = analysis_FileNameData(buf, 3);

    printf("%s\n",(char *)pa[0]);
    printf("%s\n",(char *)pa[1]);
    printf("%s\n",(char *)pa[2]);

    #endif
    //analysis_data(p2, 3)
}

