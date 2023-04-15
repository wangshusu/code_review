#include <stdlib.h>
#include <time.h>
#include "food.h"

Food::Food()
{
	m_x = rand() % 80 * 10 + 5;
	m_y = rand() % 48 * 10 + 5;
	m_color = rand() % 0xffffff;
	m_r = 5;
}

Food::Food(int x, int y, int r)
{
	m_x = x;
	m_y = y;
	m_color = rand() % 0xffffff;
	m_r = r;
}

void Food::setR(int r)
{
	m_r = r;
}

int Food::getR() const
{
	return m_r;
}

void Food::setX(int x)
{
	m_x = x;
}

int Food::getX() const
{
	return m_x;
}

void Food::setY(int y)
{
	m_y = y;
}

int Food::getY() const
{
	return m_y;
}

void Food::setColor(int color)
{
	m_color = color;
}
int Food::getColor() const
{
	return m_color;
}
