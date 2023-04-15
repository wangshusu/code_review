#include "lcd.h"

#include "ziku.h"
//                                                    printf("%s %d \n",__FUNCTION__,__LINE__);
int fb_fd = -1; //��Ļ�ļ����ļ�������
int* plcd = NULL;//ָ���ں�֡���������׵�ַ
int LineColor;
int BackColor;

int arr[800][480];//������ɫ
int arr_x[16];//ÿ�������xλ�� ���Ͻ�
int arr_y[11];//ÿ�������yλ�� ���Ͻ�

//lcd ����׼������
void lcd_open(void)
{	
    int fd = open("/dev/fb0", O_RDWR);
	if (fd == -1)
	{
		printf("failed to open /dev/fb0\n");
		return ;
	}
    fb_fd = fd;
    

    plcd = mmap(
                NULL,
                800*480*4,
                PROT_WRITE,
                MAP_SHARED,
                fd,
                0    );
    if (plcd == MAP_FAILED)
    {
        printf("Sorry,failed to mmap\n");
        return ;
    }
    
    LineColor = 0xb0b0b0;
    BackColor = 0xffff;
}

//lcd ��Դ���պ���
void lcd_close(void)
{
    munmap(plcd, 800*480*4);//�ر�ӳ��
    close(fb_fd);//�ر���Ļ�ļ�
}


//lcd_draw_point: ����(���ض������ص�һ��ָ������ɫֵ)
//@x: ���x������
//@y: ���y������
//@color: ��ɫֵ
void lcd_draw_point(int x, int y, int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + y*800 +x) = color;
    }
}


//lcd_draw_point_base: ���� �ֱ�ˢ�� 1
void lcd_draw_point_form0(int colorbuf[WIDTH][HEIGHT],int x0,int y0,int width,int hight)
{
    for(int i = 0 ; i < width ; i++)
    {
        for(int j = 0 ; j < hight ; j++)
        {                                                    
            *(plcd + (j + y0)*800 + i + x0) = colorbuf[i][j];//s++;
        }
    }
}


//"����": ����Ļ��ÿһ�����ص㣬��ʾ��ͬ����ɫ
void lcd_clear_screen(int color)
{
    int x, y;

    for (y = 0 ; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            lcd_draw_point(x,y, color);
        }
    }
}

//����������
void lcd_draw_rectangle(int x0, int y0,
                        int w,  int h,
                        int color)
{
    //(x,y)Ϊ�þ����ڵ�����һ���������
    //  x >= x0 &&  x < x0 + w
    //  y >= y0 &&  y < y0 + h
    int x,y;

    for (y = y0; y < y0 + h; y++)
    {
        for (x = x0; x < x0 + w; x++)
        {
            lcd_draw_point(x,y,color);
        }

    }
}


//����ԲȦ
void lcd_draw_circle(int x0, int y0,
                     double r, int color)
{
    //(x,y)ΪԲ�ڵ�����һ��������
    // (x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r
    int x,y;

    for (y = 0; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            if ((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r*r)
            {
                lcd_draw_point(x,y,color);
            }
        }
    }

}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(int x,int y,unsigned char num,unsigned char size,unsigned char mode,int color)
{
	int temp,t1;
	int y0,t;
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)return;//��֧�ֵ�size ֧��12/16/20/24/28/32/36/40/44/48
	if(size > 40)
	{
		x+=(size-40)/2;//�滻�����
		y+=(size-40)/2;
		size = 40;
	}
	y0=y;
	int csize=( size/8 + ((size%8)?1:0)) * (size/2);//�õ�����һ���ַ���Ӧ������ռ���ֽ��� size/2 ������ʾ
	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ�����ģ��

	for(t=0;t<csize;t++)
	{
		switch (size)
		{
		case 12:	temp=asc2_1206[num][t];break;//����1206����
		case 16:	temp=asc2_1608[num][t];break;//����1608����
		case 20:	temp=asc2_2010[num][t];break;//����1608����
		case 24:	temp=asc2_2412[num][t];break;//����1608����
		case 28:	temp=asc2_2814[num][t];break;//����1608����
		case 32:	temp=asc2_3216[num][t];break;//����1608����
		case 36:	temp=asc2_3618[num][t];break;//����1608����	
		case 40:	temp=asc2_4020[num][t];break;//����1608����
		//case 44:	temp=asc2_4422[num][t];break;//����1608����		(��Ҫͬ���Ķ�ziku.h��lcd.c 208)
		//case 48:	temp=asc2_4824[num][t];break;//����1608����
		default:
			return ;
		}
		
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){lcd_draw_point(x,y,color);}
			else if(mode==0){lcd_draw_point(x,y,0xffffff);}//�б���ɫģʽ Ĭ�ϰ�ɫ����ɫ
			temp<<=1;
			y++;
			if(y>=480)return;//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=800)return;//��������
				break;
			}
		}
	}
}

//x,y:�������
//width,height:�����С  
//size:�����С
//(*p):�ַ�����ʼ��ַ		  
//color:������ɫ
void LCD_ShowString(int x,int y,int width,int height,unsigned char size,unsigned char *p,int color)
{

	int x0=x;
	width+=x;
	height+=y;
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)//��֧�ֵ�size ֧��12/16/20/24/28/32/36/40/44/48
	{
		printf("�ֿ���û�и��ֺ���ģ\n");
		return;//��֧�ֵ�size 
	}
	while((*p)!='\0')//�ж��Ǵ�ӡ���
	{
		if(x>=width){x=x0;y+=size;}//�жϺ����Ƿ񳬳��������������
		if(y>=height)break;//�˳�
		LCD_ShowChar(x,y,*p,size,1,color);//0 ��ɫ���屳�� 1 �ޱ���ɫ
		x+=size/2;//��ǵ���һ���ֵ�X��λ��
		p++;//��һ����
	}
}


//��ָ��λ����ʾһ������
//x,y:��ʼ����
//num:Ҫ��ʾ�ĺ������ֿ��е�λ��
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowHanzi(int x,int y,unsigned char num,int size,unsigned char mode,int color)
{
	unsigned char temp,t1;
	int y0=y,t;
	int csize=( size/8 + ((size%8)?1:0)) * (size);//�õ�һ�����ֶ�Ӧ������ռ���ֽ���

	for(t=0;t<csize;t++)
	{
		if(size==40)temp=Chinese_4040[num][t];//����4040����
		else if(size==48)temp=Chinese_4848[num][t];//����4824����
		else return;//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){lcd_draw_point(x,y,color);}
			else if(mode==0){lcd_draw_point(x,y,0xffffff);}//�б���ɫģʽ Ĭ�ϰ�ɫ����ɫ
			temp<<=1;
			y++;
			if(y>=480)return;//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=800)return;//��������
				break;
			}
		}
	}
}

//x,y:�������
//width,height:�����С  
//size:�����С
//(*p):�ַ�����ʼ��ַ		  
//color:������ɫ
void LCD_ShowHanzis(int x,int y,int width,int height,int size,unsigned char *p,int color)
{
	int x0=x;
	width+=x;
	height+=y;

	while((*p)!='\0')//�ж��Ǵ�ӡ���
	{
		if(x>=width){x=x0;y+=size;}//�жϺ����Ƿ񳬳��������������
		if(y>=height)break;//�˳�
		LCD_ShowHanzi(x,y,(*p - '0'),size,1,color);//0 ��ɫ���屳�� 1 �ޱ���ɫ		���������ַ�����������ʾ����ƫ������������ɸ�������ʽ��ʾ
		x+=size;//��ǵ���һ���ֵ�X��λ��
		p++;//��һ����
	}
}



