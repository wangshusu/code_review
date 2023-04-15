#include <stdio.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "touch.h"

Touch::Touch()
{
	m_fd = open("/dev/input/event0", O_RDONLY);
	if (m_fd == -1)
		perror("open error");
}
Touch::~Touch()
{
	close(m_fd);
}

void Touch::getInputData()
{
	struct input_event ev;
	int r;
	int x1 = -1, y1 = -1, x2, y2;
	while (1)
	{
		while (1)
		{
			r = read(m_fd, &ev, sizeof(ev));
			if (r != sizeof(ev))
			{
				continue;
			}
			if (ev.type == EV_ABS && ev.code == 0) // x轴事件
			{
				if (x1 == -1)
				{
					x1 = ev.value;
				}
				x2 = ev.value;
			}
			if (ev.type == EV_ABS && ev.code == 1) // y轴事件
			{
				if (y1 == -1)
				{
					y1 = ev.value;
				}
				y2 = ev.value;
			}
			if (ev.type == EV_KEY && ev.code == 0x14a && ev.value == 0)
			{
				//此次滑动结束
				m_x = x2;
				m_y = y2;
				break;
			}
		}
		//进行方向判断
		if (abs(y2 - y1) > 2 * abs(x2 - x1)) //上下
		{
			if (y2 < y1) //上
			{
				m_dir = UP;
			}
			else //下
			{
				m_dir = DOWN;
			}
		}
		else if (abs(x2 - x1) > 2 * abs(y2 - y1)) //左右
		{
			if (x2 < x1) //左
			{
				m_dir = LEFT;
			}
			else
			{
				m_dir = RIGHT;
			}
		}
		x1 = -1;
		y1 = -1;
	}
}

enum direction Touch::getDir() const
{
	return m_dir;
}