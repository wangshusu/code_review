/*
 * @Author: 阿果果
 * @Date: 2022-07-28 20:56:18
 * @LastEditors: your Name
 * @LastEditTime: 2023-01-27 15:08:47
 * @Description: 
 */
#include "threadpool.hpp"
void func(void* arg)
{
    cout << "开始任务" << endl;
    int num = *((int *)arg);
    cout << "This is "<< pthread_self() << "num = " << num << endl;
    usleep(50000);/* 休眠千分之一毫秒 100毫秒 */   
}

int main()
{
    /* 创建线程池并初始化 */
    ThreadPool T(2,10);
    /* 给线程池添加任务 */
    for(int i = 0; i < 1000; ++i)
    {
        int *num = (int *)calloc(1,sizeof(int));
        *num = 100+i;
        T.add_Task(func, num, sizeof(int));
        usleep(2000);
    }
  
    sleep(15);

}