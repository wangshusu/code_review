## C++线程并发

### 1.1启动线程

```C++
#include <thread> // 头文件

// 1.函数初始化
void do_some_work();
std::thread my_thread(do_some_work);


// 2.类传入参数初始化
class background_task
{
pubulic:
    void operator()()const
    {
        do_something();
        do_something_else();
    }
}
background_task f;
std::thread my_thread(f);

// 3.使用lambda表达式
std::thread my_thread([]{
    do_something();
    do_something_else();
})
```



