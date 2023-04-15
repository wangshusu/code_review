#include <iostream>
#include <unistd.h>
#include <time.h>
#include "touch.h"
#include <thread>
#include "lcd.h"
#include "snake.h"
#include "food.h"
//注意：如果在开发板运行时提示C++标准库不兼容，可以在编译时加上该选项  -static-libstdc++
using std::thread;
int main()
{
    srand(time(NULL));      //生成随机数种子
    Lcd &l = Lcd::getLcd(); //获取Lcd唯一实例的引用
    l.clear(0);
    Snake s;
    Food food;
    s.displaySnake(l);
    l.displayFood(food);
    bool b = false;
    Touch t;
    thread th{&Touch::getInputData, &t};
    // t.getdir(s);
    while (1)
    {
        switch (t.getDir())
        {
        case UP:
            if (s.getDir() != DOWN)
            {
                s.setDir(UP);
                b = s.up(&food, l);
            }
            break;
        case DOWN:
            if (s.getDir() != UP)
            {
                s.setDir(DOWN);
                b = s.down(&food, l);
            }
            break;
        case LEFT:
            if (s.getDir() != RIGHT)
            {
                s.setDir(LEFT);
                b = s.left(&food, l);
            }
            break;
        case RIGHT:
            if (s.getDir() != LEFT)
            {
                s.setDir(RIGHT);
                b = s.right(&food, l);
            }
            break;
        default:
            break;
        }
        if (s.isover())
        {
            //显示 gameover
            break;
        }
        if (b) //吃到食物
        {
            l.displayFood(food);
        }

        usleep(100000);
    }

    return 0;
}