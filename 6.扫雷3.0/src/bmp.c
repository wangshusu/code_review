#include "bmp.h"
#include "lcd.h"

/*
    ��ʾbmpͼƬ����
    @char *bmp_file:·��
    @int x0, int y0 : ���ĸ�����ʾͼƬ
    �޷���ֵ
*/
void display_bmp(char *bmp_file, int x0, int y0, int form)
{
    int fd = open(bmp_file, O_RDONLY);//����򿪾�Ҫ�ж�Ӧ�Ĺر�
    if (fd == -1)
    {
        printf("failed to open %s\n", bmp_file);
        return;
    }

    int width, height;
    short depth;

    lseek(fd, 0x12, SEEK_SET);
    read(fd, &width, 4);

    lseek(fd, 0x16, SEEK_SET);
    read(fd, &height, 4);

    //printf("%d X %d\n", width, height);

    lseek(fd, 0x1c, SEEK_SET);
    read(fd, &depth, 2);
    
    if (  !(depth == 24 || depth == 32) )//������Ƿ��Ϲ涨���ļ� ����Ҫ�ر�
    {
        printf("Sorry, Not Supported\n");
        close(fd);
        return ;
    }

    //�������鱣����ÿ�����ص����ɫֵ
    //  abs(width) * abs(height) * (depth/8)

    // һ��ʵ���ֽ���:  width * (depth/8)
    // һ�е�ĩβ������伸�� ������ laizi��
    int laizi = 0; //һ��ĩβ���ġ����ӡ���
    int line_valid_bytes = abs(width) * (depth/8); //һ����Ч���ֽ���

    if ( line_valid_bytes % 4 )
    {
        laizi =  4 - line_valid_bytes % 4;
    }

    int line_total_bytes; //һ���ܹ��ֽ���
    line_total_bytes = line_valid_bytes + laizi;

    int total_bytes; //��������������ֽ���
    total_bytes = line_total_bytes * abs(height);

    char *pixel = malloc( total_bytes );
    lseek(fd, 54, SEEK_SET);
    read(fd, pixel, total_bytes);

    unsigned char b,g,r,a;
    int color;
    int i = 0;
    int x,y;

    int colorbuf[WIDTH][HEIGHT] = {0};
  
    for (y = 0; y < abs(height); y++)
    {
        for (x = 0; x < abs(width); x++)
        {
            b = pixel[i++];
            g = pixel[i++];
            r = pixel[i++];
            if (depth == 32)
            {
                a = pixel[i++];
            }
            else
            {
                a = 0;
            }
            color = (a << 24) | (r << 16) | (g << 8) | (b);

            if(x < 800 && (abs(height) -1 - y) < 480)//��ֹԽ��
            {
                colorbuf[x][abs(height) -1 - y] = color;
            }
        }
        i += laizi;
    }
    
    close(fd);//�������  �ҾͰ����ر���

    switch (form)
    {
    case 0:
        lcd_draw_point_form0(colorbuf,x0,y0,width,height);
        break;

    default:
        break;
    }
    
    free(pixel);
    
}

