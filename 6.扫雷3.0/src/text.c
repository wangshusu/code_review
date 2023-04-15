#include "text.h"

static int Fd = -1;

//���ֿ��ļ� ��ʧ�ܷ���-1
int open_GBK_Font(u8 size)
{
	char path[64];
	strcpy(path,GBK_FON_PATH);
	switch (size)
	{
	case 12:
		strcat(path,GBK12FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 16:
		strcat(path,GBK16FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 20:
		strcat(path,GBK20FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 24:
		strcat(path,GBK24FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 28:
		strcat(path,GBK28FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 32:
		strcat(path,GBK32FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 36:
		strcat(path,GBK36FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 40:
		strcat(path,GBK40FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 44:
		strcat(path,GBK44FON_file);
		Fd = open(path, O_RDONLY);
		break;
	case 48:
		strcat(path,GBK48FON_file);
		Fd = open(path, O_RDONLY);
		break;
	default:
		return -1;
	}

	if(Fd == -1)
		return -1;
	else
		return 0;
}

//�ر��ֿ��ļ�	���ܹر��Ƿ�ʧ�ܶ��ὫFd��λ-1
void close_GBK_Font()
{
	if(close(Fd) == -1)
		printf("�ر�GBK�ֿ��ļ�ʧ��\n");
	else
		Fd = -1;
}

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С
//size:�����С
int Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
	int file; 
	unsigned char qh,ql;
	unsigned int i;
	unsigned long foffset;
	u16 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�ǳ��ú��� ȫ0���
	{
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return 0;//��������
	}
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����

	file = lseek(Fd, foffset, SEEK_SET);
	if(file == -1) return -1;
	file = read(Fd, mat, csize);
	if(file == -1) return -1;

	return 0;
}
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode,int color)
{
	int file;
	u16 temp,t,t1;
	u16 y0=y;
	u8 dzk[288];
	u16 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	//if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)return;//��֧�ֵ�size ֧��12/16/20/24/28/32/36/40/44/48	��Show_Str�������Ѿ��ж���
	//if(size!=12&&size!=16&&size!=24&&size!=32)return;	//��֧�ֵ�size
	file = Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ�������
	if(file == -1)
	{
		printf("��ȡGBK�ֿ���ģʧ��\n");
		return; //û�ж�ȡ������
	} 
	for(t=0;t<csize;t++)
	{
		temp=dzk[t];			//�õ���������
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){lcd_draw_point(x,y,color);}
			else if(mode==0){lcd_draw_point(x,y,color);} 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
	}
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С ��12/16/20/24/28/32/36/40/44/48��
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ
//color:������ɫ
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int color)
{
	u16 x0=x;
	u16 y0=y;
    u8 bHz=0;     //�ַ���������
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)//֧��12/16/20/24/28/32/36/40/44/48
	{
		printf("�ֿ���û�и��ֺ���ģ\n");
		return;//��֧�ֵ�size 
	}

    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//����
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{
					y+=size;
					x=x0;
				}
		        if(y>(y0+height-size))break;//Խ�緵��
		        if(*str==13)//���з���
		        {
		            y+=size;
					x=x0;
		            str++;
		        }
		        else LCD_ShowChar(x,y,*str,size,mode,color);//��Ч����д��
				str++;
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ��
	        }
        }else//����
        {
			if(Fd == -1)//�ֿ�δ�򿪲���Ҫ��
				if(open_GBK_Font(size) == -1)//���ֿ��Ƿ�ɹ�����ʧ�ܻ�ֱ���Թ����ַ�
					printf("��GBK�ֿ�ʧ��\n");

            bHz=0;//�к��ֿ�
            if(x>(x0+width-size))//����
			{
				y+=size;
				x=x0;
			}
	        if(y>(y0+height-size))break;//Խ�緵��
	        Show_Font(x,y,str,size,mode,color); //��ʾ�������,������ʾ
	        str+=2;
	        x+=size;//��һ������ƫ��
        }
    }
	if(Fd != -1)
		close_GBK_Font();
}

//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,��ֱ����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u16 len,int color)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,800,480,str,size,1,color);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,800,480,str,size,1,color);
	}
}

