#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "snake.h"

Snake::Snake()
{
    //随机生成一个蛇头矩形
    snake_head.emplace_back(rand() % 80 * 10, rand() % 48 * 10, 0xff0000, 10, 10);
    dir = UP; //游戏开始时的默认前进方向
}

//蛇向上:把最后一个节点删除，在蛇头上面添加一个节点
//不能先删除在添加，应该先添加再删除
bool Snake::up(Food *const f, Lcd &l)
{
    Body head = snake_head.front(); //获取当前蛇头节点
    head.m_y -= 10;
    if (head.m_y < 0)
        head.m_y = 470; //穿墙
    // snake_head.emplace_front(head);
    snake_head.push_front(head);
    head.display(l);
    if (head != *f) //如果当前蛇头位置和食物位置不重合，没有吃到食物，就删除最后一个节点
    {
        Body tail = snake_head.back();
        tail.m_color = 0; //把颜色变为背景色
        tail.display(l);

        snake_head.pop_back(); //删除最后一个节点
        return false;
    }
    else
    {
        *f = Food{}; // f指向新的食物对象
        return true;
    }
}

//蛇向下:把最后一个节点删除，在蛇头下面添加一个节点
bool Snake::down(Food *const f, Lcd &l)
{
    Body head = snake_head.front(); //获取当前蛇头节点
    head.m_y += 10;
    if (head.m_y >= 480)
        head.m_y = 0; //穿墙
    // snake_head.emplace_front(head);
    snake_head.push_front(head);
    head.display(l);
    if (head != *f) //如果当前蛇头位置和食物位置不重合，没有吃到食物，就删除最后一个节点
    {
        Body tail = snake_head.back();
        tail.m_color = 0; //把颜色变为背景色
        tail.display(l);

        snake_head.pop_back(); //删除最后一个节点
        return false;
    }
    else
    {
        *f = Food{};
        return true;
    }
}

//蛇向左:把最后一个节点删除，在蛇头左边添加一个节点
bool Snake::left(Food *const f, Lcd &l)
{
    Body head = snake_head.front(); //获取当前蛇头节点
    head.m_x -= 10;
    if (head.m_x < 0)
        head.m_x = 790; //穿墙
    // snake_head.emplace_front(head);
    snake_head.push_front(head);
    head.display(l);
    if (head != *f) //如果当前蛇头位置和食物位置不重合，没有吃到食物，就删除最后一个节点
    {
        Body tail = snake_head.back();
        tail.m_color = 0; //把颜色变为背景色
        tail.display(l);

        snake_head.pop_back(); //删除最后一个节点
        return false;
    }

    *f = Food{};
    return true;
}

//蛇向右:把最后一个节点删除，在蛇头右边添加一个节点
bool Snake::right(Food *const f, Lcd &l)
{
    Body head = snake_head.front(); //获取当前蛇头节点
    head.m_x += 10;
    if (head.m_x >= 800)
        head.m_x = 0; //穿墙
    // snake_head.emplace_front(head);
    snake_head.push_front(head);
    head.display(l);
    if (head != *f) //如果当前蛇头位置和食物位置不重合，没有吃到食物，就删除最后一个节点
    {
        Body tail = snake_head.back();
        tail.m_color = 0; //把颜色变为背景色
        tail.display(l);

        snake_head.pop_back(); //删除最后一个节点
        return false;
    }

    *f = Food{};
    return true;
}

bool Snake::isover()
{
    //判断蛇头是否和身体相撞
    Body head = snake_head.front();                              //获取当前蛇头节点
    int cou = count(snake_head.begin(), snake_head.end(), head); //获取蛇头位置的元素个数，如果>1 ：蛇头与身体相撞了
    if (cou > 1)
        return true;
    else
        return false;

    //判断是否越界
}

void Snake::displaySnake(Lcd &l)
{
    for (list<Body>::iterator it = snake_head.begin(); it != snake_head.end(); it++)
    {
        it->display(l);
    }
}

void Snake::setDir(enum direction d)
{
    dir = d;
}

enum direction Snake::getDir() const
{
    return dir;
}