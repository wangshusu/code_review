#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    unsigned char Cmd[128];
    memset(Cmd, 0, sizeof(Cmd));	 // 清空 Cmd 里面的内容
	fgets(Cmd,sizeof( Cmd), stdin);	 /* 从键盘获取输入 */

    char *p = Cmd;
    while (*p == ' ')
    {
        p++;
    }
    

    // while (1)
    // {
    //     if (*p != ' ')
    //         break;
    //     p++;
    // }
    

    // for(int i = 0; i < strlen(Cmd); i++)
    // {
    //     if ( *p == ' ')
    //         p++;
    //     else
    //         break;
    // }

    printf("cmd=%s\n",p);

    return 0;
}
