#include "text.h"

static int Fd = -1;

//打开字库文件 打开失败返回-1
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

//关闭字库文件	不管关闭是否失败都会将Fd置位-1
void close_GBK_Font()
{
	if(close(Fd) == -1)
		printf("关闭GBK字库文件失败\n");
	else
		Fd = -1;
}

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
//size:字体大小
int Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
	int file; 
	unsigned char qh,ql;
	unsigned int i;
	unsigned long foffset;
	u16 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非常用汉字 全0填充
	{
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return 0;//结束访问
	}
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量

	file = lseek(Fd, foffset, SEEK_SET);
	if(file == -1) return -1;
	file = read(Fd, mat, csize);
	if(file == -1) return -1;

	return 0;
}
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode,int color)
{
	int file;
	u16 temp,t,t1;
	u16 y0=y;
	u8 dzk[288];
	u16 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	//if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)return;//不支持的size 支持12/16/20/24/28/32/36/40/44/48	在Show_Str函数中已经判断了
	//if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
	file = Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据
	if(file == -1)
	{
		printf("读取GBK字库字模失败\n");
		return; //没有读取到数据
	} 
	for(t=0;t<csize;t++)
	{
		temp=dzk[t];			//得到点阵数据
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
//在指定位置开始显示一个字符串
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小 （12/16/20/24/28/32/36/40/44/48）
//mode:0,非叠加方式;1,叠加方式
//color:字体颜色
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode,int color)
{
	u16 x0=x;
	u16 y0=y;
    u8 bHz=0;     //字符或者中文
	if(size%4 != 0 || (size/4) < 3 || (size/4) > 12)//支持12/16/20/24/28/32/36/40/44/48
	{
		printf("字库中没有该字号字模\n");
		return;//不支持的size 
	}

    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{
					y+=size;
					x=x0;
				}
		        if(y>(y0+height-size))break;//越界返回
		        if(*str==13)//换行符号
		        {
		            y+=size;
					x=x0;
		            str++;
		        }
		        else LCD_ShowChar(x,y,*str,size,mode,color);//有效部分写入
				str++;
		        x+=size/2; //字符,为全字的一半
	        }
        }else//中文
        {
			if(Fd == -1)//字库未打开才需要打开
				if(open_GBK_Font(size) == -1)//打开字库是否成功，打开失败会直接略过该字符
					printf("打开GBK字库失败\n");

            bHz=0;//有汉字库
            if(x>(x0+width-size))//换行
			{
				y+=size;
				x=x0;
			}
	        if(y>(y0+height-size))break;//越界返回
	        Show_Font(x,y,str,size,mode,color); //显示这个汉字,空心显示
	        str+=2;
	        x+=size;//下一个汉字偏移
        }
    }
	if(Fd != -1)
		close_GBK_Font();
}

//在指定宽度的中间显示字符串
//如果字符长度超过了len,则直接用Show_Str显示
//len:指定要显示的宽度
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

