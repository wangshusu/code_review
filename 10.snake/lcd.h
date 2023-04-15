#ifndef __LCD_H__
#define __LCD_H__
#include"food.h"
#define LCDNAME "/dev/fb0"
class Lcd
{
public:
	~Lcd();
	void displayPoint(int x,int y,int color);
	void clear(int color);
	void displayFood(const Food& c);
	Lcd(const Lcd&) = delete;
	Lcd& operator=(const Lcd&) = delete;
	static Lcd &getLcd();
private:
	Lcd(const char * pathname,int height,int width);
	int m_width;
	int m_height;
	int m_fd;
	int * m_p;
	static Lcd l;
};


#endif
