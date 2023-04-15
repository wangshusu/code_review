#include <stdio.h>


int main()
{
    char buf[128] = "";
    fgets(buf, sizeof(buf), stdin);
    char buf1[32] = {0};
    char buf2[32] = {0};
    char buf4[32] = {0};
    sscanf(buf, "%s%s%s", buf1, buf2, buf4);

    printf("%s\n",buf1);
    printf("%s\n",buf2);
    printf("%s\n",buf4);
}