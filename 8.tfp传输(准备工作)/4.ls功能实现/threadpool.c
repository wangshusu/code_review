#include "threadpool.h"

#define NUMBER 1

/* 结构体不希望体现在.h文件中 */

/* 任务队列结构体 */
struct Task
{
    void (*func)(void *);
    void *arg;
};

/* 线程池结构体 */
struct Threadpool
{
    /* 任务队列 */
    Task *taskQ;
    int queueCapacity;  /* 等待队列的容量 */ 
    int queueSize;      /* 等待队列中任务的个数 */
    int queueFront;     /* 等待队列的第一个 */
    int queueRear;      /* 等待队列的最后一个 */

    /* 线程 */
    pthread_t managerID;    /* 管理者线程的ID */
    pthread_t* workersID;    /* 工作者的线程ID 这里需要指针 因为要根据初始化才知道有多大*/
    int maxNum;     /* 线程的最多个数 */
    int minNum;     /* 线程的最小个数 */
    int liveNum;    /* 线程的存活个数 */
    int busyNum;    /* 忙的线程个数  在执行任务的 */
    int exitNum;    /* 要退出的线程数 */
    pthread_mutex_t mutexPool;  /* 线程池的锁 */
    pthread_mutex_t mutexBusy;  /* busyNum 的锁 */
    pthread_cond_t isEmpty;     /* 等待队列是否为空 */
    pthread_cond_t isFull;      /* 等待队列是否已经满了 */

    int shoutdown;      /* 标识线程池是够关闭  1 为关闭 0 为打开*/
};

void handler_Worker(void *arg)
{
    Threadpool *pool = (Threadpool*)arg;
    pthread_mutex_unlock(&pool->mutexPool);   /* 带锁退出需要解锁 */
    pthread_t tid = pthread_self(); /* 获取该线程的线程号 */

    /* 找到这个线程所在的id */
    for(int i = 0; i < pool->maxNum; ++i)
    {
        if(tid == pool->workersID[i])   /* 找到了的话 那就将它退出 */
        {
            pool->workersID[i] = 0; /* 重置值,方便下次使用 */
            printf("WorkerID: %ld is Exiting().......\n",tid);
            break;
        }
    }
}

/* 单个线程退出函数 */
void *thread_Exit(Threadpool *pool)
{
    pthread_t tid = pthread_self(); /* 获取该线程的线程号 */

    /* 找到这个线程所在的id */
    for(int i = 0; i < pool->maxNum; ++i)
    {
        if(tid == pool->workersID[i])   /* 找到了的话 那就将它退出 */
        {
            pool->workersID[i] = 0; /* 重置值,方便下次使用 */
            printf("WorkerID: %ld is Exiting().......\n",tid);
            break;
        }
    }
    pthread_exit(NULL); /* 退出进程 */

}

/* 管理者线程 */
void *manager_Function(void *arg)
{
    Threadpool *pool = (Threadpool*)arg;

    while (!pool->shoutdown)  /* 如果线程池没有关闭 管理者线程 轮询 */
    {
        sleep(1);

        int liveNum;    /* 存活的数量 */
        int busyNum;    /* 忙的数量 */
        int queueSize;  /* 排队的数量 */

        /* 取出 存活的数量 排队的数量 */
        pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */
        liveNum = pool->liveNum;    
        queueSize = pool->queueSize;
        pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */

        /* 取出 忙的数量  */
        pthread_mutex_lock(&pool->mutexBusy);   /* busyNum 上锁 */
        busyNum = pool->busyNum;    
        pthread_mutex_unlock(&pool->mutexBusy); /* busyNum 解锁 */

//printf("\n\nliveNum = %d , busyNum = %d , queueSize = %d\n\n",liveNum,busyNum,queueSize);

        /* 线程的添加 */
        /* 要求:任务的个数 > 当前存活的线程个数 && 当前存活的个数 < 最大的线程数 */
        //if (queueSize > liveNum && liveNum < pool->maxNum)
        /* 要求:当前存活的个数 < 最大的线程数 && 等待队列里不为 0 */
        if (liveNum < pool->maxNum && queueSize != 0)
        {
            pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */

            /* 找到一个线程ID是空的 */
            int counter = 0;    //-------------------------------------------------------------< 这里没有考虑周全
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->workersID[i] == 0) /* 那么就在这来创建 */
                {
                    pthread_create(&pool->workersID[i], NULL, worker_Function, pool);   /* 工作者线程的创建 */
                    pool->liveNum++;    /* 生存的个数++ */
                    counter++;
                    printf("\n\nAdd pthread !\n\n");
                }
            }
            pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */
        }

        /* 线程的删除 */
        /* 要求:忙的线程 < 存活的线程数 && 存活的线程 > 最小线程数 */
        if (busyNum + NUMBER < liveNum && liveNum > pool->minNum)
        {
            /* 那么就要唤醒那些在睡眠的进程了 让他们自杀 */
            pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */

            for(int i = 0; i < NUMBER; i++)//-----------------------------------------------------< 这里没有考虑周全
            {
                pthread_cond_signal(&pool->isEmpty);    /* 唤醒线程 */
            }
        }
    }
    
}

/* 工作者线程 */
void *worker_Function(void *arg)
{
    Threadpool *pool = (Threadpool*)arg;

    while (1)   /* 工作者 线程轮询 */
    {
        //设置一个线程退出清理函数-->防止线程带锁退出
		//pthread_cleanup_push(handler_Worker,(void *)pool);

        pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */

        /* 如果 等待队列里面没有任务 &&  线程池没有被关闭*/
        while(pool->queueSize == 0 && !pool->shoutdown)
        {
            /* 没事干 那就让线程阻塞 */
            pthread_cond_wait(&pool->isEmpty, &pool->mutexPool);

            /* 如果被唤醒了 , 还没有出循环 , 那肯定是要进行线程的删除 */
            if(pool->exitNum > 0)
            {
                pool->exitNum--;
                if(pool->liveNum > pool->minNum)//----------------------------------------------------------------<考虑不周
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);/* 先把线程池解锁 */
                    thread_Exit(pool);  /* 线程的退出 */
                }
            }

            /* 当然,如果出了循环,不是要关闭线程池,就是有事要做了 */
        }

        if(pool->shoutdown) /* 如果线程池关闭了 */
        {
            pthread_mutex_unlock(&pool->mutexPool);/* 先把线程池解锁 */
            thread_Exit(pool);  /* 线程的退出 */
        }
        
        //将当前任务设置为不可被取消属性
		//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

        /* 到了这里,肯定是有任务需要认领  所以开始领取任务 现在线程池还是锁的 */
        Task task;  /* 创建一个任务的结构体 */
        task.func = pool->taskQ[pool->queueFront].func; /* 从队列的头领取一个任务 */
        task.arg = pool->taskQ[pool->queueFront].arg;   /* 领取需要传入的数据 */
        pool->queueFront = (pool->queueFront + 1) % (pool->queueCapacity);  /* 将头结点下移 */
        pool->queueSize--;//--------------------------------------------------------------------------------<考虑不周
        
        pthread_mutex_unlock(&pool->mutexPool);/* 先把线程池解锁 */

        printf("thread %ld start working......\n",pthread_self());

        

        /* 就要执行任务了,所以我们要将busyNum的锁 锁上,修改一下busyNum的个数 */
        pthread_mutex_lock(&pool->mutexBusy);   /* busyNum 上锁 */
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);   /* busyNum 解锁 */

        task.func(task.arg); /* 执行函数 */
        if (task.arg)
        {
            free(task.arg);
        }
        task.arg = NULL; //------------------------------------------------------------------------<考虑不周
        /* 函数执行完毕 这里我们就需要将 busyNum 的值修改回去 */
        pthread_mutex_lock(&pool->mutexBusy);   /* busyNum 上锁 */
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);   /* busyNum 解锁 */


        //pthread_cleanup_pop(0); /* 取消清理函数 */
		//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    }
    

    return NULL;
}

/* 创建并且初始化一个线程池 */
Threadpool *creat_Pthread(int queueCapacity, int maxNum, int minNum)
{
//printf("初始化成功!\n");
    Threadpool *pool = NULL;
    do
    {

        /* 开辟线程池空间 */
        pool = (Threadpool*)calloc(1,sizeof(Threadpool));
        if(NULL == pool)
        {
            /* 如果开辟空间失败,就退出循环 */
            printf("cread pool failed!\n");
            break;
        }

        pool->shoutdown = 0;  /* 线程池打开 */

        /* 任务队列的开辟 及 初始化 */
        pool->queueRear = 0;    /* 等待队列的第一个 */
        pool->queueFront = 0;   /* 等待队列的第一个 */
        pool->queueSize = 0;    /* 等待队列中任务的个数 */
        pool->queueCapacity = queueCapacity;    /* 等待队列的容量 */ 

        pool->taskQ = (Task *)calloc(1,sizeof(Task) * (pool->queueCapacity));    /* 根据容量开辟任务队列 */
        if(NULL == pool->taskQ)
        {
            /* 如果开辟空间失败,就退出循环 */
            printf("cread pool->taskQ failed!\n");
            break;
        }

        /* 线程的开辟 及 初始化 */
        pool->maxNum = maxNum;
        pool->minNum = minNum;
        pool->liveNum = minNum;
        pool->exitNum = 0;
        pool->busyNum = 0;

        /* 工作者线程ID创建 */
        pool->workersID = (pthread_t *)calloc(1,(pool->maxNum) * sizeof(pthread_t));
        if(NULL == pool->workersID)
        {
            /* 如果开辟空间失败,就退出循环 */
            printf("creadpool->workersID failed!\n");
            break;
        }

        /* 锁的初始化 这里的锁 如果初始化成功 都是 返回 0*/
        if ( pthread_mutex_init(&pool->mutexPool, NULL) != 0
          ||pthread_mutex_init(&pool->mutexBusy, NULL) != 0
          ||pthread_cond_init(&pool->isEmpty, NULL) != 0
          ||pthread_cond_init(&pool->isFull, NULL) != 0 )   /* 如果这些锁有一个没有初始化成功 */
        {
            /* 就退出循环 */
            printf("cread mutex_init or cond_init failed!\n");
            break;
        }

        /* 线程的创建 */
        pthread_create(&pool->managerID, NULL, manager_Function, pool); /* 管理者线程的创建 */
        for(int i = 0; i < pool->minNum; ++i)
        {
            pthread_create(&pool->workersID[i], NULL, worker_Function, pool);   /* 工作者线程的创建 */
        }

        return pool;

    } while (0);
    
    if(pool && pool->taskQ) /* 任务队列的销毁 */
    {
        free(pool->taskQ);
    }
    if(pool && pool->workersID) /* 工作者队列的销毁 */
    {
        free(pool->workersID);
    }
    if(pool)
    {
        free(pool); /* 线程池的销毁 */
    }    

    return NULL;
}

/* 添加任务函数 */
void thread_Add(Threadpool *pool, void (*func)(void*), void *arg)
{
    /* 要求:线程池没有关闭 线程队列有空闲的 */
    pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */

    int queueSize = pool->queueSize;

    while (queueSize == pool->queueCapacity && !pool ->shoutdown)
    {
        /* 如果没有满足这个要求 那么将会阻塞在这里 */
        pthread_cond_wait(&pool->isFull, &pool->mutexPool);
    }
    if(!pool ->shoutdown) /* 如果走进了这一步 那就说明要填加任务了 */ 
    {
        //pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */------------------------<这边出来就自动锁上了
        pool->taskQ[pool->queueRear].arg = arg;
        pool->taskQ[pool->queueRear].func = func;
        pool->queueSize++;
        pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;  /* 尾结点向后移 */

        pthread_cond_signal(&pool->isEmpty);    /* 唤醒已经阻塞的工作线程 *///-----------------------<这边没有给那些解锁
        pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */
    }
    else    /* 线程池要关闭了 */
    {
        pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */
        return;
    }
    

}

/* 线程池销毁函数 */
int thread_Destroy(Threadpool *pool)
{
    if(NULL == pool)
    {
        printf("poll is NULL!\n");
        return -1;
    }

    /* 将shoutdown 置为 1 */
    pool->shoutdown = 1;

    /* 等待管理线程的死亡 回收管理线程 */
    pthread_join(pool->managerID, NULL);
    printf("管理者遣散\n");

    /* 唤醒所有的沉睡的工作者 */
    for(int i = 0; i < pool->liveNum; ++i)
    {
        if(pool->workersID[i] != 0)
        {
            pthread_cond_signal(&pool->isEmpty);
            usleep(400);
        }
    }
    printf("工作者全部遣散\n");


    /* 销毁所有的锁 */
    if (pthread_mutex_destroy(&pool->mutexPool) == 0
      ||pthread_mutex_destroy(&pool->mutexBusy) == 0
      ||pthread_cond_destroy(&pool->isEmpty) == 0
      ||pthread_cond_destroy(&pool->isFull) == 0)
    {
        printf("锁全部都已经销毁\n");
    }

    /* 释放所有的空间 */
    if(pool->workersID)
    {
        free(pool->workersID);
    }
    if(pool->taskQ)
    {
        free(pool->taskQ);
    }
    free(pool);
    pool = NULL;
    printf("堆区空间全部释放完毕\n");

    return 0;
}
