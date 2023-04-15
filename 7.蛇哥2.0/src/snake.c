#include "snake.h"

/************* 全局变量区 **************/
int arr_x[X_Max];//每个方块的x位置 右上角
int arr_y[Y_Max];//每个方块的y位置 右上角
int snake_x[COL];//35
int snake_y[ROW];//24
int face[COL][ROW] = {0};
int WallColor = 0xffff00;
int FoodColor = 0xd15fee;
int background = 0x000000;
int SnakeHeadColor = 0xffff00;
int SnakeBodyColor = 0xaeeeee;



int grade = 0;//分数
int max = 100;//最高分数

//初始化屏幕 并将位置存入数组
void Init_Screen(void)
{
    memset(face,0,sizeof(face));
    
    //1.游戏背景为黑色
    lcd_clear_screen(background);
    //2.围墙为红色
    //<1.先区域分隔存入数组>
    for(int i = 0 ; i < X_Max ; i ++)
    {
        arr_x[i] += width * i;
    }
    for(int i = 0 ; i < Y_Max ; i ++)
    {
        arr_y[i] += height * i;
    }
    
    for(int i = 0 ; i < X_Max - 5 ; i ++)//画墙
    {
        lcd_draw_rectangle(arr_x[i],arr_y[0],width - 2,height - 2,WallColor);
        lcd_draw_rectangle(arr_x[i],arr_y[Y_Max - 1],width - 2,height - 2,WallColor);
    }
    for(int i = 0 ; i < Y_Max ; i ++)//画墙
    {
        lcd_draw_rectangle(arr_x[0],arr_y[i],width - 2,height - 2,WallColor);
        lcd_draw_rectangle(arr_x[X_Max - 6],arr_y[i],width - 2,height - 2,WallColor);
    }

    //lcd_draw_rectangle(arr_x[1],arr_y[1],width - 2,height - 2,0xffffff);
    //lcd_draw_rectangle(arr_x[33],arr_y[22],width - 2,height - 2,0xffffff);

    
    for(int i = 0 ; i < COL ; i ++)//这里将蛇哥的移动空间存入 蛇哥的点 snake_x  snake_y
    {
        snake_x[i] = arr_x[i];
		face[i][0] = WALL;
		face[i][ROW - 1] = WALL;
    }
    for(int i = 0 ; i < ROW ; i ++)
    {
        snake_y[i] = arr_y[i];
		face[COL - 1][i] = WALL;
		face[0][i] = WALL;
    }
    
    // lcd_draw_rectangle(snake_x[0],snake_y[0],width - 2,height - 2,0xffffff);
    // lcd_draw_rectangle(snake_x[COL - 1],snake_y[ROW - 1],width - 2,height - 2,0xffffff);

    printf("arr %d %d\n",arr_x[1],arr_y[1]);
    printf("snake_ %d %d\n",snake_x[0],snake_y[0]);
}

//初始化蛇
//蛇的长度 为 2
void Init_Snake()
{
    snake.len = 2; //蛇的身体长度初始化为2
	snake.x = COL / 2; //蛇头位置的横坐标
	snake.y = ROW / 2; //蛇头位置的纵坐标
	//蛇身坐标的初始化
	body[0].x = COL / 2 - 1;
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	//将蛇头和蛇身位置进行标记
	face[snake.x][snake.y] = HEAD;
	face[body[0].x][body[0].y] = BODY;
	face[body[1].x][body[1].y] = BODY;

    printf("asdf %d\n",snake.x);
}

//随机生成食物
void RandFood()
{
	int i, j;
	do
	{
		//随机生成食物的横纵坐标
		i = rand() % COL;
		j = rand() % ROW;
	} while (face[i][j] != KONG); //确保生成食物的位置为空，若不为空则重新生成
	face[i][j] = FOOD; //将食物位置进行标记

	lcd_draw_rectangle(snake_x[i],snake_y[j],width - 5,height - 5,FoodColor);
}

//打印蛇与覆盖蛇
//传入 1 打印蛇
//传入 0 覆盖蛇
void DrawSnake(int flag)
{

    if (flag == 1) //打印蛇
	{
		lcd_draw_rectangle(snake_x[snake.x],snake_y[snake.y],width - 2,height - 2,SnakeHeadColor);//蛇头
		for (int i = 0; i < snake.len; i++)
		{
            lcd_draw_rectangle(snake_x[body[i].x],snake_y[body[i].y],width - 2,height - 2,SnakeBodyColor);
		}
	}
	else //覆盖蛇
	{
		if (body[snake.len - 1].x != 0) //防止len++后将(0, 0)位置的墙覆盖
		{
			//将蛇尾覆盖为空格即可
			lcd_draw_rectangle(snake_x[body[snake.len - 1].x], snake_y[body[snake.len - 1].y],width - 2,height - 2,background);
			//printf("尾部覆盖结束!\n");
		}
	}
    
}

//移动蛇
void MoveSnake(int x, int y)
{
	//printf("开始移动蛇!\n");
	DrawSnake(0); //先覆盖当前所显示的蛇
	face[body[snake.len - 1].x][body[snake.len - 1].y] = KONG; //蛇移动后蛇尾重新标记为空
	face[snake.x][snake.y] = BODY; //蛇移动后蛇头的位置变为蛇身
	//蛇移动后各个蛇身位置坐标需要更新
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//蛇 移动后蛇头位置信息 变为第0个蛇身的位置信息
	body[0].x = snake.x;
	body[0].y = snake.y;
	//蛇头的位置更改
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	//face[snake.x][snake.y] = HEAD;
	DrawSnake(1); //打印移动后的蛇
}

//判断得分与结束
//游戏结束返回 0
//游戏正常返回 1
int JudgeFunc(int x, int y)
{
	//printf("开始判断!\n");
	//若蛇头即将到达的位置是食物，则得分
	if (face[snake.x + x][snake.y + y] == FOOD)
	{
		snake.len++; //蛇身加长
		grade += 10; //更新当前得分
		printf("\n\n\n\n当前得分:%d\n\n\n", grade); //重新打印当前得分
		RandFood(); //重新随机生成食物
	}
	//若蛇头即将到达的位置是墙或者蛇身，则游戏结束
	else if (face[snake.x + x][snake.y + y] == WALL || face[snake.x + x][snake.y + y] == BODY)
	{
		//Sleep(1000); //留给玩家反应时间

		if (grade > max)
		{
			printf("恭喜你打破最高记录，最高记录更新为%d", grade);
		}
		else if (grade == max)
		{
			printf("与最高记录持平，加油再创佳绩", grade);
		}
		else
		{
			printf("请继续加油，当前与最高记录相差%d", max - grade);
		}
		
		//打印游戏失败
		printf("GAME OVER\n");
		return 0;

	}

	return 1;
}


void Print(void)
{
	for(int i = 0 ; i < ROW ; i ++)
	{
		for(int j = 0 ; j < COL ; j ++)
		{
			if(face[j][i] == 0)
				printf(". ");
			else
				printf("%d ",face[j][i]);
		}
		printf("\n");
	}
	printf("\n");

	for(int i = 0 ; i < COL ; i ++)
	{
		for(int j = 0 ; j < ROW ; j ++)
		{
			if(face[i][j] == 2)
				printf("(%d,%d)\n",i,j);
	
		}
	}


	
}


//是对整个屏幕进行划分区域
//所以 屏幕大小 存储为 40 * 20
//但是蛇的 活动区域 只有 (1,1)~(33,12)   这里是 <= 的