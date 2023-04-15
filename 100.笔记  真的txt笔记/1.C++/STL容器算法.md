# 容器

STL(Standard Template Library,标准模板库)

* **STL的6大组件：**容器、算法、迭代器、适配器、仿函数、空间配置 
    * **容器：**存放数据 
    * **算法：**操作数据 
    * **迭代器：**算法 通过迭代器 操作 容器 
    * **适配器：**为算法 提供更多的接口 
    * **仿函数：**为算法 提供策略
    * **空间配置：**为算法、容器提供动态空间 

* **算法分类：**质变算法、非质变算法
    * **质变算法：**会更改容器的值（拷贝，替换，删除等等）
    * **非质变算法：**是指运算过程中不会更改区间内的元素内容，例如查找、计数、遍历、寻 找极值等等



## 1.string 容器

### 1.1 *string* *构造函数*

```c++
string();//创建一个空的字符串 例如: string str; 
string(const string& str);//使用一个 string 对象初始化另一个 string 对象 
string(const char* s);//使用字符串 s 初始化 
string(int n, char c);//使用 n 个字符 c 初始化 v
```

### 1.2 *string* *基本赋值操作*

```c++
string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串
string& operator=(const string &s);//把字符串 s 赋给当前的字符串 
string& operator=(char c);//字符赋值给当前的字符串

string& assign(const char *s);//把字符串 s 赋给当前的字符串 
string& assign(const char *s, int n);//把字符串 s 的前 n 个字符赋给当前的字符串
string& assign(const string &s);//把字符串 s 赋给当前字符串 
string& assign(int n, char c);//用 n 个字符 c 赋给当前字符串 
string& assign(const string &s, int start, int n);//将 s 从 start 开始 n 个 字符赋值给字符串
```

### 1.3 *string* *存取字符操作*

```c++
char& operator[](int n);//通过[]方式取字符 
char& at(int n);//通过 at 方法获取字符
```

### 1.4 *string* *拼接操作*

```c++
string& operator+=(const string& str);//重载+=操作符 
string& operator+=(const char* str);//重载+=操作符 
string& operator+=(const char c);//重载+=操作符 

string& append(const char *s);//把字符串 s 连接到当前字符串结尾 
string& append(const char *s, int n);//把字符串 s 的前 n 个字符连接到当前字 符串结尾 
string& append(const string &s);//同 operator+=() 
string& append(const string &s, int pos, int n);//把字符串 s 中从 pos 开始 的 n 个字符连接到当前字符串结尾 
string& append(int n, char c);//在当前字符串结尾添加 n 个字符 c
```

### 1.5 *string* *查找和替换*

```c++
int find(const string& str, int pos = 0) const; //查找 str 第一次出现位置, 从 pos 开始查找 
int find(const char* s, int pos = 0) const; //查找 s 第一次出现位置,从 pos 开始查找 
int find(const char* s, int pos, int n) const; //从 pos 位置查找 s 的前 n 个 字符第一次位置 
int find(const char c, int pos = 0) const; //查找字符 c 第一次出现位置 

int rfind(const string& str, int pos = npos) const;//查找 str 最后一次位置, 从 pos 开始查找 
int rfind(const char* s, int pos = npos) const;//查找 s 最后一次出现位置,从 pos 开始查找 
int rfind(const char* s, int pos, int n) const;//从 pos 查找 s 的前 n 个字符 最后一次位置 
int rfind(const char c, int pos = 0) const; //查找字符 c 最后一次出现位置 

string& replace(int pos, int n, const string& str); //替换从 pos 开始 n 个 字符为字符串 
str string& replace(int pos, int n, const char* s); //替换从 pos 开始的 n 个字 符为字符串 s
```

### 1.5 *string* *比较操作*

```c++
/*
	compare 函数在>时返回 1，<时返回 -1，==时返回 0。
	比较区分大小写，比较时参考字典顺序，排越前面的越小。
	大写的 A 比小写的 a 小
*/
int compare(const string &s) const;//与字符串 s 比较 
int compare(const char *s) const;//与字符串 s 比较
```

### 1.6 *string* *子串*

```c++
string substr(int pos = 0, int n = npos) const;//返回由 pos 开始的 n 个字符 组成的字符串
```

### 1.7 *string* *插入和删除操作*

```c++
string& insert(int pos, const char* s); //插入字符串 
string& insert(int pos, const string& str); //插入字符串 
string& insert(int pos, int n, char c);//在指定位置插入 n 个字符 c 
string& erase(int pos, int n = npos);//删除从 Pos 开始的 n 个字符
```

### 1.8 *string* *和* *c-style* *字符串转换*

```c++
//string 转 char* 
string str = "itcast"; 
const char* cstr = str.c_str(); 
//char* 转 string 
char* s = "itcast"; 
string str(s);
```







## 2. vector 容器

> *Vector* *支持随机存取*

### 2.1 *vector* *构造函数*

```c++
vector<T> v; //采用模板实现类实现，默认构造函数 
vector(v.begin(), v.end());//将 v[begin(), end())区间中的元素拷贝给本身。 
vector(n, elem);//构造函数将 n 个 elem 拷贝给本身。 
vector(const vector &vec);//拷贝构造函数。
```

### 2.2 *vector* *常用赋值操作*

```c++
assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。 
assign(n, elem);//将 n 个 elem 拷贝赋值给本身。 
vector& operator=(const vector &vec);//重载等号操作符 
swap(vec);// 将 vec 与本身的元素互换。
```

swap配合可以缩小容器到合适的大小  ↓   ↓   ↓ 

```c++
vector<int>(v1).swap(v1);
cout<<"大小:"<<v1.size()<<" 容量:"<<v1.capacity()<<endl;
```

### 2.3 *vector* *大小操作* 

```c++
size();//返回容器中元素的个数 
empty();//判断容器是否为空 
resize(int num);//重新指定容器的长度为 num，若容器变长，则以默认值填充新位置。	
				//如果容器变短，则末尾超出容器长度的元素被删除。 
resize(int num, elem);//重新指定容器的长度为 num，若容器变长，则以 elem 值填充新位置。
					  //如果容器变短，则末尾超出容器长>度的元素被删除。 
capacity();//容器的容量 
reserve(int len);//容器预留 len 个元素长度，预留位置不初始化，元素不可访问。
```

### 2.4 *vector* *数据存取操作*

```c++
at(int idx); //返回索引 idx 所指的数据，如果 idx 越界，抛出 out_of_range 异 常。
operator[];//返回索引 idx 所指的数据，越界时，运行直接报错 
front();//返回容器中第一个数据元素 
back();//返回容器中最后一个数据元素
```

### 2.5 *vector* *插入和删除操作*

```c++
insert(const_iterator pos, int count, ele);//迭代器指向位置 pos 插入 count 个元素 ele. 
push_back(ele); //尾部插入元素 ele 
pop_back();//删除最后一个元素 
erase(const_iterator start, const_iterator end);//删除迭代器从 start 到 en d 之间的元素 
erase(const_iterator pos);//删除迭代器指向的元素 
clear();//删除容器中所有元素
```



## 3. deque 容器

### 3.1 *deque* *构造函数*

```c++
deque<T> deqT;//默认构造形式 
deque(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。 
deque(n, elem);//构造函数将 n 个 elem 拷贝给本身。 
deque(const deque &deq);//拷贝构造函数。
```

### 3.2 *deque* *赋值操作*

```c++
assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。 
assign(n, elem);//将 n 个 elem 拷贝赋值给本身。 
deque& operator=(const deque &deq); //重载等号操作符 
swap(deq);// 将 deq 与本身的元素互换
```

### 3.3 *deque* *大小操作*

```c++
deque.size();//返回容器中元素的个数 
deque.empty();//判断容器是否为空 
deque.resize(num);//重新指定容器的长度为 num,若容器变长，则以默认值填充新位置。
				  //如果容器变短，则末尾超出容器长度的元素被删除。 
deque.resize(num, elem); //重新指定容器的长度为 num,若容器变长，则以 elem 值 填充新位置,
						 //如果容器变短，则末尾超出容器长度的元素被删除。
```

### 3.4 *deque* *双端插入和删除操作*

```c++
push_back(elem);//在容器尾部添加一个数据 
push_front(elem);//在容器头部插入一个数据 
pop_back();//删除容器最后一个数据 
pop_front();//删除容器第一个数据
```

### 3.5 *deque* *数据存取*

```c++
at(idx);//返回索引 idx 所指的数据，如果 idx 越界，抛出 out_of_range。 
operator[];//返回索引 idx 所指的数据，如果 idx 越界，不抛出异常，直接出错。 
front();//返回第一个数据。 
back();//返回最后一个数据
```

### 3.6 *deque* *插入操作*

```c++
insert(pos,elem);//在 pos 位置插入一个 elem 元素的拷贝，返回新数据的位置。 
insert(pos,n,elem);//在 pos 位置插入 n 个 elem 数据，无返回值。 
insert(pos,beg,end);//在 pos 位置插入[beg,end)区间的数据，无返回值。
```

### 3.7 *deque* *删除操作*

```c++
clear();//移除容器的所有数据 
erase(beg,end);//删除[beg,end)区间的数据，返回下一个数据的位置。 
erase(pos);//删除 pos 位置的数据，返回下一个数据的位置。
```





## 4. stack 栈容器

> stack 容器没有迭代器 不提供遍历功能

### 4.1 *stack 构造函数*

```c++
stack<T> stkT;//stack 采用模板类实现， stack 对象的默认构造形式： 
stack(const stack &stk);//拷贝构造函数
```

### 4.2 *stack 赋值操作*

```c++
stack& operator=(const stack &stk);//重载等号操作符
```

### 4.3 *stack 数据存取操作*

```c++
push(elem);//向栈顶添加元素 
pop();//从栈顶移除第一个元素 
top();//返回栈顶元素
```

### 4.4  *stack 大小操作*

```c++
empty();//判断堆栈是否为空 
size();//返回堆栈的大小
```







## 5.queue 队列容器

> queue 容器没有迭代器 不支持随机访问

### 5.1 *queue* 

```c++
//queue 构造函数
queue<T> queT;//queue 采用模板类实现，queue 对象的默认构造形式： 
queue(const queue &que);//拷贝构造函数

//queue 存取、插入和删除操作
push(elem);//往队尾添加元素 
pop();//从队头移除第一个元素
back();//返回最后一个元素 
front();//返回第一个元素

//queue 赋值操作
queue& operator=(const queue &que);//重载等号操作符

//queue 大小操作
empty();//判断队列是否为空 
size();//返回队列的大小
```



### 5.2 priority_queue

> 与queu相比 自定义其中数据的优先级 让优先级高的排在前面先出队

函数体 : 

```c++
/*
	@Type 就是数据类型
	@Container 就是容器类型
	@Functional 就是比较的方式
	注意: 1.当需要用自定义的数据类型时才需要传入这三个参数，使用基本数据类型时，只需要传入数据类型，默认是大顶堆。
		 2.Container必须是用数组实现的容器，比如vector,deque等等,但不能用list,默认用的是vector。
*/
priority_queue<Type, Container, Functional>
    
//升序队列
priority_queue <int,vector<int>,greater<int> > q;
//降序队列
priority_queue <int,vector<int>,less<int> >q;
```

**函数操作 :** 

```c++
//queue 存取、插入和删除操作 这里有一点略有修改
top();//返回第一个元素
```

**网址连接 :** 

http://c.biancheng.net/view/480.html



## 6.list 链表容器









## 7. set 容器

### 7.1 *set* *构造函数*

```c++
set<T> st;//set 默认构造函数： 
mulitset<T> mst; //multiset 默认构造函数: 
set(const set &st);//拷贝构造函数
```

### 7.2 *set* *赋值操作*

```c++
set& operator=(const set &st);//重载等号操作符 
swap(st);//交换两个集合容器
```

### 7.3 *set* *大小操作*

```c++
size();//返回容器中元素的数目 
empty();//判断容器是否为空
```

### 7.4 *set* *插入和删除操作*

```c++
insert(elem);//在容器中插入元素。 
clear();//清除所有元素 
erase(pos);//删除 pos 迭代器所指的元素，返回下一个元素的迭代器。 
erase(beg, end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。 
erase(elem);//删除容器中值为 elem 的元素。
```

### 7.5 *set* *查找操作*

```c++
find(key);//查找键 key 是否存在,若存在，返回该键的元素的迭代器；若不存在，返回 set.end(); 
count(key);//查找键 key 的元素个数 
lower_bound(keyElem);//返回第一个 key>=keyElem 元素的迭代器。 
upper_bound(keyElem);//返回第一个 key>keyElem 元素的迭代器。 
equal_range(keyElem);//返回容器中 key 与 keyElem 相等的上下限的两个迭代器。
```





## 8. pair 对组

对组(pair)将一对值组合成一个值，这一对值可以具有不同的数据类型，两个值可以分别用 pair 的两个公有属性 first 和 second 访问。 类模板：template <class T1, class T2> struct pair. 如何创建对组?

```C++
//第一种方法创建一个对组
pair<string, int> pair1(string("name"), 20); 
cout << pair1.first << endl; //访问 pair 第一个值 
cout << pair1.second << endl;//访问 pair 第二个值

//第二种 
pair<string, int> pair2 = make_pair("name", 30); 
cout << pair2.first << endl; 
cout << pair2.second << endl;

//pair=赋值 
pair<string, int> pair3 = pair2; 
cout << pair3.first << endl; 
cout << pair3.second << endl;
```



## 9. **map/multimap** **容器**

### 9.1 *map* *构造函数*

```c++
map<T1, T2> mapTT;//map 默认构造函数: 
map(const map &mp);//拷贝构造函数
```

### 9.2 *map* *赋值操作*

```c++
map& operator=(const map &mp);//重载等号操作符 
swap(mp);//交换两个集合容器
```

### 9.3 *map* *大小操作*

```c++
size();//返回容器中元素的数目 
empty();//判断容器是否为空
```

### 9.4 *map* *插入数据元素操作*

```c++
map.insert(...); //往容器插入元素，返回 pair<iterator,bool>
map<int, string> mapStu;

// 第一种 通过 pair 的方式插入对象 
mapStu.insert(pair<int, string>(3, "小张"));

// 第二种 通过 pair 的方式插入对象 
mapStu.inset(make_pair(-1, "校长"));

// 第三种 通过 value_type 的方式插入对象 
mapStu.insert(map<int, string>::value_type(1, "小李"));

// 第四种 通过数组的方式插入值 
mapStu[3] = "小刘"; 
mapStu[5] = "小王";
```

### 9.5 *map* *删除操作*

```c++
clear();//删除所有元素 
erase(pos);//删除 pos 迭代器所指的元素，返回下一个元素的迭代器。 
erase(beg,end);//删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。 
erase(keyElem);//删除容器中 key 为 keyElem 的对组。
```

### 9.6 *map* *查找操作*

```c++
find(key);//查找键 key 是否存在,若存在，返回该键的元素的迭代器；/若不存在，返 回 map.end(); 
count(keyElem);//返回容器中 key 为 keyElem 的对组个数。对 map 来说，要么是 0， 要么是 1。对 multimap 来说，值可能大于 1。 
lower_bound(keyElem);//返回第一个 key>=keyElem 元素的迭代器。 
upper_bound(keyElem);//返回第一个 key>keyElem 元素的迭代器。 
equal_range(keyElem);//返回容器中 key 与 keyElem 相等的上下限的两个迭代器。
```







## 10. 练习

### 10.1 *作业*

有 5 名选手：选手 ABCDE，10 个评委分别对每一名选手打分，去除最高分，去除评委中最低分，取平均分。

​	\1. 创建五名选手，放到 vector 中

​	\2. 遍历 vector 容器，取出来每一个选手，执行 for 循环，可以把 10 个评分打分存到 deque 容器中

​	\3. sort 算法对 deque 容器中分数排序，pop_back pop_front 去除最高和最低分 

​	\4. deque 容器遍历一遍，累加分数，累加分数/d.size()

​	\5. person.score = 平均分

### 10.2 *作业*





# 算法

## 1. 函数对象

重载了  **函数调用运算符**  `() ` 的**类** 实例化的**对象** 就叫：函数对象

函数对象+（） 触发 重载函数调用运算符 执行 ==>类似函数调用 （**仿函数**）

![image-20220725143447789](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725143447.png)

如果函数对象 有一个参数 叫：一元函数对象 

如果函数对象 有二个参数 叫：二元函数对象

如果函数对象 有三个参数 叫：多元函数对象



## 2. 谓词

返回值为 **bool类型** 的普通函数或仿函数 都叫 **谓词** 。

如果谓词 有一个参数 叫：一元谓词 

如果谓词 有二个参数 叫：二元谓词 

别想了  一般不会有三个参数



### 2.1 一元谓词

用户查找

> 寻找某个值 或者找大于等于某个值的地点  这里都是为这些查找提供判断方法

![image-20220725143806294](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725143806.png)



```c++
#include<vector>
#include<algorithm>
bool greaterThan30(int value) // 全局函数
{
    return value>30;
}

class GreaterThan30	// 类
{
public:
    bool operator()(int value)
    {
        return value>30;
    }
};

void test02()
{
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    //find_if条件查找
    vector<int>::iterator ret; // 迭代器
    
    //普通函数提供策略 函数名
    //ret = find_if(v1.begin(), v1.end(),greaterThan30);
    
    //仿函数提供策略 类名称+（）
    ret = find_if(v1.begin(), v1.end(),GreaterThan30());
    
    if(ret != v1.end())
    {
        cout<<"寻找的结果:"<<*ret<<endl;
    }
}
```

![image-20220725144823991](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725144824.png)



### 2.2 二元谓词

用户排序

> 类似于提供一个算法 解决程序如何进行排序的问题

```c++
#include<vector>
#include<algorithm>
bool myGreaterInt(int v1, int v2) // 全局函数
{
    return v1>v2;
}

class GreaterThan30	// 类
{
public:
    bool operator()(int v1, int v2)
    {
        return v1>v2;
    }
};

void test02()
{
    vector<int> v1;
    v1.push_back(90);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    printVectorAll(v1); // 遍历函数
    
    //普通函数提供策略 函数名
    //sort(v1.begin(), v1.end(), myGreaterInt);
    
    /仿函数提供策略 类名称+（）
    sort(v1.begin(), v1.end(), MyGreaterInt());   
    
    printVectorAll(v1); // 遍历函数
}
```



## 3. 内建函数对象

系统提供好的  函数对象

```c++
//6个算数类函数对象,除了negate是一元运算，其他都是二元运算。
template<class T> T plus<T>//加法仿函数
template<class T> T minus<T>//减法仿函数
template<class T> T multiplies<T>//乘法仿函数
template<class T> T divides<T>//除法仿函数
template<class T> T modulus<T>//取模仿函数
template<class T> T negate<T>//取反仿函数

//6个关系运算类函数对象,每一种都是二元运算。
template<class T> bool equal_to<T>//等于
template<class T> bool not_equal_to<T>//不等于
template<class T> bool greater<T>//大于
template<class T> bool greater_equal<T>//大于等于
template<class T> bool less<T>//小于
template<class T> bool less_equal<T>//小于等于
    
//逻辑运算类运算函数,not为一元运算，其余为二元运算。
template<class T> bool logical_and<T>//逻辑与
template<class T> bool logical_or<T>//逻辑或
template<class T> bool logical_not<T>//逻辑非
```

![image-20220725150105168](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725150105.png)

接下来就是一个小重点

![image-20220725150152488](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725150152.png)

> 这里的  `  bind2nd(greater<int>(), 30)`  是将两个参数合二为一
>
> 共同传入这个参数  简称适配器



## 4. 适配器

适配器 为算法 提供接口

### 4.1 函数对象 适配器

```c++
#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;

//第二步：公共继承binary_function 参数萃取
class printInt:public binary_function<int,int,void>
{
public:
    //第三步：整个函数加const修饰
    void operator()(int value, int tmp) const
    {
        cout<<"value="<<value<<" tmp="<< tmp<<endl;
    }
};

void test02()
{
    vector<int> v1;
    v1.push_back(90);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    //for_each 提取容器的每个元素
    //第一步bind2nd 或bind1st
    //bind2nd将100绑定到第二个参数tmp上 容器的元素在value上
    //bind1fst将100绑定到第一个参数value上 容器的元素在tmp上
    for_each(v1.begin(), v1.end(),
             bind2nd(printInt(),100) );
    cout<<endl;
}
```

bind2nd 第二种绑定

![image-20220725152446392](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725152623.png)

bind1fst 第一种绑定

![image-20220725152508099](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725152656.png)



### 4.2 函数指针 适配器

> 说明 : C++ 中的函数名不能代表函数入口地址  具体的入口地址应该是 函数名+形参列表

```c++
void myPrintInt(int value, int tmp)
{
    cout<<"value="<<value<<" tmp="<< tmp<<endl;
}

void test02()
{
    vector<int> v1;
    v1.push_back(90);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    //for_each 提取容器的每个元素
    //第一步bind2nd 或bind1st
    //bind2nd将100绑定到第二个参数tmp上 容器的元素在value上
    for_each(v1.begin(), v1.end(),
             bind2nd(ptr_fun(myPrintInt),100) );
    cout<<endl;
}
```

> 这里的函数地址需要使用 ptr_fun 进行转换



### 4.3 成员函数 适配器

> 这里的成员函数适配器需要注意的是  指定的函数必须是这个类中的 不然无法启用

```c++
class Data
{
public:
    int data;
public:
    Data(){}
    Data(int d):data(d){}
    void printInt(int tmp){	// 成员函数
         cout<<"value="<<value<<" tmp="<< tmp<<endl;
    }
}

void test02()
{
    vector<int> v1;
    v1.push_back(90);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    //for_each 提取容器的每个元素
    //bind2nd将100绑定到第二个参数tmp上 容器的元素在value上
    for_each(v1.begin(), v1.end(),bind2nd
             ((&Data::myPrintInt),100));
    cout<<endl;
}
```

> 这里的类函数还要进行 作用域的声明 &Data::myPrintInt 这样的话指向的就是这个函数表的入口地址 一样不是这个函数的入口地址



### 4.4 取反 适配器

* not1 一元取反

![image-20220725203804881](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725203804.png)

> 一元取反适用于用户查找数据 greater<int> 原意为 大于 取反后为小于等于

![image-20220725204154458](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725204154.png)

* not2 二元取反

```c++
void test02()
{
    vector<int> v1;
    v1.push_back(90);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    
    //lambda 表达式 c++11才支持
    //[]里面啥都不写 lambda不能识别 外部数据
    //[=] lambda能对 外部数据 读操作
    //[&] lambda能对 外部数据 读写操作
    for_each(v1.begin(), v1.end(), 
             [&](int val){cout<<val<<" ";});
    cout << endl;
    
    // 排序算法  采用二元取反
    sort(v1.begin(), v1.end(),not2(greater<int>()));
    
    for_each(v1.begin(), v1.end(), 
             [&](int val){cout<<val<<" ";});
    cout << endl;
}
```

![image-20220725204203936](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220725204203.png)



## 5. 常见遍历算法

### 5.1 for_each 遍历算法

```C++
/*
	for_each 遍历算法
	@param beg 开始迭代器
	@param end 结束迭代器
	@param _callback 函数回调或者函数对象
	@return 函数对象
*/
for_each(iterator beg, iterator end, _callback);
```



### 5.2 transform 算法

```c++
/*
	transform 算法
	注意 : transform 不会给目标容器分配内存，所以需要我们提前分配好内存
	@param beg1 源容器开始迭代器
	@param end1 源容器结束迭代器
	@param beg2 目标容器开始迭代器
	@param _cakkback 回调函数或者函数对象
	@return 返回目标容器迭代器
*/
 transform(iterator beg1, iterator end1, iterator beg2, _callbakc);
```

示例:

```c++
int myTransInt01(int val)
{
    return val;
}

void test01()
{
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(70);
    v1.push_back(50);
    v1.push_back(30);
    v1.push_back(20);
    
    vector<int> v2;
    v2.resize(v1.size()); // 初始化V2的容器大小
    transform(v1.begin(), v1.end(), v2.begin(), myTransInt01);
    
}
```

> 注意 : 这里的 transform 中的 _cakkback 参数(第三个) 他的返回值是作为目标容器的 存储值的 所以可以在这个函数里对读取到的数据进行一些处理 



### 5.3 遍历容器 方法集合













## 6. 常见查找算法

### 6.1 *find 算法*

```c++
/*
	find算法 查找元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return 返回查找元素的位置
*/
find(iterator beg, iterator end, value)
```

案例:

```c++
class Person
{
    friend ostream &operator<<(ostream &out, Person &ob); // 重载 << 输出运算符
private:
    int age;
    string name;
public:
    Person(int age, string name):age(age),name(name){}  // 有参构造
    bool operator==(const Person &ob){
        return ((this->age == ob.age) && (this->name == ob.name)); // 重载 == 符号
    }
};

ostream &operator<<(ostream &out, Person &ob) // 重载 << 输出运算符
{
    out << ob.age << " " << ob.name;
    return out;
}

void test01()
{
    vector<Person> p1;
    p1.push_back(Person(18, "lucy"));
    p1.push_back(Person(13, "bobb"));
    p1.push_back(Person(19, "jony"));

    vector<Person>::iterator ret;
    ret = find(p1.begin(), p1.end(), Person(13, "bobb"));
    if (ret != p1.end())
    {   
        cout << "找到了" << (*ret) << endl;
    }
}
```

> 这里的 == 比较需要重载 bool 类型的



### 6.2 *find_if 算法*

```c++
/*
	find_if算法 条件查找
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback 回调函数或者谓词(返回bool类型的函数对象)
	@return 返回查找元素的位置
*/
find_if(iterator beg, iterator end, _callback);
```

案例: 

```c++
class Person
{
    friend class Compare;
    friend ostream &operator<<(ostream &out, Person &ob); // 重载 << 输出运算符
public:
    int age;
    string name;
public:
    Person(int age, string name):age(age),name(name){}  // 有参构造
};

bool operator==(const Person &p1, const Person &p2){  // 重载 == 符号
        if ((p1.age == p2.age) && (p1.name == p2.name))
            return true;
        else
            return false;
}

ostream &operator<<(ostream &out, Person &ob) // 重载 << 输出运算符
{
    out << ob.age << " " << ob.name;
    return out;
}

void test01()
{
    vector<Person> p1;
    p1.push_back(Person(18, "lucy"));
    p1.push_back(Person(13, "bobb"));
    p1.push_back(Person(19, "jony"));

    vector<Person>::iterator ret;
    ret = find_if(p1.begin(), p1.end(), bind2nd(equal_to<Person>(), Person(13, "bobb")));
    if (ret != p1.end())
    {
        cout << "哈哈找到了" << *ret <<endl;
    }
}
```

> 这里的 == 符号的重载需要定义在外部  函数内的 == 号函数重载他识别不到  我也不知道为什么



### 6.3 *adjacent_find 算法*

```c++
/*
	adjacent_find 算法 查找相邻重复元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback 回调函数或者谓词(返回bool类型的函数对象)
	@return 返回相邻元素的第一个位置的迭代器
*/
adjacent_find(iterator beg, iterator end, _callback);
```

案列:

```c++
void test01()
{
    vector<Person> p1;
    p1.push_back(Person(18, "lucy"));
    p1.push_back(Person(13, "bobb"));
    p1.push_back(Person(13, "bobb"));
    p1.push_back(Person(19, "jony"));
    p1.push_back(Person(19, "jony"));

    vector<Person>::iterator ret;
    ret = adjacent_find(p1.begin(), p1.end());
    if (ret != p1.end())
    {
        cout << "哈哈找到了" << *ret <<endl;
    }
}
```

> 只能返回第一个重复的值得地址  返回的是第一个出现该元素的地址
>
> 这个函数有两种形式的 ①不写callback函数 ②写callcack函数



### 6.4 *binary_search 算法*

```c++
/*
	binary_search 算法 二分查找法
		注意: 在无序序列中不可用
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 查找的元素
	@return bool 查找返回 true 否则 false
*/
bool binary_search(iterator beg, iterator end, value);
```





### 6.5 *count* *算法*

```c++
/*
	count 算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param value 回调函数或者谓词(返回 bool 类型的函数对象)
	@return int 返回元素个数
*/
count(iterator beg, iterator end, value);
```





### 6.6 *count_if* *算法*

```c++
/*
	count_if 算法 统计元素出现次数
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param callback 回调函数或者谓词(返回 bool 类型的函数对象)
	@return int 返回元素个数
*/
count_if(iterator beg, iterator end, _callback);
```



## 7. 常用排序算法

### 7.1 *merge* *算法*

```c++
/*
	merge 算法 容器元素合并，并存储到另一容器中
		注意:两个容器必须是有序的
	@param beg1 容器 1 开始迭代器
	@param end1 容器 1 结束迭代器
	@param beg2 容器 2 开始迭代器
	@param end2 容器 2 结束迭代器
	@param dest 目标容器开始迭代器
*/
merge(iterator beg1, iterator end1, 
      iterator beg2, iterator end2, 
      iterat or dest)
```



### 7.2 *sort* *算法*

> 必须支持随机访问

```c++
/*
	sort 算法 容器元素排序
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param _callback 回调函数或者谓词(返回 bool 类型的函数对象)
*/
sort(iterator beg, iterator end, _callback)
```





### 7.3 *random_shuffle* *算法*

> 这里的随机算法也是通过随机数的  设置随机数种子可以达到更好的效果

```c++
/*
	random_shuffle 算法 对指定范围内的元素随机调整次序
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
*/
random_shuffle(iterator beg, iterator end)
```





### 7.4 *reverse* *算法*

```c++
/*
	reverse 算法 反转指定范围的元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
*/
reverse(iterator beg, iterator end)
```





## 8. 常用拷贝替换算法

### 8.1 *copy* *算法*

> 需要提前开辟好空间

```c++
/*
	copy 算法 将容器内指定范围的元素拷贝到另一容器中
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
*/
copy(iterator beg, iterator end, iterator dest)
```



### 8.2 *replace* *算法*

```c++
/*
	replace 算法 将容器内指定范围的旧元素修改为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
	@param oldvalue 旧元素
	@param oldvalue 新元素
*/
replace(iterator beg, iterator end, oldvalue, newvalue)
```



### 8.3 *replace_if* *算法*

```c++
/*
	replace_if 算法 将容器内指定范围满足条件的元素替换为新元素
	@param beg 容器开始迭代器
	@param end 容器结束迭代器
	@param dest 目标起始迭代器
	@param callback 函数回调或者谓词(返回 Bool 类型的函数对象) 
	@param oldvalue 新元素
*/
replace_if(iterator beg, iterator end, _callback, newvalue)
```



### 8.4 *swap* *算法*

```c++
/*
	swap 算法 互换两个容器的元素 
	@param c1 容器 1 
	@param c2 容器 2
*/
swap(container c1, container c2)
```



## 9. 常用算法生成函数

### 9.1 *accumulate* *算法*

```c++
/*
	accumulate 算法 计算容器元素累计总和 
	@param beg 容器开始迭代器 
	@param end 容器结束迭代器 
	@param value 累加值 也就是加上多少
*/
#include <numeric>
accumulate(iterator beg, iterator end, value)
```





### 9.2 *fill* *算法*

```c++
/*
	fill 算法 向容器中添加元素
	@param beg 容器开始迭代器 
	@param end 容器结束迭代器 
	@param value t 填充元素
*/
fill(iterator beg, iterator end, value)
```





## 10. 常用集合算法

### 10.1 *set_intersection* *算法*

> 需要事先分配地址 ∵交集 ∴分配   ` min(v1.size(), v2.size())`

```c++
/*
	set_intersection 算法 求两个 set 集合的交集 
	注意:两个集合必须是有序序列 
	@param beg1 容器 1 开始迭代器 
	@param end1 容器 1 结束迭代器 
	@param beg2 容器 2 开始迭代器
	@param end2 容器 2 结束迭代器 
	@param dest 目标容器开始迭代器 
	@return 目标容器的最后一个元素的迭代器地址
*/
set_intersection(iterator beg1, iterator end1, 
                 iterator beg2, iterator e nd2, 
                 iterator dest)
```





### 10.2 *set_union* *算法*

>  需要事先分配地址 ∵并集 ∴分配  ` (v1.size() +  v2.size())`

```c++
/*
	set_union 算法 求两个 set 集合的并集
	注意:两个集合必须是有序序列 
	@param beg1 容器 1 开始迭代器 
	@param end1 容器 1 结束迭代器 
	@param beg2 容器 2 开始迭代器
	@param end2 容器 2 结束迭代器 
	@param dest 目标容器开始迭代器 
	@return 目标容器的最后一个元素的迭代器地址
*/
set_union(iterator beg1, iterator end1, 
          iterator beg2, iterator end2, 
          it erator dest)
```





### 10.3 *set_difference* *算法*

> 需要事先分配地址 ∵并集 ∴分配  ` (v1.size() -  v2.size())`

```c++
/*
	set_difference 算法 求两个 set 集合的差集
	注意:两个集合必须是有序序列 
	@param beg1 容器 1 开始迭代器 
	@param end1 容器 1 结束迭代器 
	@param beg2 容器 2 开始迭代器
	@param end2 容器 2 结束迭代器 
	@param dest 目标容器开始迭代器 
	@return 目标容器的最后一个元素的迭代器地址
*/
set_difference(iterator beg1, iterator end1, 
               iterator beg2, iterator end 2, 
               iterator dest)
```

