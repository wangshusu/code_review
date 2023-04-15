#ifndef __SNAKE_H__
#define __SNAKE_H__
#include <list>
#include"Body.h"
#include"lcd.h"
#include"Food.h"
using std::list;

enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
class Snake
{
public:
    Snake();
    bool up(Food * const f,Lcd &l);
    bool down(Food * const f,Lcd &l);
    bool left(Food * const f,Lcd &l);
    bool right(Food * const f,Lcd &l);
    bool isover();
    void displaySnake(Lcd &l);
    void setDir(enum direction );
    enum direction getDir() const;
private:
    list<Body> snake_head;
    enum direction dir;
};



#endif