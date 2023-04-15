#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <queue>
#include <time.h>
#include <stdio.h>
using namespace std;

#define NUMBER 2  /* 添加删除任务的数量 */
using callback = void (*)(void *); /* 取别名 */

struct Task /* 任务结构体 */
{
    Task(callback func = nullptr, void *arg = nullptr, int len = 0):function(func), arg(arg), len(len){}
    Task(const Task &ob){
        if (this->arg != nullptr)
            free (this->arg);
        this->len = ob.len;
        this->arg = (void *)calloc(1,ob.len);
        memcpy(this->arg, ob.arg, this->len);
    }
    Task& operator=(Task& ob){
        if (this->arg != nullptr)
            free (this->arg);
        this->len = ob.len;
        this->arg = (void *)calloc(1,ob.len);
        memcpy(this->arg, ob.arg, this->len);
cout << "arg = " << *(int *)(this->arg) << endl;
cout << "等号赋值" << endl;
        return *this;
    }
    ~Task(){
        if (this->arg != nullptr){
            free (this->arg);
                arg = nullptr;
            cout << "释放" << endl;
        }
    }

    callback function;
    void *arg;
    int len;//记录 数据长度
};
static Task tmp_T;

class TaskQueue /* 任务队列类 */
{
public:
    TaskQueue();
    ~TaskQueue();

    //添加任务函数
    void addTask(Task &task);
    void addTask(callback func, void *arg, int len);

    //取出一个任务
    void getTask(Task &task) ;

    //获取当前队列中的任务个数
    inline int taskNum(){return m_queue.size();}

private:
    pthread_mutex_t mutexTask;  /* 任务队列锁 */
    queue<Task> m_queue;        /* 任务队列 */
};

TaskQueue::TaskQueue() /* TaskQueue构造函数 */
{
    pthread_mutex_init(&mutexTask, NULL); /* 锁的初始化 */
}

TaskQueue::~TaskQueue() /* TaskQueue析构函数 */
{
    pthread_mutex_destroy(&mutexTask);
}

void TaskQueue::addTask(Task &task) /* TaskQueue添加任务函数 {Task} */
{
    pthread_mutex_lock(&mutexTask);
    m_queue.push(task);
    pthread_mutex_unlock(&mutexTask);
}

void TaskQueue::addTask(callback func, void *arg, int len) /* TaskQueue添加任务函数 {callback}{void *}*/
{
    pthread_mutex_lock(&mutexTask);
    Task task;
    task.len = len;
    task.arg = arg;
    task.function = func;
    tmp_T = task;
    m_queue.push(tmp_T);
    pthread_mutex_unlock(&mutexTask);

    cout << " 98 " << endl;
}

void TaskQueue::getTask(Task &task) /* 获取任务函数 */
{
    pthread_mutex_lock(&mutexTask);
    if (m_queue.size() > 0)
    {
        task = m_queue.front();
cout << "arg = " << *(int *)(task.arg) << endl;
        m_queue.pop();
    }
    pthread_mutex_unlock(&mutexTask);
}



class ThreadPool /* 线程池类 */
{
public:
    ThreadPool(int minNum, int maxNum);
    ~ThreadPool();

    void add_Task(callback func, void *arg, int len); /* 添加任务 */
    int get_BusyNum();  /* 获取忙的线程数 */
    int get_AliveNum(); /* 获取存活的线程数 */
    void thread_Exit(); /* 单个线程退出函数 */
private:
    /* 工作者线程函数 */
    static void *worker(void *arg);
    /* 管理者线程函数 */
    static void *manager(void *arg);
    /* 意外退出处理函数 */
    static void handler(void *arg);
private:
    int minNum;     /* 线程的最小个数 */
    int maxNum;     /* 线程的最大个数 */
    int liveNum;    /* 线程的存活个数 */
    int busyNum;    /* 忙的线程个数  在执行任务的 */
    int exitNum;    /* 要退出的线程数 */
    TaskQueue *m_Task;  /* 任务队列 */

    pthread_t managerID;    /* 管理者线程的ID */
    pthread_t *workersID;   /* 工作者的线程ID 这里需要指针 因为要根据初始化才知道有多大*/

    pthread_mutex_t mutexPool;  /* 线程池的锁 */
    pthread_cond_t isEmpty;      /* 等待队列是否为空 */

    bool shoutdown = false;      /* 标识线程池是否关闭  true 为关闭 false 为打开*/
};

ThreadPool::ThreadPool(int minNum, int maxNum) /*  */
{
    m_Task = new TaskQueue;
    do
    {
        if ( nullptr == m_Task ) /* 如果开辟空间失败,就退出循环 */
        {
            cout << "cread pool failed!" << endl;
            break;
        }

        this->minNum = minNum;
        this->maxNum = maxNum;
        liveNum = minNum;
        busyNum = 0;
        exitNum = 0;

        workersID = new pthread_t[maxNum]; /* 工作者线程ID创建 */
        if ( nullptr == workersID ) /* 如果开辟空间失败,就退出循环 */
        {
            cout << "cread workersID failed!" << endl;
            break;
        }
        memset(workersID, 0, sizeof(pthread_t) * maxNum);

        if ( pthread_mutex_init(&mutexPool, NULL) != 0 ||/* 锁的初始化 这里的锁 如果初始化成功 都是 返回 0*/
             pthread_cond_init(&isEmpty, NULL) != 0)    /* 如果这些锁有一个没有初始化成功 就退出循环 */
        {
            cout << "cread mutex_init or cond_init failed! " << endl;
            break;
        }

        pthread_create( &managerID, NULL, manager , this); /* 管理者线程的创建 */
        for ( int i = 0; i < minNum; ++i )
        {
            pthread_create(&workersID[i], NULL, worker , this);   /* 工作者线程的创建 */
        }
cout << "初始化成功" << endl;
        return ;

    } while (0);
    
    if ( m_Task ) /* 任务队列的销毁 */
    {
        delete m_Task;
    }
    if ( workersID ) /* 工作者队列的销毁 */
    {
         delete []workersID;
    }
cout << "初始化失败" << endl;
    return ;
    /* 这里可以写一个异常 创建失败抛出异常 */
}

ThreadPool::~ThreadPool() /* ThreadPool析构函数 */
{
    shoutdown = true; /* 线程池关闭 */
    
    pthread_join( managerID, NULL ); /* 等待管理线程的死亡 回收管理线程 */
    cout << "管理者遣散" << endl;

    for (int i = 0; i < liveNum; ++i )
    {
        pthread_cond_signal(&isEmpty); /* 唤醒所有的沉睡的工作者 */
        usleep(400);
    }
    cout << "工作者全部遣散" << endl;

    if (pthread_mutex_destroy(&mutexPool) == 0 ||/* 销毁所有的锁 */
        pthread_cond_destroy(&isEmpty) == 0)
    {
        cout << "锁全部都已经销毁" << endl;
    }

    if ( m_Task ) /* 释放所有的空间 */
    {
        delete m_Task;  /* 任务队列的销毁 */ 
    }
    if ( workersID ) 
    {
         delete []workersID; /* 工作者队列的销毁 */
    }
    cout << "堆区空间全部释放完毕" << endl;
}

int ThreadPool::get_BusyNum()  /* 获取忙的线程数 */
{
    int busynum = 0;
    pthread_mutex_lock(&mutexPool);
    busynum = this->busyNum;
    pthread_mutex_unlock(&mutexPool);
    return busynum;
}

int ThreadPool::get_AliveNum() /* 获取存活的线程数 */
{
    int alivenum = 0;
    pthread_mutex_lock(&mutexPool);
    alivenum = this->liveNum;
    pthread_mutex_unlock(&mutexPool);
    return alivenum;
}

void ThreadPool::add_Task(callback func, void *arg, int len) /* 添加任务 */
{

    //pthread_mutex_lock(&mutexPool);   /* 先把关锁 线程池 */

    if(!shoutdown) /* 如果走进了这一步 那就说明要填加任务了 */ 
    {
        //tmp_T = Task t1(func, arg, len);
        m_Task->addTask(func, arg, len);
        pthread_cond_signal(&isEmpty);    /* 唤醒已经阻塞的工作线程 */ 
        //pthread_mutex_unlock(&mutexPool); /* 线程池解锁 */
    }
    else    /* 线程池要关闭了 */
    {
        pthread_mutex_unlock(&mutexPool); /* 线程池解锁 */
        return;
    }
    
}

void ThreadPool::thread_Exit() /* 单个线程退出函数 */
{
    pthread_t tid = pthread_self(); /* 获取该线程的线程号 */

    for(int i = 0; i < maxNum; ++i) /* 找到这个线程所在的id */
    {
        if(tid ==  workersID[i])   /* 找到了的话 重置值,方便下次使用 */
        {
            workersID[i] = 0;
            cout << "WorkerID: " << to_string(tid) << " is Exiting()......." << endl;
            break;
        }
    }
    pthread_exit(NULL); /* 退出线程 */
}

void *ThreadPool::manager(void *arg) /* 管理者线程 */
{
   
    ThreadPool *pool = static_cast<ThreadPool *>(arg);

    while (!pool->shoutdown)  /* 如果线程池没有关闭 管理者线程 轮询 */
    {
        sleep(2);

        int liveNum;    /* 存活的数量 */
        int busyNum;    /* 忙的数量 */
        int queueSize;  /* 队列里面等待任务的个数 */

        /* 取出 存活的数量 排队的数量 */
        /* 取出 忙的数量  */
        pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */
        liveNum = pool->liveNum;    
        busyNum = pool->busyNum;
        queueSize = pool->m_Task->taskNum();
        pthread_mutex_unlock(&pool->mutexPool); /* 线程池解锁 */
        
cout << "\n\nliveNum = " << liveNum  << "  busyNum = " << busyNum << " queuesize = " << queueSize << endl;

        /* 创建线程 */
        /* 要求:当前存活的个数 < 最大的线程数 && 任务队列任务数 > 存活的线程数*/
        if (liveNum < pool->maxNum && queueSize > liveNum)
        {
            pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */

            /* 找到一个线程ID是空的 */
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->workersID[i] == 0) /* 那么就在这来创建 */
                {
                    pthread_create(&pool->workersID[i], NULL, worker, pool);   /* 工作者线程的创建 */
                    pool->liveNum++;    /* 生存的个数++ */
                    counter++;
                    cout << "\n\n--------- Add thread ! ---------\n\n" << endl;
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

            for(int i = 0; i < NUMBER; i++)
            {
                pthread_cond_signal(&pool->isEmpty);    /* 唤醒线程 */
            }        
        }
    }
    return nullptr;
}

void *ThreadPool::worker(void *arg) /* 工作者线程 */
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);

    while (true)   /* 工作者 线程轮询 */
    {
        //设置一个线程退出清理函数-->防止线程带锁退出
		pthread_cleanup_push(handler, pool);

        pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */

        while(pool->m_Task->taskNum() == 0 && !pool->shoutdown) /* 如果 等待队列里面没有任务 &&  线程池没有被关闭*/
        {
            pthread_cond_wait(&pool->isEmpty, &pool->mutexPool); /* 没事干 那就让线程阻塞 */

            if (pool->exitNum > 0) /* 如果有退出数量 需要进行线程销毁 */
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);/* 先把线程池解锁 */
                    pool->thread_Exit();  /* 线程的退出 */
                }
            }
        }

        if(pool->shoutdown) /* 如果线程池关闭了 */
        {
            pthread_mutex_unlock(&pool->mutexPool); /* 先把线程池解锁 */
            pool->thread_Exit();  /* 当前线程退出 */
        }
        
        Task task;
        pool->m_Task->getTask(task); /* 获取任务 */
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexPool); /* 先把线程池解锁 */

cout << "Thread " << pthread_self() << " start working......" << endl;

		//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL); /* 将当前任务设置为不可被取消属性 */
        task.function(task.arg); /* 执行函数 */
        // delete static_cast<int *> (task.arg);
        // task.arg = nullptr;
        cout << "拿到并且执行完毕 arg = " << *(int *)(task.arg) << endl;
        //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);  /* 取消不可被取消属性 */

        pthread_mutex_lock(&pool->mutexPool);   /* 先把关锁 线程池 */
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);/* 先把线程池解锁 */

        pthread_cleanup_pop(0); /* 取消清理函数 */
    }

    return nullptr;
}

void ThreadPool::handler(void *arg) /* 意外退出处理函数 */
{
    ThreadPool *pool = static_cast<ThreadPool *> (arg);
    pthread_mutex_unlock(&pool->mutexPool);   /* 带锁退出需要解锁 */

    pthread_t tid = pthread_self(); /* 获取该线程的线程号 */

    for(int i = 0; i < pool->maxNum; ++i)/* 找到这个线程所在的id */
    {
        if(tid == pool->workersID[i])   /* 找到了的话 那就将它退出 */
        {
            pool->workersID[i] = 0; /* 重置值,方便下次使用 */
            printf("WorkerID: %ld is Exiting().......\n",tid);
            break;
        }
    }
}


#endif
