#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include "lcd.h"
Lcd Lcd::l{LCDNAME, 480, 800};
Lcd::Lcd(const char *pathname, int height, int width)
{
	m_height = 480;
	m_width = 800;
	m_fd = open(pathname, O_RDWR);
	if (m_fd == -1)
	{
		perror("open failed");
		return;
	}

	m_p = (int *)mmap(NULL, m_height * m_width * 4, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
	if (m_p == MAP_FAILED)
	{
		std::cout << "mmap failed" << std::endl;
	}
	return;
}
//关闭
Lcd::~Lcd()
{
	//解映射
	munmap(m_p, m_height * m_width * 4);
	//关闭
	close(m_fd);
}

void Lcd::clear(int color)
{
	int i, j;
	for (j = 0; j < m_height; j++) //每一行
	{
		for (i = 0; i < m_width; i++)
		{
			displayPoint(i, j, color);
		}
	}
}

void Lcd::displayPoint(int x, int y, int color)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		*(m_p + y * m_width + x) = color;
	}
}

void Lcd::displayFood(const Food &c)
{
	int r = c.getR();
	int x = c.getX();
	int y = c.getY();
	int color = c.getColor();
	int i, j;
	for (i = y - r; i < y + r; i++)
	{
		for (j = x - r; j < x + r; j++)
		{
			if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r)
				displayPoint(j, i, color);
		}
	}
}

Lcd &Lcd::getLcd()
{
	return l;
}