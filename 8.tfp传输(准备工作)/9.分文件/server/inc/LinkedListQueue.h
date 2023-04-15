/*
 * @Author: your Name
 * @Date: 2022-07-28 14:58:45
 * @LastEditors: your Name
 * @LastEditTime: 2022-07-28 19:34:48
 * @Description: 
 */
#ifndef __LINKEDLISTQUEUE_H__
#define __LINKEDLISTQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void * MyStackType;

#define QueueMaxnum 50	/* 队列的最大容量 */

/* 队列数据节点结构体 */
typedef struct Node
{
	MyStackType data;	/* 数据保存在这里 */
	struct Node * next;	/* 指向下一个数据节点 */
}Node;

/*队列头节点结构体  */
typedef struct Queue
{
	struct Node * rear;		/* 尾节点 */
	struct Node * front;	/* 头结点 */
	int num;
}Queue;


/*
	queueInit:队列初始化函数
		@返回值:一个队列头结点
*/
Queue *queue_Init(void);

/*
	queueDestroy:销毁队列 传二级指针进来 改变其指向
		@(*callback):用户指定的处理规则 回调函数
					 可以指定NULL 就没有处理
		@MyStackType:默认(void*) .h头文件可修改
*/					
void queue_Destroy(Queue **qu,void (*callback)(MyStackType));

/*
	queueClear:清空队列
		@(*callback):用户指定的处理规则 回调函数
					 可以指定NULL 默认-->free(data)
		@MyStackType:默认(void*) .h头文件可修改
*/		
void queue_Clear(Queue *qu,void (*callback)(MyStackType));

/*
	queueIn:入队
		@qu	:队列头
		@data:数据(MyStackType)类型
		@返回值:添加成功返回0 , 失败返回-1
*/		
int queue_In(Queue *qu,MyStackType data);

/*
	queueOut:队列出队
		@qu	:队列头
		@返回值:没有队列返回 NULL
			    否则返回出队元素的 (MyStackType)data
*/	
MyStackType queue_Out(Queue *qu);


/*
	queueIsEmpty:判断这个队列是否是空的
		@qu	:队列头
		@返回值:为空返回true 否则返回false
*/		
bool queue_IsEmpty(Queue *qu);

/*
	queueLength:求队列里元素个数
		@qu	:队列头
		@返回值:队列存在返回元素个数 
			   否则返回0
*/	
int queue_Length(Queue *qu);

/*
	queueGetTop:返回队头元素(不出队)
		@qu	:队列头
		@返回值:返回队头元素(MyStackType类型)
				没有队列返回 NULL
*/	
MyStackType queue_GetTop(Queue *qu);

/* 
	queuePrintHeadList:队列检测函数
*/
void queue_PrintHeadList(Queue *qu);

#endif
