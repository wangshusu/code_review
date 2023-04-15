#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define StackMax 50

typedef void * MyStackType;

/* 栈 的头结点 */
typedef struct StackNode
{

    MyStackType data;
    struct StackNode *next;

}StackNode;

/* 栈 的数据节点 */
typedef struct Stack
{

    int num;    /* 栈里面的数量 */
    struct StackNode *top;     /* 指向栈的顶部 */

}Stack;

/*
    stackInit:初始化函数 返回一个栈
        @返回值:(Stack *)栈头
*/
Stack *stack_Init(void);

/*
    stack_In:入栈
        @st:栈头
        @data:数据类型-->MyStackType
*/
void stack_In(Stack *st, MyStackType data);

/*
    stack_Out:出栈,出栈有返回值,返回出栈元素存储的data
        @st:栈头
        @返回值:失败返回 0  成功返回data--->(MyStackType)
*/
MyStackType stack_Out(Stack *st);

/*
    stack_Destory:销毁函数,调用清空函数
        @st:栈头
        @void (*callback)(MyStackType):传入参数可以对清空的数据进行处理
         可以传入NULL 默认对数据进行free操作
*/
void stack_Destory(Stack **st, void (*callback)(MyStackType ));

/*
    stack_Clear:清除栈内的空间,释放 st 在堆区开辟的空间
        @st:栈头
        @void (*callback)(MyStackType):传入参数可以对清空的数据进行处理
         可以传入NULL 默认对数据进行free操作
*/
void stack_Clear(Stack *st, void (*callback)(MyStackType ));

/*
    stack_Length:返回栈的长度
        @返回值:(int)(return st->length)
*/
int stack_Length(Stack *st);

/*
    stack_Empty:判断栈是否为空
        @返回值:为空:返回 true
               不空:返回 false
*/
bool stack_Empty(Stack *st);


#endif