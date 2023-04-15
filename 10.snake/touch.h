#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "snake.h"

class Touch
{
public:
    Touch();
    ~Touch();

    // void getdir(Snake &s);
    void getInputData();
    enum direction getDir() const;

private:
    int m_fd;
    int m_x;
    int m_y;
    enum direction m_dir; //滑动方向
};

#endif