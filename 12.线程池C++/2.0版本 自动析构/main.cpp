/*
 * @Author: 阿果果
 * @Date: 2022-07-28 20:56:18
 * @LastEditors: your Name
 * @LastEditTime: 2022-07-30 15:07:29
 * @Description: 
 */
#include "threadpool.hpp"

template<typename T>
void func(void* arg)
{
    T num = *((T *)arg);
    cout << "This is "<< pthread_self() << "num = " << num << endl;
    usleep(50000);/* 休眠千分之一毫秒 100毫秒 */  
}

int main()
{
    /* 创建线程池并初始化 */
    ThreadPool<int> T(2,10);
    /* 给线程池添加任务 */
    for(int i = 0; i < 1000; ++i)
    {
        int *num = new int(i + 100);
        T.add_Task(func<int>, num);
        usleep(2000);
    }
  
    sleep(20);

}