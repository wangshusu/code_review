#include "body.h"

Body::Body(int x0, int y0, int color, int height, int width)
{
    m_x = x0;
    m_y = y0;
    m_color = color;
    m_height = height;
    m_width = width;
}

void Body::move(int x, int y)
{
    m_x += x;
    m_y += y;
}

bool Body::operator==(const Body &other)
{
    if (m_x == other.m_x && m_y == other.m_y)
        return true;
    else
        return false;
}

bool Body::operator!=(const Food &other)
{
    if (m_x < other.getX() && m_x + m_width > other.getX() && m_y < other.getY() && m_y + m_height > other.getY())
        return false;
    else
        return true;
}

void Body::display(Lcd &l)
{
    int i, j;
    for (i = m_y; i < m_height + m_y; i++)
    {
        for (j = m_x; j < m_width + m_x; j++)
        {
            l.displayPoint(j, i, m_color);
        }
    }
}