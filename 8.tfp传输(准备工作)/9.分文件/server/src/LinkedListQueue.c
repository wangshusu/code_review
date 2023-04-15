#include "LinkedListQueue.h"


/*
	queueInit:队列初始化函数
		@返回值:一个队列头结点
*/
Queue *queue_Init(void)
{
	Queue * qu = malloc(sizeof(Queue));
	qu->num = 0;
	qu->rear = qu->front = NULL;
	return qu;
}

/*
	queueDestroy:销毁队列 传二级指针进来 改变其指向
		@(*callback):用户指定的处理规则 回调函数
					 可以指定NULL 就没有处理
		@MyStackType:默认(void*) .h头文件可修改
*/					
void queue_Destroy(Queue **qu,void (*callback)(MyStackType))
{

	queue_Clear(*qu,callback);	/* 清空这个队列 */
	
	//free(*qu);	/* 将这个队列释放 */
	
	//*qu = NULL; /* 改变二级指针指向 */
	//printf("qu was destroy!\n");
}

/*
	queueClear:清空队列
		@(*callback):用户指定的处理规则 回调函数
					 可以指定NULL 默认-->free(data)
		@MyStackType:默认(void*) .h头文件可修改
*/		
void queue_Clear(Queue *qu,void (*callback)(MyStackType))
{
	while(!queue_IsEmpty(qu))	/* 不为空才会需要删除 */
	{
		void *data = (void *)queue_Out(qu);

		if(callback)	/* 传入函数指针才会处理 */
		{
			callback(data);
		}
		else			/* 默认直接free(data) */
		{
			free(data);
		}
	}	
}

/*
	queueIsEmpty:判断这个队列是否是空的
		@qu	:队列头
		@返回值:为空返回true 否则返回false
*/		
bool queue_IsEmpty(Queue *qu)
{
	if(!qu || qu-> num == 0)
		return true;
	else
		return false;
}

/*
	queueLength:求队列里元素个数
		@qu	:队列头
		@返回值:队列存在返回元素个数 
			   否则返回0
*/	
int queue_Length(Queue *qu)
{
	if(!qu)
		return 0;
	else
		return qu->num;	/* 返回队列元素个数 */
}

/*
	queueGetTop:返回队头元素(不出队)
		@qu	:队列头
		@返回值:返回队头元素(MyStackType类型)
				没有队列返回 NULL
*/	
MyStackType queue_GetTop(Queue *qu)
{
	if(!qu)
		return NULL;
	else
		return (MyStackType)qu->front ->data;	/* 返回值为(MyStackType)类型 */
}

/*
	queueOut:队列出队
		@qu	:队列头
		@返回值:没有队列返回 NULL
			    否则返回出队元素的 (MyStackType)data
*/	
MyStackType queue_Out(Queue *qu)
{
	if(!qu || !qu->front)	/* 如果没有数据 直接返回 NULL */
	{
		return NULL;
	}
		
	MyStackType data = (MyStackType)qu->front ->data;	/* 先取出数据 */

	if(qu->num == 1)	/* 如果队列里面只存在一个数据 */
	{
		free(qu->front);	/* 释放最后一个节点 */

		qu->front = qu->rear = NULL;	/* 重置队列头标志位 */
		qu->num--;	/* 头结点num数量减少 */
		free(qu);	/* 将头结点释放 */
		qu = NULL;	/* 头结点指向空 */

		return data;
	}
	else	/* 如果队列里面元素还有多个 */
	{
		/* 进行节点的删除 与 连接 */
		Node* ptr = qu->front; 
		qu->front = qu->front ->next;
		ptr ->next = NULL;
		//qu->rear ->next = qu->front;	/* 单项循环链表 */

		free(ptr);	/* 释放当前节点的空间 */
		qu->num--;
		
		return data;
	}
	
}

/*
	queueIn:入队
		@qu	:队列头
		@data:数据(MyStackType)类型
		@返回值:添加成功返回0 , 失败返回-1
*/		
int queue_In(Queue *qu,MyStackType data)	
{
	if(!qu)	/* 队列头不存在 */
	{
		return -1;
	}
	if(QueueMaxnum ==  queue_Length(qu))	/* 数量已经打到设置的上限 */
	{
		printf("\n栈里面的元素太多了,不能加了\n");
		return -1;
	}
	
	/* 创建一个新的节点 */
	Node *pnew = (Node *)calloc(1,sizeof(Node));
	pnew->data = data;
	pnew->next = NULL;
	
	/* 如果队列本身没有节点 */
	if(qu->num == 0)
	{
		qu->front = qu->rear = pnew;
		//pnew->next = pnew;	/* 自己指向自己 单项循环链表 */
		qu->num++;
		return 0;
	}

	/* 如果队列本身存在 尾插 */
	//pnew ->next = qu->front; /* 单项循环链表 */
	qu->rear ->next = pnew;
	qu->rear = pnew;
	qu->num++;
	return 0;
}	

/* 
	queuePrintHeadList:队列检测函数
*/
void queue_PrintHeadList(Queue *qu)
{
	if(!qu)
	{
		printf("link inexistence !!!\n\n");
		return;
	}

	Node *ptr = qu->front;

	for(int i = 0 ; i < qu->num ; i ++)
	{
		printf("%d ",*((int *)ptr ->data));	
		ptr = ptr ->next;
	}

	printf("\n************\ntop  = ");
	printf("%d",*((int *)qu->front ->data));	
	printf("\n");

	printf("rear = ");
	printf("%d",*((int *)qu->rear ->data));	
	printf("\n");
	
	printf("num  = ");
	printf("%d",qu->num);	
	printf("\n************\n");
	
	printf("\n");
}







