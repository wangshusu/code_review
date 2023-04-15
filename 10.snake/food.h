#ifndef __FOOD_H__
#define __FOOD_H__
//#include "point.h"

class Food
{
public:
	Food(int x,int y,int r = 5);
	Food();

	void setR(int r);
	int getR()  const;
	void setX(int x);
	int getX()  const;
	void setY(int y);
	int getY()  const;

	void setColor(int color);
	int getColor()  const;
	
private:
	//属性
	int m_x;
    int m_y;
	int m_r;//半径
	int m_color;//圆的颜色
};



#endif
