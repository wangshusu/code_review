#include "LinkedListStack.h"


/********************* 下面是栈的函数 ********************/

/*
    stackInit:初始化函数 返回一个栈
        @返回值:(Stack *)栈头
*/
Stack *stack_Init(void)
{
    Stack *pnew = (Stack *)calloc(1,sizeof(pnew));
    if(!pnew)
        return NULL;
    pnew->top = NULL;
    pnew->num = 0;
    return pnew;
}

/*
    stack_In:入栈
        @st:栈头
        @data:数据类型-->MyStackType
*/
void stack_In(Stack *st, MyStackType data)
{
    if(!st)
    {
        return;
    }
    if(StackMax < st->num) /* 判断是否超过了最大的 */
    {
        printf("Too Much!!\n");
        return;
    }

    /* 先用东西来接住这个值 */
    StackNode *pnew = (StackNode *)calloc(1,sizeof(pnew));
    pnew->data = data;
    pnew->next = NULL;

    /* 如果里面没有数据节点 */
    if(0 == st->num)
    {   
        st->top = pnew;    /* 那么这个就是第一个节点 */
        st->num++;         /* 添加了节点 这里的数字就要加一 */
        return;
    }

    /* 如果不是第一个  那么这里就是用头插法 */
    pnew->next = st->top;
    st->top = pnew;        /* 更改头结点的指向 */
    st->num++;             /* 添加了节点 这里的数字就要加一 */
    return;
}

/*
    stack_Out:出栈,出栈有返回值,返回出栈元素存储的data
        @st:栈头
        @返回值:失败返回 0  成功返回data--->(MyStackType)
*/
MyStackType stack_Out(Stack *st)
{
    if(!st )    /* 栈不存在 */
    {
        return (MyStackType)0;
    }
    if(0 == st->num)   /* 栈里无数据节点 */
    {
        printf("The Stack was NULL!\n");
        return (MyStackType)0;
    }

    /* 先把这个值保存起来 */
    MyStackType data1 = st->top->data;
    StackNode *ptr = st->top;

    /* 如果是最后一个还要出队 */
    if(0 == st->num)
    {
        st->top = NULL;
        st->num--;
        free(ptr);
        return data1;
    }
    else    /* 如果不是最后一个 */
    {
        st->top = st->top->next;
        ptr->next = NULL;
        free(ptr);
        st->num--;
        return data1;
    }
    
}

/*
    stack_Destory:销毁函数,调用清空函数
        @st:栈头
        @void (*callback)(MyStackType):传入参数可以对清空的数据进行处理
         可以传入NULL 默认对数据进行free操作
*/
void stack_Destory(Stack **st, void (*callback)(MyStackType ))
{
    /* 销毁传的是二级指针 这里要改变他的指向 */
    stack_Clear(*st, callback);
    free(*st);
    *st = NULL;
    printf("DestoryStack Success!!!\n");
}

/*
    stack_Clear:清除栈内的空间,释放 st 在堆区开辟的空间
        @st:栈头
        @void (*callback)(MyStackType):传入参数可以对清空的数据进行处理
         可以传入NULL 默认对数据进行free操作
*/
void stack_Clear(Stack *st, void (*callback)(MyStackType ))
{
    if(!st)
    {
        printf("The st is NULL!\n");
        return;
    }
        
    while(st->num)  /* 直到里面没有数据为止 */
    {
        MyStackType data = stack_Out(st);

        if(callback)
        {
            callback(data);
        }
        else
        {
            free(data);
        }
    }
}

/*
    stack_Length:返回栈的长度
        @返回值:(int)(return st->length)
*/
int stack_Length(Stack *st)
{
    return st->num;
}


/*
    stack_Empty:判断栈是否为空
        @返回值:为空:返回 true
               不空:返回 false
*/
bool stack_Empty(Stack *st)
{
    if(st->num != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
