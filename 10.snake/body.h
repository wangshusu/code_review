#ifndef __BODY_H__
#define __BODY_H__
//#include"point.h"
#include"food.h"
#include "lcd.h"
/*
    屏幕是480*800，以10*10的矩形为单位
    那么矩形个数就是 48*800
*/

class Body//身体类
{
friend class Snake;
public:
    Body(int x0,int y0,int color,int height = 10,int width = 10);
    void move(int x,int y);
    bool operator==(const Body &other);
    bool operator!=(const Food &other);
    void display( Lcd &l);
private:
    int m_x;
    int m_y;
    int m_color;
    int m_height;
    int m_width;
};

#endif