#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* 任务队列结构体 */
typedef struct Task Task;
/* 线程池结构体 */
typedef struct Threadpool Threadpool;

/* 单个线程的退出 */
void *thread_Exit(Threadpool *pool);

/* 管理者线程 */
void *manager_Function(void *arg);

/* 工作者线程 */
void *worker_Function(void *arg);

/* 创建并且初始化一个线程池 */
Threadpool *creat_Pthread(int queueCapacity, int maxNum, int minNum);

/* 添加任务函数 */
void thread_Add(Threadpool *pool, void (*func)(void*), void *arg);

/* 线程池销毁函数 */
int thread_Destroy(Threadpool *pool);

#endif
