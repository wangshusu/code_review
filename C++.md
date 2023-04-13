# C++对C的提升

## 1. ::作用域

::解决归属问题 (谁是谁的谁)

### 1. 可以优先使用全局变量

```c++
using namespace std;
int a = 10;
void test01(){
    int a = 20;
    cout << "局部变量" << a << endl;
    cout << "全局变量" << ::a << endl;   //<----使用全局变量
}
```



## 2. namespace命名空间

使用关键字 namespace ,控制标名称的作用域
命名空间的本质:对 **符号常量**,**变量**,**函数**,**结构体**,**枚举**,**类和对象** 等等进行封装

示例:创建一个命名空间

```c++
namespace A{
    //定义变量,函数,类型等
    int data = 10;
}
```



### 2.1 命名空间只能定义在**全局**

```
//俺是全局变量哦~
int main(){
    namespace A{
        int b = 10;		//error
    }
}
```



### 2.2 命名空间可以嵌套

```c
namespace A{
    namespace B{
        int data = 10;
    }
}
ccout<<A::B::data<<endl; 
```



### 2.3 可以随时将新的成员加入命名空间

```c
namespace A{
    int a = 10;
}
namespace A{
    int b = 10;
}

int main(){
    cout << A::b << endl;
}
```



### 2.4 命名空间中,函数的实现和声明分开


```c++
namespace D{
    void func1();
    void func2();
}
void D::func1(){
    cout << "哈哈" << endl;
}
void D::func2(){
    cout << "呵呵" << endl;
}
```

### 2.5 无名的命名空间

  只能在 本源文件中使用 ==都是全局的==

  C++会把==不属于==任何名字空间的全局标识符(变量名, 全局函数,类型名...)

  放入到无名名字空间中去

```
//无名命名空间
namespace{
    int b = 10;
}
int a; // 会自动添加到 无名命名空间

void test01(void){
    ::a = 100;
    cout << "a = " << ::a << endl;
    cout << "a = " << a   << endl;
    cout << "b = " << ::b << endl;
}
```

### 2.6 命名空间取别名

```c
namespace A{
    int data1 = 10;
    int data2 = 20;
}
namespace B = A; //命名空间取别名
void test01(){
    cout << A::data1 << endl;
    cout << B::data1 << endl;
}
```



### 2.7 using声明

* 使用using声明命名空间 ==中== 的 ==某几个成员== 可用

  >  声明单个 : 会跟局部变量冲突
  >
  > 声明类 : 会跟全局变量冲突
  >
  > 声明函数重载 : 所有重载函数都可用

**using 申明某个成员 容易造成名字冲突**

```c
namespace A{
    int data1 = 10;
    int data2 = 20;
    int data2 = 30;
}

void test01(){
	// 只能说明 A::a 可以直接使用变量名 a 访问
    using A::a;
    int a = 100;//会和普通变量产生名字冲突  (局部变量)
    cout << "a = " << a << endl;
}
```

**using 申明定制成员函数 遇到函数重载**

```c
namespace B{
    void func01(void){
        cout << "B 中的 func01" << endl;
    }
    void func01(int a){
        cout << "B 中的 func02" << endl;
    }
    void func01(int a, int b){
        cout << "B 中的 func03" << endl;
    }
}

int main(int argc, char const *argv[])
{
    //函数名重载 命名空间的所有同名函数 都被声明可以使用
    using B::func01;

    func01();
    func01(10);
    func01(10,10);

    return 0;
}
```

**using 申明==整个命名空间== 可以直接通过成员名 使用**

```c
namespace A{
    int a = 10;
    int b = 20;
    int c = 30;
}
// int a = 100; //冲突
void test01(){
    // 使用整个命名空间
    // 不会和同名局部变量冲突 会跟全局同名变量冲突
    using namespace A;
    cout << "a = " << a << endl;
    int a = 100;
    // 变量a 现寻找局部变量 a ,如果找不到 再到申明的命名空间寻找
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
}
```



## 3. 增强类型

### 3.1 全局变量增强

```c
int a = 10;	//赋值 当做定义
int a;	    //没有赋值 当做声明  

int main(){
    printf("a = %d\n",a);
}
// 在C++下编译失败,在C下编译成功  更加严格的规范
```



### 3.2 C++的函数==形参==必须有类型

```c
//i可以没有类型,可以是任意类型
int func(i){
    printf("i = %d\n",i);
}
//i可以没有类型,可以是任意类型  但是 C++ 不允许
int func(i){
    printf("i = %s\n",i);
}
```



### 3.3 如果函数没有参数,建议写void

```c
void test01(void){
    cout << "hello" << endl;
}
```



### 3.4 更严格的类型转换

```C
enum COLOR{ GREEN, RED, YELLOW };
int main(){
    enmu COLOR num;
    num = 100; // C语言中 枚举变量允许赋其他 int 类型的值 C++不允许
	return 0;
}
```



### 3.5 **结构体类型增强**

```c
struct stu
{
	int num;
    char name[32];
    
    void func(void){
        cout << "stu 中的 func" << endl;
    }
};

void test01(){
    //C++可以不写struct  C语言必须写
    stu lucy = {100, "lucy"};
    cout << lucy.num << " " << lucy.name <<  " " << endl;
    lucy.func();
}
```



### 3.6 C++新增bool类型

* bool类型拥有两个值 true false



### 3.6 三目运算符增强

```c
void test05(){
    int a = 10;
    int b = 20;
    
    //C++三目运算符 返回值是引用 而c语言返回值是值
    a > b ? a : b = 100; // C++可以赋值 C语言不可以赋值
    cout << "a = " << a << " " << "b = " << b << endl;
}
```



## 4. cout  流操作算子

### 4.1 操作流算子概述

* cout  : console output 控制台输出
* clog ：表示标准错误，带缓冲的标准错误，用来输出错误信息的，类似于perror，但是perror不带缓冲
* cerr ：表示标准错误，不带缓冲的标准错误，用来输出错误信息的，和perror类似

![image-20220718184559173](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220718184614.png)

![image-20220718184750437](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220718184752.png)

![image-20220718184818470](C:/Users/%E9%98%BF%E4%B8%83/AppData/Roaming/Typora/typora-user-images/image-20220718184818470.png)

**流操作算子的使用方法**

使用这些算子的方法是将算子用 << 和 cout 连用。例如：

```c
cout << hex << 12 << "," << 24;
```

> 这条语句的作用是指定以十六进制形式输出后面两个数，因此输出结果是：c, 18



### 3.1 setiosflags() 算子

setiosflags() 算子实际上是一个**库函数**，它以一些标志作为参数，这些标志可以是在 iostream 头文件中定义的以下几种取值，它们的含义和同名算子一样

![image-20220718185123598](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220718185125.png)

**说明：**这些标志实际上都是仅有某比特位为 1，而其他比特位都为 0 的整数。

如果想用多个标志可以用 **`|`**运算符 连接，表示同时设置。例如：

```c
cout << setiosflags(ios::scientific|ios::showpos) << 12.34;
```

> 输出结果是：+1.234000e+001



如果两个相互矛盾的标志同时被设置，如先设置 setiosflags(ios::fixed)，然后又设置 setiosflags(ios::scientific)，那么结果可能就是两个标志都不起作用。因此，在设置了某标志，又要设置其他与之矛盾的标志时，就应该用 resetiosflags 清除原先的标志。例如下面三条语句：

```c
cout << setiosflags(ios::fixed) << 12.34 << endl;
cout << resetiosflags(ios::fixed) << setiosflags(ios::scientific | ios::showpos) << 12.34 << endl;
cout << resetiosflags(ios::showpos) << 12.34 << endl;  //清除要输出正号的标志
```

> 输出结果是：
> 12.340000
> +1.234000e+001
> 1.234000e+001

### 3.3 综合示例

```c
#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    int n = 141;
    //1) 分别以十六进制、十进制、八进制先后输出 n
    cout << "1)" << hex << n << " " << dec << n << " " << oct << n << endl;
    double x = 1234567.89, y = 12.34567;
    //2)保留5位有效数字
    cout << "2)" << setprecision(5) << x << " " << y << " " << endl;
    //3)保留小数点后面5位
    cout << "3)" << fixed << setprecision(5) << x << " " << y << endl;
    //4)科学计数法输出，且保留小数点后面5位
    cout << "4)" << scientific << setprecision(5) << x << " " << y << endl;
    //5)非负数显示正号，输出宽度为12字符，宽度不足则用 * 填补
    cout << "5)" << showpos << fixed << setw(12) << setfill('*') << 12.1 << endl;
    //6)非负数不显示正号，输出宽度为12字符，宽度不足则右边用填充字符填充
    cout << "6)" << noshowpos << setw(12) << left << 12.1 << endl;
    //7)输出宽度为 12 字符，宽度不足则左边用填充字符填充
    cout << "7)" << setw(12) << right << 12.1 << endl;
    //8)宽度不足时，负号和数值分列左右，中间用填充字符填充
    cout << "8)" << setw(12) << internal << -12.1 << endl;
    cout << "9)" << 12.1 << endl;
    return 0;
}
```

>程序的输出结果是：
>1)8d 141 215
>2)1.2346e+06 12.346
>3)1234567.89000 12.34567
>4)1.23457e+06 1.23457e+01
>5)***+12.10000
>6)12.10000****
>7)****12.10000
>8)-***12.10000
>9)12.10000



### 3.4 setw() 算子注意事项

需要注意的是，**setw() 算子**所起的作用是**一次性**的，即**只影响下一次输出**。每次需要指定输出宽度时都要使用 setw()。因此可以看到，第 9) 行的输出因为没有使用 setw()，输出的宽度就不再是前面指定的 12 个字符。

 在读入字符串时，setw() 还能影响 cin 的行为。例如下面的程序：

```c

#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    string s1, s2;
    cin >> setw(4) >> s1 >> setw(3) >> s2;
    cout << s1 << "," << s2 << endl;
    return 0;
}
```

>输入：
>1234567890↙
>
>程序的输出结果是：
>1234,567

 **说明：**`setw(4)`使得读入 s1 时，只读入 4 个字符，其后的`setw(3)`使得读入 s2 时只读入 3 个字符。
      setw() 用于 cin 时，同样只影响下一次的输入。

### 3.5 调用cout的成员函数

ostream 类有一些成员函数，通过 cout 调用它们也能用于控制输出的格式，其作用和流操纵算子相同，如表 3 所示。

![image-20220718190153509](C:/Users/%E9%98%BF%E4%B8%83/AppData/Roaming/Typora/typora-user-images/image-20220718190153509.png)

**说明：**setf 和 unsetf 函数用到的`flag`，与 setiosflags 和 resetiosflags 用到的完全相同。

这些成员函数的用法十分简单。例如下面的三行程序：

```c++
cout.setf(ios::scientific);
cout.precision(8);
cout << 12.23 << endl;
```

> 输出结果是：1.22300000e+001



## 5. C++中的const(重要)

* 如果以 ==常量== 初始化const修饰的变量 编译器会将变量的 ==值== 放入==符号常量表==中  不会 ==立即== 给变量开空间

* 只有对 a ==取地址==时 , 编译器才会给 a 开空间 (只读变量)

* 如果以 ==变量== 初始化const修饰的只读变量, ==没有== 符号常量表,立即开辟空间

  ```c
  int a = 10;
  const int c = a;
  ```

* 如果以 const 修饰的是 ==自定义类型== 的变量 , 也不会有符号常量表 , 立即开辟空间 

## 6. 引用

**引用的定义:**

​	1.本质就是给对象取一个别名

​	2.引用定义的步骤:

```c
&别名
//给哪个变量取别名 就定义该变量
//从上往下整体替换
//类型&(期望的名字) = (原来的名字)
```

### 6.1 案例1:给普通变量取别名

```c
void test01(void){
    int a = 10;
    //在定义的时候 &修饰变量名为引用 b就是a的变量名
    //系统不会为引用  开辟独立空间
    int &b = a; //引用必须初始化
}
```

### 6.2 案例2:给数组取别名

```c++
void test02(){
    int arr[5] = {10,20,30,40,50};
    
    int (&myarr)[5] = arr;
    myarr[4] = 100;
    for (int i = 0; i < 5; i++)
    {
        cout << arr[i] << " " ;
    }
    cout << endl;
}
```

### 6.3 案例3:给指针变量取别名

```c++
void test03(){
    int num = 10;
    int *p = &num;

    int *&myp = p;
    cout << "*p   = " << *p << endl;
    cout << "*myp = " << *myp << endl;
}
```

### 6.4 案例4:给函数取别名

```c
void func01(void){
    cout << "func" << endl;
}
void test01(){
    void (&my_func)(void) = func01;
    my_func();
}
```

* 引用可以作为函数的参数 

​     函数内部可以 通过 ==引用== 操作外部变量    <节约空间>

### 6.5 案例5:引用作为函数体的返回值类型 (链式操作)

```c
int &getData(void)
{
    static int num = 10;
    //不要返回局部变量的引用
    return num; // 返回num 外界就是给num取别名
}
void test04()
{
    //b 就是别名;
    int &b = getData();
    cout << "b = " << b << endl;
}
```

链式操作:

```c++
struct stu
{
    stu& printStu(stu &ob, int value)
    {
        cout << value << " ";
        return ob;
    }
};

void test05()
{
    stu ob1;
    ob1.printStu(ob1, 100).printStu(ob1, 200).printStu(ob1, 300);
}
```

* 引用的本质 常量指针

## 7.内联函数

内联函数：

* 在==编译阶段== 将内联函数中的==函数体== 替换函数==调用处==。避免函数调用时的开销。 
* 必须在==定义==的时候 使用关键字 inline 修饰， 不能在声明的时候使用 inline  

```c
//函數声明的时候 不要使用inline
int my_add(int x, int y);
void test01()
{
    cout<<my_add(100,200)<<endl;
}
//内联函数 在定义的时候使用inline
inline int my_add(int x, int y)
{
    return x+y;
}
```

节省调用函数的开销,提高程序执行的效率
由于内联函数的函数体会拷贝到多个调用点,会增加可执行文件的大小
所以,内联函数的思想就是用 "空间" 换 "时间"

## 8. 宏函数和内联函数的区别（重要背）

* 宏函数和内联函数 都会在适当的位置 进行展开 **避免函数调用开销**。 


* 宏函数的参数**没有类型**，不能保证参数的完整性。 
* 内联函数的参数**有类型,**  能保证参数的完整性。  


* 宏函数在**预处理阶段**展开 
* 内联函数在**编译阶段**展开

* 宏函数**没有作用域**的限制，不能作为命名空间、结构体、类的成员  
* 内联函数**有作用域**的限制，能作为命名空间、结构体、类的成员 

**内联函数的注意事项:**

* 在内联函数定义的时候加 inline 修饰  
* 类中的成员函数 默认都是内联函数（不加inline 也是内联函数）
* 有时候 就算加上inline也不一定是内联函数（==内联函数条件==） 
  * 不能存在**任何形式**的循环语句  
  * 不能存在**过多**的条件判断语句  
  * 函数体不能**过于庞大** 
  * 不能对函数**取地址**
  * 有时候不加inline修饰 也有可能是内联函数。 
* 内不内联 由编译器决定。  



## 9. 函数重载(重要)

函数重载 是c++的**多态**的特性（静态多态）。 

函数重载：用**同一个函数名** 代表**不同的函数功能 **

* 1、函数重载的条件：（背） 

  **同一作用域**，函数的参数**类型**、**个数**、**顺序**不同 都可以重载。（**返回值类型**不能作为重载的条件）

```c++
void printFun(int a)
{
    cout<<"int"<<endl;
}
void printFun(int a, char b) 
{
    cout<<"int char"<<endl;
}
void test02()
{
    printFun(10);
    printFun(10, 'a');
}
```

c++中 不能直接将函数名作为函数的入口地址（为啥呢?）  

**函数名和参数 共同决定函数的入口地址 **

函数重载的底层实现:

![image-20220718173534091](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220718173536.png)

```c
先将源文件编译成二进制文件
    g++ -c 源文件 -o xxx.o 
    如：
    g++ -c 06换名机制.cpp -o 06换名机制.

 使用 nm 命令去查看该二进制文件中的符号表，查看编译过后的函数名和编译器的有什么区别
    如：
    nm 06换名机制.o
```

> 

## 10. 函数的缺省参数(重要)

​	在函数声明处 给函数参数一个默认的值，如果函数调用处，用户没用传实参，编译器就可以使用 这个**默认**的值。

```c
//建议 在函数声明处 给出缺省值
int my_sub(int x=100, int y=200);
void test01()
{
    cout << my_sub(200, 50) << endl;
    cout << my_sub() << endl;
}
int my_sub(int x , int y)
{
    return x-y;
}
```

**注意点:**如果函数的**某个参数**设置为**默认参数**， 那么这个参数的**右边的所有参数** 都必须是**默认参数**。 



## 11. 占位参数

* 没有形参名的形参 叫占位参数

```c
void myFun(int x)
{
    cout << "A:int x = " << endl;
}
void myFun(int x , int )
{
    cout << "B:int x = " << endl;
}
void test01()
{
    myFun(10);
    myFun(10 , 20);
}
```

* 占位参数也可以是缺省值

```c
void myFun(int x , int=10 )
{
    cout << "B:int x = " << endl;
}
void test01()
{
    myFun(10);
    myFun(10 , 20);
}
```

 ↑  默认参数和函数重载同时出现 一定要注意二义性



# 类和对象 1.0

## 1.类的概述

类将数据和方法封装在一起，加以权限区分，用户只能通过公共方法 访问私有数据。 

### 1.1 定义一个类 关键字 class

* 类的权限分为：private、protected、public。
* 但是在类的内部 不存在 权限之分。只是对 类外有效。
* 如果类不涉及到继承，private、protected没有区别，都是私有属性。

![image-20220718200555009](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220718200555.png)

建议:
	将数据设置为私有,将方法设置为公有.

## 2. 成员函数在类外实现

```c++
 class Data2
 {
private:
     int mA;
public:
     void setA(int a);
     int getA(void);
 }
void Data2::setA(int a)
{
    mA = a;
}
int Data2::getA(void)
{
    return mA;
}
```

也可以这样 : 头文件定义类， cpp 实现类的成员函数 

## 3. 构造函数

### 3.1 **构造函数的概述**

* 类实例化对象的时候 系统自动调用构造函数 完成对象的初始化。 
* 如果用户不提供构造函数 编译器 会自动添加一个默认的构造函数（空函数） 

### 3.2 构造函数的定义（重要,公有方式创建)

* 构造函数名 和 类名相同，**没有返回值类型**（连void都不可以），**可以有参数**（可以重载） 
* **先**给对象**开辟空间**（实例化） 然**后调用构造函数**（初始化）

一个有趣的发现 : 结构体中也是可以有构造函数的

## 4. 析构函数 (重要)

* 当对象生命周期结束的时候 系统自动调用析构函数。 
* 函数名和类名称相同，在函数名前加 ==~== ，没有返回值类型，没有函数形参。（**不能被重载,不能抛出异常**） 
* **先**调用**析构函数** **再释放**对象的**空间**。 

### 4.1 有指针成员的析构函数

一般情况下，空的析构函数就足够。但是如果一个类有指针成员，这个类必须 写析构函 数，释放指针成员所指向空间。 

```c
Data4(const Data4 &ob)//ob就是旧对象的别名
{
    //一旦实现 了拷贝构造 必须完成赋值动作
    mA = ob.mA;
    cout<<"拷贝构造函数"<<endl;
}
```

```c
class Data4
{
public:
    int mA;
public:
#if 1
    Data4()
    {
        mA = 0;
        cout<<"无参构造 mA = "<<mA<<endl;
    }
#endif
#if 1
    Data4(int a)
    {
        mA = a;
        cout<<"有参构造 mA = "<<mA<<endl;
    }
#endif
#if 1
    Data4(const Data4 &ob)//ob就是旧对象的别名
    {
        //一旦实现 了拷贝构造 必须完成赋值动作
        mA = ob.mA;
        cout<<"拷贝构造函数"<<endl;
    }
#endif
    ~Data4()
    {
        cout<<"析构函数 mA = "<<mA<<endl;
    }
};
Data4 getObject(void)
{
    Data4 ob1(10);
    cout << "A:" << &ob1 << endl;
    return ob1;
}

void test04()
{
    Data4 ob2 = getObject();
    cout << "B:" << &ob2 << endl;
}
```



![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719144443.png)



## 5. 拷贝构造函数

### 5.1 拷贝构造的定义

* 拷贝构造：本质是构造函数
* 拷贝构造的调用时机：旧对象 初始化 新对象 才会调用拷贝构造。
* 如果用户不提供拷贝构造 编译器会自动提供一个默认的拷贝构造（完成赋值动作--浅拷贝）

### 5.2 拷贝构造 和 无参构造 有参构造的关系

* 如果用户**定义了 拷贝构造**或者**有参构造** 都会**屏蔽无参构造**。 
* 如果用户**定义了 无参构造**或者**有参构**造 不会**屏蔽拷贝构造**。 

### 5.3 拷贝构造几种调用形式（了解）

* **旧对象给新对象初始化 调用拷贝构造** 

```c
Data4 ob1(10);
Data4 ob2 = ob1;//调用拷贝构造
```

* **给对象取别名 不会调用拷贝构造**

```c
Data4 ob1(10);
Data4 &ob2 = ob1;//不会调用拷贝构造
```

* **普通对象作为函数参数 调用函数时 会发生拷贝构造** 

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719145543.png)

* **函数返回值 普通对象（Visual Studio会发生拷贝构造）（Qtcreater,linux不会发生）**

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719145737.png)



## 6. 浅拷贝和深拷贝(重要)

* 默认的拷贝构造 都是**浅拷贝**。 
* 如果类中**没有指针成员**， 不用实现**拷贝构造**和**析构函数**。 
* 如果类中**有指针成员**， 必须实现**析构函数**释放指针成员指向的堆区空间，必须**实现拷贝构造**完成**深拷贝**动作。

```c++
class Data5
{
public:
    char *name;
public:
    Data5()
    {
        name=NULL;
    }
    Data5(char *str)
    {
            name = (char *)calloc(1, strlen(str)+1);
        strcpy(name, str);
        cout<<"有参构造 name="<<name<<endl;
    }
    ~Data5()
    {
        cout<<"析构函数name = "<<name<<endl;
        if(name != NULL)
        {
            free(name);
            name=NULL;
        }
    }
};
void test05()
{
    Data5 ob1("hello world\n");
    Data5 ob2 = ob1;
}

```



## 7. 初始化列表(重要)

* 一个类的对象 作为另一个类的成员：**成员对象 **
* 如果类中想调用**成员对象**的**有参**构造 必须使用**初始化列表**。 

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719151030.png)

* 类会**自动调用**成员对象的**无参**构造。 
* 类想调用成员对象 **有参构造** 必须使用**初始化列表**。



**还有一些必须使用初始化列表的场合 : **

* 1.成员变量是**常量** 
* 2.成员变量是**引用**(后面会讲，因为引用在定义的时候***必须初始化**) 
* 3.成员对象**没有默认构造函数**(无参构造函数) √
* 4.想要**调用**成员对象的某个**有参构造**函数 √
* 5.初始化**基类**成员(继承 -->初始化父类) √



案例:

```c++
B(int a, int b):mB(b),ob(a) //不分顺序
{
    
}
```

```c++
class A
{
public:
    int mA;
public:
    A()
    {
        mA = 0;
        cout<<"A的无参构造 mA="<<mA<<endl;
    }
    explicit A(int a)
    {
        mA = a;
        cout<<"A的有参构造mA="<<mA<<endl;
    }
    ~A()
    {
        cout<<"A的析构函数 mA = "<<mA<<endl;
    }
};
```

```c++
class B
{
public:
    int mB;
    A ob;//成员对象
public:
    B()
    {
        cout<<"B类的无参构造"<<endl;
    }
    //初始化列表 成员对象 必须使用对象名+（）
    //B(int a, int b):ob(a)
    B(int a, int b):mB(b),ob(a)
    {
        cout<<"B类的有参构造"<<endl;
    }
    ~B()
    {
       cout<<"B的析构函数"<<endl;
    }
};
```

```c++
void test01()
{
    B ob1(10,20);
    cout<<"mA ="<<ob1.ob.mA<<", mB ="<<ob1.mB<<endl;
}
```

结果:

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719152644.png)



## 8. 对象数组 (重要)

**对象数组：**本质是数组 数组的每个元素是对象 

```c++
 class A
{
public:
    int mA;
public:
    A()
    {
        mA = 0;
        cout<<"A的无参构造 mA="<<mA<<endl;
    }
    explicit A(int a)
    {
        mA = a;
        cout<<"A的有参构造mA="<<mA<<endl;
    }
    ~A()
    {
        cout<<"A的析构函数 mA = "<<mA<<endl;
    }
};
```

```c++
void test02()
{
    //对象数组 每个元素都会自动调用构造和析构函数
    //对象数组不初始化  每个元素 调用无参构造
    A arr1[5];

    //对象数组的初始化 必须显示使用有参构造 逐个元素初始化
    A arr2[5]={A(10),A(20),A(30),A(40),A(50) };
    int n =sizeof(arr2)/sizeof(arr2[0]);
    int i=0;
    for(i=0;i<n;i++)
    {
        cout<<arr2[i].mA<<" ";
    }
    cout<<endl;
}
```

可以尝试跑一下看看



## 9. explicit 关键字

* explicit 防止 **构造函数隐式转换 **
* explicit **修饰构造函数 **

```c++
//允许有参构造 隐式转换 
A(int a)
{ 
    mA = a;
    cout<<"A的有参构造mA="<<mA<<endl;
}
```

```c++
//构造函数隐式转换（类中只有一个数据成员）
A ob1=100;//ok
```

```c++
//防止有参构造 隐式转换
explicit A(int a)
{ 
	mA = a;
	cout<<"A的有参构造mA="<<mA<<endl;
}

//构造函数隐式转换（类中只有一个数据成员）
 A ob1=100;//err 转换失败
```



# 类和对象 2.0



## 1. new和delete

### 1.1 new和delete操作==基本类型==的空间

**new 和 malloc delete和free 没有区别** 

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719161743.png)

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719161807.png)

**区别：**

* new/delete**是关键字**，**是运算符**，需要编译器支持 , malloc free 是库函数  需要头文件的支持
* 使用new运算符申请空间的时候无需指定内存大小, 编译器会自动计算,malloc则需要显示地指定所需内存的大小

* new 不用强制类型转换 

* new 在申请空间的时候可以 初始化空间内容 

  * new 分配空间 调用构造函数		malloc 分配空间
  * delete 调用析构函数 释放内存     free 释放内存

* new 可以进行运算符重载 , 从而控制动态内存的分配

* new 操作符是从自由存储区上为对象分配空间 malloc 函数是从堆区为变量分配空间

  * 自由存储区 是C++基于new操作的一个抽象概念，凡是通过new操作符分配的空间 都称之为自由存储区
  * 堆是操作系统的术语,是操作空间所维护的一块特殊的内存,用于程序的内存分配

* new 分配内存失败时,会抛出一个 bac_alloc 的异常值                                                    

  malloc 分配内存失败时, 返回一个NULL;65

  

 **new 申请基本类型的数组**

```c++
void test02()
{
    int *arr = NULL;

    //申请数组 5个int元素
    //arr = new int[5];//堆区空间
    arr = new int[5]{10,20,30,40,50};
    int i=0;
    for(i=0;i<5;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;

    //如果new有[]  delete就必须有[]
    delete [] arr;
}
```



### 1.2new和delete操作类的空间

* malloc不会调用构造函数 free不会调用析构函数
* new 会调用构造函数 delete调用析构函数

```c++
class A
{
public:
    int mA;
public:
    A()
    {
        cout<<"无参构造"<<endl;
    }
    A(int a)
    {
        mA = a;
        cout<<"有参构造 mA="<<mA<<endl;
    }
    ~A()
    {
        cout<<"析构函数 mA="<<mA<<endl;
    }
};
```

```c++
void test03()
{
#if 0  //malloc free
    A *p = (A *)malloc(sizeof(A));

    p->mA = 100;
    cout<<p->mA<<endl;

    free(p);

#endif

#if 1  // new 调用有参构造
    A *p = new A(100);

    //p->mA = 100;
    cout<<p->mA<<endl;

    delete p;
#endif
}
```



### 1.3 new申请对象数组

```c++
void test04()
{
    //每个元素 默认调用无参构造
    //A *arr = new A[5];

    A *arr = new A[5]{A(10), A(20),A(30),A(40),A(50)};

    delete  [] arr;
}
```



## 2. 静态成员 (重要)

### 2.1 概念的引入

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719191402.png)

* 类的对象 拥有独立的 普通成员数据。 
* static 修饰的成员 叫 静态成员。

```c++
class Data
{
    static int a;//静态成员数据
    static void func()//静态成员函数
    {
        
    }
}
```



### 2.2 静态成员数据

* static修饰的静态成员 属于**类**而不是**对象**。（所有对象 **共享** 一份 静态成员数据）

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719191632.png)

* static修饰的成员 定义类的时候 必须**分配空间**。 
* static修饰的静态成员数据 必须**类中**定义 **类外**初始化。 

```c++
class Data
{
public:
    int a;//普通成员数据
    //类中定义
    static int b;//静态成员数据
};
//类外初始化
int Data::b=100;

void test01()
{
    //静态成员数据 通过类名称直接访问（属于类）
    cout<<Data::b<<endl;

    //静态成员数据 通过对象访问(共享)
    Data ob1;
    cout<<ob1.b<<endl;

    ob1.b = 200;
    Data ob2;
    ob2.b = 300;
    cout<<Data::b<<endl;//300
}
```

案例:**使用静态成员数据 统计对象的个数** 

```c++
class Data2
{
public:
    int mA;
    static int count;
public:
    Data2()//无参构造
    {
        count++;
    }
    Data2(int a)//有参构造
    {
        mA = a;
        count++;
    }

    Data2(const Data2 &ob)//有参构造
    {
        count++;
    }
    ~Data2()//析构函数
    {
       count--;
    }
};
int Data2::count=0;

void test02()
{
    Data2 ob1;
    Data2 ob2(10);
    Data2 ob3 = ob2;
    cout<<"对象个数:"<<Data2::count<<endl;//3
    {
        Data2 ob4;
        Data2 ob5;
        cout<<"对象个数:"<<Data2::count<<endl;//5
    }
    cout<<"对象个数:"<<Data2::count<<endl;//3
}
```

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719192050.png)



### 2.3 静态成员函数

静态成员函数 是属于**类** 而不是对象（所有对象 **共享**） 

```c++
class Data
{
    static void func()
    {
        
    }
}
```



* 静态成员函数 可以**直接通过类名称访问**
* **静态成员函数**内 **只能**操作**静态成员数据**

```c++
class Data3
{
private:
    int data;
    static int a;
public:
    static int getA()
    {
        data = 10;//err 静态成员函数 只能操作 静态成员数据 而data为普通成员数据
        return a;
    }
};
int Data3::a = 100;
void test03()
{
    cout<<Data3::getA()<<endl;
}
```



## 3. 单例模式 (重要)

* **单例模式**的类 只能实例化**一个**对象
* 重要步骤：将构造函数**私有化** 

```c++
class SingleTon//单例模式
{
    //构造私有化 防止实例化其他对象
private:
    SingleTon(){
        count=0;
        cout<<"构造"<<endl;
    }
    SingleTon(const SingleTon &ob){
        count=0;
    }
    ~SingleTon()
    {
        cout<<"析够"<<endl;
    }
private:
    //const防止p 在类内部 被修改指向
    static SingleTon * const p;//保存唯一的实例地址
    int count;//统计任务执行次数
public:
    static SingleTon * getSingleTon(void)//获取唯一的实例地址
    {
        return p;
    }

    //用户自定义 任务函数
    void printString(char *str)
    {
        count++;
        cout<<"当前第"<<count<<"次任务打印:"<<str<<endl;
    }

};
SingleTon *const SingleTon::p = new SingleTon;//创建唯一的实例

int main(int argc, char *argv[])
{
    //获取单例的地址
    SingleTon *p1 =SingleTon::getSingleTon();
    p1->printString("离职证明1");
    p1->printString("学历证明1");
    p1->printString("学位证明1");
    p1->printString("身份证明1");

    SingleTon *p2 =SingleTon::getSingleTon();
    p2->printString("离职证明2");
    p2->printString("学历证明2");
    p2->printString("学位证明2");
    p2->printString("身份证明2");
    return 0;
}
```



## 4. 类的存储结构

* **成员函数** **静态成员** 不占类的**空间**



![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719205458.png)



* 成员函数、静态成员 是独立存储 是所有对象**共享**。 



## 5. this指针 (了解)

### 5.1 this指针引入

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719205626.png)

### 5.2 普通成员函数的this指针

* 普通成员函数 默认有一个 this 指针 指向**调用改成员函数的** **对象**

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719205912.png)



### 5.3 this来完成链式操作

```c++
class Data1
{
public:
    Data1& myPrintf(char *str)
    {
        cout<<str<<" ";
        return *this;//返回调用该成员函数的对象
    }
};
void test01()
{
    Data1().myPrintf("hehe").myPrintf("haha").myPrintf("xixix");
}
```



## 6.const修饰成员函数

* const 修饰成员函数为 **只读**（该成员函数不允许对 成员数据 赋值） 

  但 **mutable** 修饰的成员除外 

  ```c++
  void showData(void) const
  {
      
  }
  ```

  

```c++
class Data2
{
public:
    int a;
    int b;
    mutable int c;
public:
    Data2(int a, int b,int c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    //const 修饰成员函数为只读（该成员函数不允许对 成员数据 赋值） 
    //mutable修饰的成员除外
    void showData(void) const
    {
        //a = 100;//err
        c=100;
        cout<<a<<" "<<b<<" "<<c<<endl;
    }
};
void test02()
{
    Data2 ob1(10,20,30);
    ob1.showData();
}
```



## 7. 友元 (重要)

* 类将 **数据**和 **方法** 封装在一起 加以**权限**区分 用户只能通过**公共方法** 操作私有数据。（封装性）
* 友元 重要用在**运算符重载**上
* 一个**函数**或者**类** 作为了另一个类的友元 那么这个函数或类 就可以**直接访问** 另一个类的**私有数据**

### 7.1 普通函数 友元

```c++
class Room
{
    friend void visiting01(Room &room);
private:
    string bedRoom;//卧室
public:
    string setingRoom;//客厅
public:
    Room(string bedRoom, string setingRoom)
    {
        this->bedRoom = bedRoom;
        this->setingRoom = setingRoom;
    }
};

//普通全局函数
void visiting01(Room &room)
{
    cout<<"访问了"<<room.setingRoom<<endl;
    cout<<"访问了"<<room.bedRoom<<endl;
}

int main(int argc, char *argv[])
{
    Room room("刘坤卧室","刘坤客厅");
    visiting01(room);
    return 0;
}
```

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220719210852.png)



### 7.2 类的 某个成员函数 作友元

> 这里有一个声明的问题  向前声明
>
> 这里在解决两个类要声明友元(相当于相互包含 成员函数)
>
> 采用 成员函数类外实现 + 向前声明 的方法 解决问题

```c++
class Room;//向前声明 只能说明类名称
class goodGay
{
public:
    void visiting01(Room &room);
    void visiting02(Room &room);
};

class Room
{
    friend void goodGay::visiting02(Room &room);
private:
    string bedRoom;//卧室
public:
    string setingRoom;//客厅
public:
    Room(string bedRoom, string setingRoom)
    {
        this->bedRoom = bedRoom;
        this->setingRoom = setingRoom;
    }
};

int main(int argc, char *argv[])
{
    Room room("吴维的卧室","吴维的客厅");
    goodGay ob;
    ob.visiting01(room);
    ob.visiting02(room);
    return 0;
}

void goodGay::visiting01(Room &room)
{
    cout<<"翰文访问了"<<room.setingRoom<<endl;
    //cout<<"翰文访问了"<<room.bedRoom<<endl;
}

void goodGay::visiting02(Room &room)
{
    cout<<"好基友张三访问了"<<room.setingRoom<<endl;
    cout<<"好基友张三访问了"<<room.bedRoom<<endl;
}
```

测试一下  代码没跑的  



### 7.3 整个类作为友元

```c++
class Room;//向前声明 只能说明类名称
class goodGay
{
public:
    void visiting01(Room &room);
    void visiting02(Room &room);
};

class Room
{
    friend class goodGay;
private:
    string bedRoom;//卧室
public:
    string setingRoom;//客厅
public:
    Room(string bedRoom, string setingRoom)
    {
        this->bedRoom = bedRoom;
        this->setingRoom = setingRoom;
    }
};

int main(int argc, char *argv[])
{
    Room room("吴维的卧室","吴维的客厅");
    goodGay ob;
    ob.visiting01(room);
    ob.visiting02(room);
    return 0;
}

void goodGay::visiting01(Room &room)
{
    cout<<"翰文访问了"<<room.setingRoom<<endl;
    cout<<"翰文访问了"<<room.bedRoom<<endl;
}

void goodGay::visiting02(Room &room)
{
    cout<<"好基友张三访问了"<<room.setingRoom<<endl;
    cout<<"好基友张三访问了"<<room.bedRoom<<endl;
}

```



## 8. 设计动态数组类

![](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220720105124.png)

arry.h
```c++
class Array
{
private:
    int *arr;//存放首元素地址
    int capacity;//容量
    int size;//大小
public:
    Array();
    Array(int capacity);
    Array(const Array &ob);
    ~Array();

    int getCapacity() const;
    int getSize() const;

    void printArray(void);

    //插入尾部元素
    void pushBack(int elem);
    //删除尾部元素
    void popBack(void);
    int &at(int pos);
};
```



arry.c

```c++
int Array::getCapacity() const
{
    return capacity;
}

int Array::getSize() const
{
    return size;
}

void Array::printArray()
{
    int i=0;
    for(i=0;i<size; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    return;
}

void Array::pushBack(int elem)
{
    //判断容器是否满
    if(size == capacity)
    {
        //申请空间
        int *tmp = new int[2*capacity];
        //将就空间的内容 拷贝到新空间
        memcpy(tmp, arr, capacity*sizeof(int));
        //释放原有的空间
        delete [] arr;
        //更新arr的空间
        arr = tmp;
        //更新容量
        capacity = 2*capacity;
    }

    arr[size]=elem;
    size++;
    return;
}

void Array::popBack()
{
    if(size == 0)
    {
        cout<<"容量为空"<<endl;
    }
    else
    {
        size--;
    }
    return;
}

int& Array::at(int pos)
{
    if(pos<0 || pos >=size)
    {
        cout<<"访问违法内存"<<endl;
        exit(-1);
    }

    return arr[pos];
}

Array::Array()
{
    capacity = 5;
    size = 0;
    arr = new int[capacity];
    //空间清0
    memset(arr, 0, sizeof(int)*capacity);
}

Array::Array(int capacity)
{
    this->capacity = capacity;
    size = 0;
    arr  = new int[capacity];
    //空间清0
    memset(arr, 0, sizeof(int)*capacity);
}

Array::Array(const Array &ob)
{
    capacity = ob.capacity;
    size = ob.size;
    //深拷贝
    arr = new int[capacity];
    memcpy(arr, ob.arr, sizeof(int)*capacity);
}

Array::~Array()
{
    if(arr != NULL)
    {
        delete [] arr;
        arr = NULL;
    }
}
```



# 类和对象 3.0



## 1.重载运算符(重要)

* 运算符重载 是对已有的运算符 指定新功能。不能创建新运算。 

* 运算符重载关键字 **operator**

**思路 :**  ==↓↓↓↓↓↓↓ 重要 ↓↓↓↓↓↓↓==

* 先弄懂运算符的**运算对象的个数**。（个数决定了 重载函数的**参数个数**）
* 识别运算符**左边**的运算对象 是**类的对象** 还是**其他**. 
  * **类的对象 ：** **全局函数**实现 **成员函数**实现（**推荐，少一个参数**）
  * **其他：** 只能是全局函数实现

### 1.1 重载 << >> 运算符

* 关于 << 函数 一般使用都是在输出 cout <<  之中 可以见到 这里的 << 的左边的运算对象 不是类的对象 所以这里采用全局函数实现 

```c++
class Person
{
    friend ostream & operator<<(ostream &out, Person &p);
    friend istream & operator>>(istream &in, Person &p);
private:
    string name;
    int age;
    int score;
public:
    Person();
    Person(string name, int age, int score);
};

Person::Person()//默认构造函数
{
    this->age = 0;
    this->name = "good";
    this->score = 0;
}

Person::Person(string name, int age, int score)//有参构造函数
{
    this->age = age;
    this->name = name;
    this->score = score;
}

//全局修改 << 运算符
ostream & operator<<(ostream &out, Person &p)
{
    out << p.name << " ";
    out << p.age  << " ";
    out << p.score<< endl;
    return out;
}

//全局修改 >> 运算符
istream & operator>>(istream &in, Person &p)
{
    in >> p.name >> p.age >> p.score;
    return in;
}

int main(int argc, char const *argv[])
{
    Person p1("lucy", 22, 100);
    Person p2;
    Person p3;
    cin >> p2 >> p3;
    cout << "----------------" << endl;
    cout << p1 << p2 << p3 << endl;
    return 0;
}
```

> 全局重载 >> << 符号,如果输入输出的值在类中属于私有成员,那么就要在类中声明为友元函数, 这里也就是体现了友元的意义.



### 1.2 重载 + -

* +- 运算符 左右两边都是 类的对象 所以这里推荐使用**成员函数**来实现
* 因为自身的调用就已经具备了(this)一个参数

```c++
//这里直接借用上面已经写好了的 << >> 函数
class Person
{
    friend ostream & operator<<(ostream &out, Person &p);
    friend istream & operator>>(istream &in, Person &p);
private:
    string name;
    int age;
    int score;
public:
    Person();
    Person(string name, int age, int score);
    Person operator+(Person &ob);
    //Person operator-(Person &ob);
    //这里的 - 号的重载因为里面的名字的关系 不好进行修改  
    //但是基本的实现方式跟 + 的一样
};

Person Person::operator+(Person &ob)
{
    this->age = this->age + ob.age;
    this->name = this->name + ob.name;
    this->score = this->score +ob.score;
    return *this;
}

int main(int argc, char const *argv[])
{
    Person p1("lucy", 22, 100);
    Person p2("bob", 21, 99);
    Person p3 = p1 + p2 + p2;
    cout << p3;
}
```



### 1.3 重载 ++ 运算符

注意:编译器是如何区分 前置++ 和 后置++ 的呢?

* ++a(前置++)，它就调用operator++(a), 
* a++（后置++），它就会去调用operator++(a,int)

采用了一个占位参数 int 进行区分

**重载后置++**

```c++
类名称 operator++(int)
{
    //先保存旧的值
    //自增++
    return old;//返回旧的值
}
```

**重载前置++**

```c++
类名称 operator++(int)
{
    //自增++
    return *this;//直接返回自增后的值
}
```

下面实现一下这两种运算符的重载 :

```c++
//这里直接借用上面已经写好了的 << >> 函数
class Person
{
    friend ostream & operator<<(ostream &out, Person &p);
    friend istream & operator>>(istream &in, Person &p);
private:
    string name;
    int age;
    int score;
public:
    Person();
    Person(Person &ob);//这里是个问题
    Person(string name, int age, int score);
    Person operator++();
    Person operator++(int);
};

Person Person::operator++()
{
    this->age = this->age + this->age;
    this->name = this->name + this->name;
    this->score = this->score + this->score;
    return *this;
}

Person Person::operator++(int)
{
    Person tmp = *this;
    this->age = this->age + this->age;
    this->name = this->name + this->name;
    this->score = this->score + this->score;
    return tmp;
}

int main(int argc, char const *argv[])
{
    Person p1("lucy", 22, 100);
    Person p2(p1);
    p1++; // 后置++
    cout << p1 ;
    cout << p2 ;
    cout << "--------------" << endl;
    ++p2; // 前置++
    cout << p2;
}

```



### 1.4 重载 == 运算符

```c++
bool Person::operator==(Person &ob)
{
    if (this->name == ob.name &&
        this->age == ob.age &&
        this->socre == ob.socre)
    	return ture;
    else
        return false;
}
```

![image-20220720192137130](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220720192137.png)



## 2. 重载运算符 ()

> 重载（）运算符 一般用于 为算法 提供策略。

![image-20220720192743404](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220720192743.png)



## 3.智能指针

>  智能指针：解决 堆区空间的对象 释放问题 
>
> 重载* 运算符
>
> 重载 -> 运算符
>
> 因为调用了一层类 依照类的生存空间 类消亡的时候自动调用了析构函数
>
> 但是也限制了指针的生存空间

```c++
class Data
{
private:
    
public:
    Data(void);
    ~Data(void);
    void func(void);

};
//智能指针类 ↓ ↓ ↓
class SmartPointer
{
private:
    Data *p;
public:
    SmartPointer(void){}
    SmartPointer(Data *p);  //有参构造函数
    ~SmartPointer(void);
    Data& operator*();  // 重载 *  运算符
    Data* operator->(); // 重载 -> 运算符

};

int main(int argc, char const *argv[])
{
    SmartPointer sm(new Data); // 实例化一个智能指针
    (*sm).func();
    sm->func();   //调用Data里面的函数

    return 0;
}

SmartPointer::SmartPointer(Data *p)  //智能指针类 有参构造函数
{
    this->p = p;
}

SmartPointer::~SmartPointer(void) //智能指针类 析构函数
{
    delete p;
}

Data& SmartPointer::operator*()
{
    return *p;
}

Data* SmartPointer::operator->()
{
    return p;
}

Data::Data(void)
{
    cout << "构造函数" << endl;
}

Data::~Data(void)
{
    cout << "析构函数" << endl;
}

void Data::func(void)
{
    cout << "func" << endl;
}
```



## 4. 可以重载的符号

![image-20220720195435602](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220720195435.png)

短路特性重载很难实现 关于这里可以搜一下 不建议重载 && || 这两个符号



# 继承

## 1. 继承的概述

![image-20220721092436570](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721092731.png)

注意:

![image-20220803103612111](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220803103633.png)





## 2. 子类的定义形式

```c++
class 父类{};
class 子类:继承方式 父类名
{
    //新增子类数据
}
```

继承方式：private protected public(推荐) 

![image-20220721092908581](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721092908.png)

公共继承 保持不变，保护继承变保护，私有继承变私有，所有父类私有在子类中不可见。

```c++
//应该简单吧  不需要我写点代码了吧
```



## 3. 子类构造和析构的顺序

![image-20220721093517644](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721093535.png)

构造顺序 : 父成子

析构顺序 : 子成父

```c++

```



## 4.子类调用成员对象 父类的有参构造

* 子类 会自动调用 成员对象、父类的**默认构造**。
* 子类 必须使用**初始化列表** 调用成员对象、父类的**有参**构造。 
* 初始化列表时：父类写**类名称** 成员对象用**对象名**。 

```c++
class Base
{
public:
    int a;
public:
    Base()
    {
        cout<<"父类默认构造"<<endl;
    }
    Base(int a)
    {
        this->a = a;
        cout<<"父类有参构造"<<endl;
    }
    ~Base()
    {
        cout<<"父类析够"<<endl;
    }
};
class Other
{
public:
    int b;
public:
    Other()
    {
        cout<<"Other默认构造"<<endl;
    }
    Other(int b)
    {
        this->b = b;
        cout<<"Other有参构造"<<endl;
    }
    ~Other()
    {
        cout<<"Other析够"<<endl;
    }
};

class Son:public Base
{
public:
    Other ob;
    int c;
public:
    Son()
    {
        cout<<"Son构造"<<endl;
    }
    //父类写类名称   成员对象用对象名
    Son(int a, int b, int c):Base(a), ob(b)
    {
        this->c = c;
        cout<<"Son有参构造"<<endl;
    }
    ~Son()
    {
        cout<<"Son析够"<<endl;
    }
};

void test01()
{
    Son ob(10,20,30);
}
```

![image-20220721094755928](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721094756.png)



## 5. 子类父类同名成员处理

同名成员 最简单 最安全的处理方式：**加作用域**



### 5.1 同名成员数据

* 子类默认**优先**访问 **子类**的同名成员 
* 必须加父类**作用域** 访问父类的同名成员 

```c++
class Base
{
public:
    int a;
public:
    Base(){}
    Base(int a)
    {
        this->a = a;
    }
};

class Son:public Base
{
public:
    int a;
public:
    Son(){}
    Son(int a, int b):Base(b)
    {
        this->a = a;
    }
};

void test01()
{
    Son ob(10,20);
    cout << ob.a << endl;
    cout << ob.Base::a << endl;
}
```



### 5.2 同名成员函数

* 子类默认**优先**访问 **子类**的同名成员 函数
* 必须加父类**作用域** 访问父类的同名成员 函数

```c++
class Base
{
public:
    int a;
public:
    void func01()
    {
        cout << "Base func01 无参函数" << endl;
    }
};

class Son:public Base
{
public:
    int a;
public:
    void func01()
    {
        cout << "Son func01 无参函数" << endl;
    }
};

void test01()
{
    Son ob;
    ob.func01();
}
```



## 5.3 子类重定义父类的 同名函数

* 重载：无继承，同一作用域，参数的个数、顺序、类型不同 都可重载 
* 重定义：**有继承**， 子类 重定义 父类的同名函数（参数可以不同）（**非虚函数**） 
* 子类一旦 **重定义**了父类的同名函数（不管参数是否一致），子类中都将屏蔽**父类所有**的同名函数。
* 如果**重定义之后**还想**调用父类的同名函数**,那么加上**作用域**即可

```c++
using namespace std;
class Base
{
public:
    int a;
public:
    void func01()  //func01 重载
    {
        cout << "Base func01 无参函数" << endl;
    }
    void func01(int a)
    {
        cout << "Base  int func01 无参函数" << endl;
    }
    void func01(int a, int b)
    {
        cout << "Base  int int func01 无参函数" << endl;
    }
};

class Son:public Base
{
public:
    int a;
public:
    void func01()
    {
        cout << "Son func01 无参函数" << endl;
    }
};

void test01()
{
    Son ob;
    ob.func01();
    //ob.func01(50);        //错误 父类已被屏蔽 
    //ob.func01(10, 20);    //错误 父类已被屏蔽 
    ob.Base::func01(10, 20);//正确
}
```

> 需要将父类的作用域 才能识别 屏蔽的函数



## 6. 子类不能继承父类成员

父类的构造、析够、operator= 不能被子类 继承 



## 7. 多继承

### 7.1 多继承的格式

```c++
class 父类1{};
class 父类2{};
class 子类:继承方式1 父类1, 继承方式2 父类2
{
    //新增子类数据
}
```

这样的话 子类就拥有了父类1，父类2的**大部分数据 **

```c++
class Base
{
public:
    int a;
public:
};

class Son:public Base
{
public:
    int b;
public:
};

void test01()
{
    Son ob;
    cout << ob.a << endl;
    cout << ob.b << endl;
}
```

![image-20220721101732772](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721101732.png)



## 7.2 多继承中的同名成员

依旧是优先子类成员  父类成员需要加上作用域调用

```c++
class Base1
{
public:
    int a;
public:
    Base1(int a):a(a){}
};

class Base2
{
public:
    int a;
public:
    Base2(int a):a(a){}
};

class Son:public Base1, public Base2
{
public:
    int a;
public:
    Son(int a, int b, int c):Base1(a), Base2(b), a(c){}
};

void test01()
{
    Son ob(10, 20, 30);
    cout << ob.a << endl;
    cout << ob.Base1::a << endl;
    cout << ob.Base2::a << endl;
}
```

> 思考:如果子类没有 int a ,那么会调用哪个父类对象的 a 呢?

答案 : 出现二义性问题,直接报错.



## 8. 菱形继承  (了解)

菱形继承：有公共祖先的继承 叫菱形继承。 

最底层的子类 数据 会包含多分（公共祖先的数据）。 

![image-20220721103756980](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721103757.png)

```c++
class Animal
{
public:
    int data;
};
class Sheep :public Animal{};
class Tuo :public Animal {};
class SheepTuo:public Sheep,public Tuo{};

int main()
{
    SheepTuo ob;
    memset(&ob, 0, sizeof(SheepTuo));
    
    //cout << ob.data << endl;//二义性
    cout << ob.Sheep::data << endl;
    cout << ob.Tuo::data << endl;
}
```

这样继承的话就会有两份祖先的数据  在调用函数的时候出现 二义性 有着极大的不便

>  问 : 怎么才能只要公共祖先的一份数据呢？



## 9. 虚继承

虚继承 解决 菱形继承中 多分公共祖先数据的问题 



### 9.1 虚继承的概述

* 在继承方式 前加 **virtual** 修饰 
* 子类**虚继承**父类 子类只会保存一份公共数据

```c++
class Animal
{
public:
    int data;
};
class Sheep : virtual public Animal{};
class Tuo : virtual public Animal {};
class SheepTuo:public Sheep,public Tuo{};

int main()
{
    SheepTuo ob;
	cout << ob.data << endl;//二义性
}
```

![image-20220721104736479](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721104736.png)

实际上 这里会存在一个 vbptr 的指针 我们首先来看一下 这几个类的布局

Animal布局：

![image-20220721104833320](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721104833.png)

Sheep布局： 

![image-20220721104905747](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721104905.png)

Tuo布局：

![image-20220721104948910](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721104949.png)

SheepTuo布局：

![image-20220721105007141](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721105007.png)

![image-20220721105128535](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721105128.png)

虚继承 会在子类中产生 **虚基类指针**（vbptr） 指向**虚基类表**(vbtable), 虚基类表记录的是

通过该指针访问公共祖先的数据的**偏移量**。

注意：

​	虚继承**只能解决具备公共祖先的**多继承所带来的二义性问题，不能解决没有公共祖先的 

多继承的.

​	工程开发中真正意义上的多继承是**几乎不被使用**，因为多重继承带来的代码复杂性远多 

于其带来的便利，多重继承对代码维护性上的影响是灾难性的，在设计方法上，任何多继承 

都可以用**单继承**代替。



# 多态

* **静态**多态（**编译时**多态，**早**绑定）：函数重载、运算符重载 
* **动态**多态（**运行时**多态，**晚**绑定）：**虚**函数 



## 1. 虚函数

![image-20220721113553745](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721113554.png)

父类指针（引用）保存 子类空间地址的目的 就是让算法通用。 

> 但是这样的话会带来一个显著的问题 : 让父类指针指向子类 , 那么父类就只能去操作父类里面的类容,触及不到子类的内容了 

```c++
 class Animal
 {
public:
     void speak(void)
     {
         cout<<"动物在说话"<<endl;
     }
 };

class Dog:public Animal
{
public:
    void speak(void)
    {
        cout<<"狗在汪汪"<<endl;
    }
}

void test01()
{
    Animal *p = new Dog; // 父类指针来操作子类对象
    p‐>speak();
}
```

![image-20220721160725273](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721160725.png)



其实用户的需求：p->speak 希望等到的是“狗在汪汪” 而不是“动物在说话”。

原因在此： 

![image-20220721161007195](C:/Users/%E9%98%BF%E4%B8%83/AppData/Roaming/Typora/typora-user-images/image-20220721161007195.png)

这里的 p 指向的是父类,而不是子类里面的对象, 如何解决这个问题呢?



## 2. 虚函数的定义

虚函数的定义 :  成员函数之前加上 **virtual** 修饰

子类**重写**父类的虚函数注意：有继承、子类重写父类**虚函数**（函数名、返回值类型、参数**类型个数顺序** 必须完全一致)。



 **注意：**

1、只有**类的成员函数**才能定义为虚函数。

2、**静态成员函数**不能是虚函数，因为它不受限于某个对象。

3、**内联函数**不能是虚函数。（非多态的话可以是内敛函数）

4、**构造函数**不能是虚函数。

5、**析构函数**可以是虚函数。



```c++
class Animal
{
public:
    virtual void speak(void)
    {
        cout<<"动物在说话"<<endl;
    }
};

class Dog:public Animal
{
public:
    void speak(void)
    {
        cout<<"狗在汪汪"<<endl;
    }
}

void test01()
{
    Animal *p = new Dog; // 父类指针来操作子类对象
    p‐>speak();
}
```

> 结果: 狗在汪汪

使用虚构函数,子类的 p 的指针指向的函数的地址就被**重写**为了子类的函数的地址,

这样在调用 p->speak() 的时候, 就直接调用了子类的函数  ==↓ ↓ ↓ ↓ 原理↓ ↓ ↓ ↓==



## 3. 虚函数的原理  (重要)

**Animal的类的结构：**

![image-20220721162326413](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721162326.png)

如果一个类中的**成员函数** 被**virtual**修饰，那么这个函数就是**虚函数**。类就会产生一个**虚函数指针**（vfptr）指向了一张**虚函数表**（vftable）。

如果这个类 没有涉及到继承， 这时虚函数表中 纪录及时**当前类**的虚函数入口地址。 

> 这里也就是说 如果 Animal 没有被 Dog 继承 那么这个 vfptr 指向的就是自己的 func



**Dog的类存结构：**

![image-20220721162642290](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721162642.png)

![image-20220721162724251](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721162724.png)



## 4. 纯虚函数 (重点)

在虚函数中不实现函数体：

```c++
class Animal
{
public:
    //纯虚函数
    virtual void speak(void ) = 0;
};
```

* 一旦类中**有纯虚函数**，那么这个类 就是**抽象类**
* 抽象类 **不能**实例化 **对象**。（Animal ob；错误） 
* 抽象类 必须**被继**承 同时 子类 **必须重写** 父类的**所有纯虚函数**，否则 子类也是**抽象类**
* 抽象类主要的目的 是设计 类的接口：

```c++
using namespace std;
//抽象制作饮品
class AbstractDrinking{
public:
    //烧水
    virtual void Boil() = 0;
    //冲泡
    virtual void Brew() = 0;
    //倒入杯中
    virtual void PourInCup() = 0;
    //加入辅料
    virtual void PutSomething() = 0;

};

//制作咖啡
class Coffee : public AbstractDrinking{
public:
    //烧水
    virtual void Boil(){
        cout << "煮农夫山泉!" << endl;
    }
    //冲泡
    virtual void Brew(){
        cout << "冲泡咖啡!" << endl;
    }
    //倒入杯中
    virtual void PourInCup(){
        cout << "将咖啡倒入杯中!" << endl;
    }
    //加入辅料
    virtual void PutSomething(){
        cout << "加入牛奶!" << endl;
    }
};

//制作茶水
class Tea : public AbstractDrinking{
public:
    //烧水
    virtual void Boil(){
        cout << "煮自来水!" << endl;
    }
    //冲泡
    virtual void Brew(){
        cout << "冲泡茶叶!" << endl;
    }
    //倒入杯中
    virtual void PourInCup(){
        cout << "将茶水倒入杯中!" << endl;
    }
    //加入辅料
    virtual void PutSomething(){
        cout << "加入食盐!" << endl;
    }
};

//业务函数
void DoBussiness(AbstractDrinking* drink){
    drink->Boil();
    drink->Brew();
    drink->PourInCup();
    drink->PutSomething();
    delete drink;
}

int main(int argc, char *argv[])
{
    DoBussiness(new Coffee);
    cout << "--------------" << endl;
    DoBussiness(new Tea);

    return 0;
}
```



## 5. 虚析构函数 (重要)

虚析构：通过**父类**指针 释放**整个**子类空间。 

```c++
class Animal
{
public:
    //纯虚函数
    virtual void speak(void)
    {
        cout << "动物在说话" << endl;
    }

    //虚析构函数
    virtual ~Animal()
    {
        cout<<"Animal的析构函数"<<endl;
    }
};
class Dog :public Animal
{
public:
    //子类重写 父类的虚函数
    void speak(void)
    {
        cout << "狗在汪汪" << endl;
    }
    ~Dog()
    {
        cout<<"Dog的析构函数"<<endl;
    }
};

void test01()
{
    Animal* p1 = new Dog;
    p1->speak();

    delete p1;

}
```

构造的顺序：父类--->成员---->子类 

析构的顺序：子类--->成员---->父类

析构的时候 , 先析构子类 , 在子类的析构函数后 , 会自动去寻找成员 和 父类 的析构函数 , 并调用他们

![image-20220721172025548](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721172025.png)

这里析构函数的重写也是跟 上面的的函数成员的虚函数的重写一样的



## 6. 纯虚析构

纯虚析构的本质：是**析构函数**，各个类的回收工作。而且析构函数不能被继承。

**必须**为**纯虚析构函数**提供一个**函数体**。

**纯虚析构函数 必须在类外实现**

```c++
class Animal
{
public:
    //纯虚函数
    virtual void speak(void)=0;

    //纯虚析构函数 必须在类外实现
    virtual ~Animal()=0;
};

Animal::~Animal()
{
    cout<<"Animal的析构函数"<<endl;
}

class Dog :public Animal
{
public:
    //子类重写 父类的虚函数
    void speak(void)
    {
        cout << "狗在汪汪" << endl;
    }
    ~Dog()
    {
        cout<<"Dog的析构函数"<<endl;
    }
};

void test01()
{
    Animal* p1 = new Dog;
    p1->speak();

    delete p1;
}
```

![image-20220721172559010](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220721172559.png)



## 7. 多态的常用问题 (重要)

### 7.1 多态的分类

* 多态分为静态多态和动态多态
* 静态多态 指的是在编译阶段就确定函数入口地址 就是常用的 函数重载 运算符重载
* 动态多态 指的是在运行过程中才确定函数入口地址 常见的就是虚函数  



### 7.2 谈谈对动态捆绑机制的理解(虚函数的实现原理)

* 虚函数的实现原理是 : 当一个父类的成员函数被 virtual 修饰的时候 , 如果还没有涉及到继承 ,  就会产生一个虚函数指针 , 
* 这个虚函数指针 指向 一张虚函数表 , 表里面记录的是 父函数的入口地址 , 
* 一旦这个类派生出子类 , 同样会继承 父类的虚函数指针 , 且指向虚函数表 , 表里面记录的也是 父函数的入口地址 , 
* 但是当子类一旦重写 父类的虚函数 , 这个时候虚函数表的入口地址 , 就会被覆盖成子类重写的 虚函数函数入口地址 , 
* 这时 , 当我们用父类的指针去调用虚函数的时候 , 就会间接的调用子类的虚函数



### 7.3 重载 重定义 重写 的区别

* 重载：同一作用域，同名函数，参数的顺序、个数、类型不同 都可以重载。函数的返回值 类型不能作为重载条件（函数重载、运算符重载） 

* 重定义：有继承，子类 重定义 父类的同名函数（非虚函数）， 参数顺序、个数、类型可以不同。子类的同名函数会屏蔽父类的所有同名函数（可以通过作用域解决） 
* 重写（覆盖）：有继承，子类 重写 父类的虚函数。返回值类型、函数名、参数顺序、个数、类型都必须一致。



### 7.4 虚函数和纯虚函数的区别

* 虚函数 : virtual 修饰 有函数体 不会导致父类为抽象类

* 纯虚函数 : virtual 修饰  **=0** , 没有函数体 导致父类为抽象类 子类必须**重写**父类的**所有纯虚函数**



### 7.5 虚析构和纯虚析构的区别

* 虚析构：virtual修饰，有函数体，不会导致父类为抽象类。 
* 纯虚析构：virtual修饰，=0，函数体必须类外实现，导致父类为抽象类。



### 7.6 虚函数的作用

* 可以让成员函数操作一般化，用基类的指针指向不同的派生类的对象时，
  基类指针调用其虚成员函数，则会调用其真正指向对象的成员函数，
  而不是基类中定义的成员函数（只要派生类改写了该成员函数）。
* 若不是虚函数，则不管基类指针指向的哪个派生类对象，调用时都
  会调用基类中定义的那个函数。



### 7.7 虚析构的作用

* 定义为虚析构函数是为了当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用。

* 但并不是要把所有类的析构函数都写成[虚函数](https://so.csdn.net/so/search?q=虚函数&spm=1001.2101.3001.7020)。**只有当一个类被用来作为基类的时候，才把析构函数写成虚函数。**



# 模板

* C++面向对象编程思想：封装、继承、多态 

* C++泛型编程思想：模板

  **模板**的分类：**函数模板**、**类模板**

  将**功能**相同，类型不同的 函数（类）的类型 抽象成**虚拟的类型**。当调用函数	（类实例化对象）的时候，编译器自动将虚拟的类型 **具体化**。这个就是函数模板（类模板）。 

  

## 1. 函数模板 (重点)

==关键字 : template==

```c++
//T只能对当前函数有效
template<typename T>
 void swapAll(T &a, T &b)
{
    T tmp = a;
    a = b;
    b=tmp;
    return;
}

void test01()
{
    int a = 10, b = 20;
    //函数调用时 更具实参的类型 会自动推导T的类型
    swapAll(a, b);
    cout<<a<<" "<<b<<endl;
    
    char a1 = 'a', b1 = 'b';
    swapAll(a1, b1);
    cout<<a1<<" "<<b1<<endl;
}
```

函数模板 会**编译两次**： 

​		第一次：是对函数模板 本身编译 

​		第二次：函数调用处 将T的类型具体化 

函数模板目标：模板是为了实现泛型，可以减轻编程的工作量，增强函数的**重用性**



## 2.函数模板和普通函数

### 2.1 函数模板 和 普通模板 都识别 (优先选择普通模板)

```c++
//T只能对当前函数有效 typename可以换成class
 template<typename T>
void swapAll(T &a, T &b)
{
     T tmp = a; 
     a = b;
     b=tmp;
     cout<<"函数模板"<<endl;
}

void swapAll(int &a, int &b)
{
     int tmp = a; 
     a = b;
     b=tmp;
     cout<<"函数模板"<<endl;
}

 void test01()
 {
     int a = 10, b = 20;
     //函数调用时 更具实参的类型 会自动推导T的类型
     swapAll(a, b);
     cout<<a<<" "<<b<<endl;
 }
```

![image-20220722154234995](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220722154235.png)



### 2.2 函数模板 和 普通函数 都识别 强制使用函数模板

```c++
//强制使用函数模板
swapAll<int>(a, b);
```



### 2.3 函数模板 自动类型推导时 不能对函数的参数 进行自动类型转换

简单点说:就是函数两个形参类型都是 T , 传入一个 int char 它不会将 int 转换成 char  或者将 char 转换成 int

但是如果强制使用函数模板 他还是会给你转化的 转换的时候输入类型

```c++
#if 1
template<typename T>
void myPrintAll(T a, T b)
{
    cout<<a<<" "<<b<<endl;
    cout<<"函数模板"<<endl;
}
 #endif

void myPrintAll(int a, int b)
{
    cout<<a<<" "<<b<<endl;
    cout<<"普通函数"<<endl;
}

void test02()
{
    myPrintAll(10, 20);//普通函数
    myPrintAll('a','b');//函数模板
    myPrintAll(10,'b');//普通函数
    
    //支持自动类型转换
    myPrintAll<int>(10,'b');
}
```

如果函数模板显示指明T的具体类型 这时函数模板的参数 可以**自动类型**转换



### 2.4 函数模板可以重载

```c++
template<typename T>
void myPrintAll(T a)
{
    cout<<a<<endl;
    cout<<"函数模板"<<endl;
}

template<typename T>
void myPrintAll(T a, T b)
{
    cout<<a<<" "<<b<<endl;
    cout<<"重载的函数模板"<<endl;
}
```



### 2.5 函数模板的局限性

当函数模板 推导出 **T为数组**或其他**自定义类型**数据 可能导致**运算符** 不识别。 

比如基础的 =(赋值) 遇到数组那就不行啦

**解决办法一：运算符重载（推荐）** 

```c++
class Data
{
    friend ostream& operator<<(ostream& out, Data ob);
private:
    int data;
public:
    Data(){}
    Data(int data)
    {
        this->data = data;
    }
};
ostream& operator<<(ostream& out, Data ob)
{
    out<<ob.data;
    return out;
}
```



**解决办法二：具体化函数模板**

```c++
template<typename T>
void myPrintAll(T a)
{
    cout<<a<<endl;
    cout<<"函数模板"<<endl;
}

class Data
{
    friend void myPrintAll<Data>(Data a);
private:
    int data;
public:
    Data(){}
    Data(int data)
    {
        this->data = data;
    }
};

//函数模板具体化
template<> void myPrintAll<Data>(Data a)
{
    cout<<a.data<<endl;
    cout<<"函数模板具体化"<<endl;
}

int main(int argc, char *argv[])
{
    myPrintAll(10);
    myPrintAll('a');

    Data ob1(100);
    myPrintAll(ob1);

    return 0;
}
```



## 3. 类模板 (重要)

### 3.1 类模板的定义

类模板 将类中类型 抽象成虚拟类型。 

类中类型 : 类中的数据类型

```c++
//类模板
template<class T1, class T2>
class Data
{
private:   
    T1 a;
    T2 b;
public:
    Data(){}
    Data(T1 a, T2 b)
    {
        this->a = a;
        this->b = b;
    }
    void showData()
    {
        cout<<a<<" "<<b<<endl;
    }
}
```

类模板 实例化对象 **不能** **自动类型推导**（**重要**）

```c++
Data<int,int> √
    
```



![image-20220722193141595](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220722193304.png)

类模板 的成员函数 在类外实现：

```c++
//类模板
template<class T1, class T2>
class Data
{
private:   
    T1 a;
    T2 b;
public:
    Data(){}
    Data(T1 a, T2 b);
    void showData();
}

template<class T1, class T2>
Data<T1,T2>::Data(T1 a, T2 b)
{
    this->a = a;
    this->b = b;
}

template<class T1, class T2>
void Data<T1,T2>::showData()
{
    cout<<a<<" "<<b<<endl;
}
```

> 这里要明确的一点是  这里的类型不再是 Data 而是 Data<T1,T2>

### 3.2 函数模板做类模板的友元

```c++
template<class T1, class T2>
class Data
{
    template<typename T3, typename T4> 
    friend void myPrintData(Data<T3, T4> &ob);
private:   
    T1 a;
    T2 b;
public:
    Data(){}
    Data(T1 a, T2 b);
    void showData();
}

template<typename T3, typename T4> 
void myPrintData(Data<T3, T4> &ob)
{
    cout<<ob.a<<" "<<ob.b<<endl;
}
```

> 这里需要注意 : 函数模板作为类模板的友元的时候 这里的 T1 T2
>
>  不能继承类模板本身的 也就是不能跟类模板的相同  要采用 T3 T4



## 3.3 普通函数做类模板的友元

```c++
template<class T1, class T2>
class Data
{
    friend void myPrintData(Data<int, char> &ob);
private:   
    T1 a;
    T2 b;
public:
    Data(){}
    Data(T1 a, T2 b);
    void showData();
}

void myPrintData(Data<int, char> &ob)
{
    cout<<ob.a<<" "<<ob.b<<endl;
}

void test01()
{
    Data<int, char> ob1(100, 'A');
    myPrintData(ob1);
}
```

> 普通函数的话 每一种不同的类型都得写一种  就比较麻烦了



## 5. 类模板的继承 (重要)

### 5.1 类模板派生普通类

```c++
//类模板
template<class T1, class T2>
class Data
{
private:   
    T1 a;
    T2 b;
public:
    Data(){}
    Data(T1 a, T2 b);
    void showData();
}

template<class T1, class T2>
Data<T1,T2>::Data(T1 a, T2 b)
{
    this->a = a;
    this->b = b;
}

template<class T1, class T2>
void Data<T1,T2>::showData()
{
    cout<<a<<" "<<b<<endl;
}

//派生普通类
class Son1:public Base<int,char>
{
public:
    int c;
public:
    Son1(int a, char b, int c):Base<int,char>(a, b){
        this‐>c = c;
    }
};

int main(int argc, char *argv[])
{
    Son1 ob1(100, 'A', 200);
    ob1.showData();
    cout<<ob1.c<<endl;
    return 0;
}

```

> **类模板**派生**普通类模板** 必须指定 参数的类型 同样的可以通过参数列表来对函数进行初始化



## 5.2 类模板 派生出 类模板

```c++
template<class T1, class T2>
class Base
{
private:
    T1 a;
    T2 b;
public:
    Base(){}
    Base(T1 a, T2 b);
    void showData();
};
template<class T1, class T2>
Base<T1,T2>::Base(T1 a, T2 b)
{
    this->a = a;
    this->b = b;
}

template<class T1, class T2>
void Base<T1,T2>::showData()
{
    cout<<a<<" "<<b<<endl;
}

//类模板派生出类模板
template<class T1, class T2, class T3>
class Son1:public Base<T1,T2>
{
public:
    T3 c;
public:
    Son1(T1 a, T2 b, T3 c):Base<T1,T2>(a, b){
        this->c = c;
    }
};
int main(int argc, char *argv[])
{
    Son1<int,char, int> ob1(100, 'A', 200);
    ob1.showData();
    cout<<ob1.c<<endl;
    return 0;
}
```

> 对于 Son1 而言 他本身也是一个 类模板 所以需要加上 template 进行修饰
>
> class Son1:public Base<T1,T2> 同样需要对类模板 Data 进行参数类型说明



## 6.类模板的头文件 源文件分离问题

* 类模板最好存储在 .hpp 中


 data.hpp

```c++
#ifndef DATA_H
#define DATA_H
#include<iostream>
using namespace std;
template<class T1, class T2>
class Data
{
private:
    T1 a;
    T2 b;
public:
    Data();
    Data(T1 a, T2 b);
    void showData(void);
};

template<class T1, class T2>
Data<T1, T2>::Data()
{
    cout<<"无参构造"<<endl;
}
template<class T1, class T2>
Data<T1, T2>::Data(T1 a, T2 b)
{
    this->a = a;
    this->b = b;
}
template<class T1, class T2>
void Data<T1, T2>::showData(void)
{
    cout<<a<<" "<<b<<endl;
}
#endif // DATA_H
```

main.c

```c++
#include <iostream>
#include"data.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    Data<int,char> ob1(100,'A');
    ob1.showData();
    return 0;
}

```

> 这里要解决的问题就是 : 类中的函数 类外实现 但是类呢定义在我们的 .h 文件里面 包含不到我们的 类的类外实现函数 所以只有将类的成员函数的定义和实现放在一起 形成一个 .hpp 文件 来解决这个问题



## 7. 设计数组类模板

这里就是一个小测验 

数组类模板 : 可以存放任意数据类型

模板栈类: 可以压栈各种类型 

//运算符的重载 

//sortArry 函数(排序)





# 类型转换



## 1.上行 下行转换

![image-20220722201642036](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220722201642.png)

> 可以见到 上行转换是安全的 下行转换就相对而言没有这么安全了

 

## 2. static_cast 静态类型转换

```c++
class Base{};
class Son:public Base{};
class Other{};
```

用于**类层次结构**中基类 (父类) 和 派生类 (子类)   之间指针或引用的转换

基本类型 : 支持

```c++
int num = static_cast<int>(3.14);//ok
```

上行转换：支持 安全 

```c++
Base *p = static_cast<Base *>(new Son);
```

下行转换：支持 （不安全） 

```c++
 Son *p2 = static_cast<Son *>(new Base);
```

不相关类型转换：不支持 

```c++
Base *p3 = static_cast<Base *>(new Other);//err
```



## 3. **dynamic_cast静态类型转换** 

基本类型 : 不支持

```c++
1 int num = dynamic_cast<int>(3.14);//err
```

上行转换：支持 安全 

```c++
Base *p1 = dynamic_cast<Base *>(new Son);//ok
```

下行转换：不支持 （不安全） 

```c++
 Son *p2 = dynamic_cast<Son *>(new Base);//err
```

不相关类型转换：不支持 

```c++
Base *p3 = dynamic_cast<Base *>(new Other);//err
```



## 4. const_cast常量转换

1、将const修饰的**指针或引用**转换成 非const （支持）

```c++
const int *p1;
int *p2 = const_cast<int *>(p1);
const int &ob = 10;
int &ob1 = const_cast<int &>(ob);
```

2、将非const修饰的**指针或引用** 转换成 const （支持）

```c++
int *p3;
const *p4 = const_cast<int *>(p3);
int data = 10;
const int &ob2 = const_cast<const int &>(data);
```



## 5. 重新解释转换(reinterpret_cast)

![image-20220722202921380](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220722202921.png)



# 异常

遇到错误 **抛出**异常 **捕获**异常 

* 构造函数**可以**抛出异常
* 普通函数**可以**抛出异常
* 析构函数**不能**抛出异常 `noexcept` 修饰

如果一个函数绝对不会抛出异常 ,  那么就在这个函数的参数列表后面 添加

 `noexcept`  来进行说明 ,  但是你还是可以抛出异常的



异常：是指在程序运行的过程中发生的一些异常事件（如：**除0**溢出，**数组下标**越界，所要读取的**文件不存在** , **空指针**，**内存不足**，**访问非法内存**等等）。（异常是一个**类**）

c++异常机制相比C语言异常处理的优势? 

* 函数的**返回值**可以忽略，但异常不可忽略。（忽略异常 程序结束）

* 整型返回值没有任何语义信息。而**异常却包含语义信息**，有时你从类名就能够体现出来 



## 1. 异常的抛出 捕获

### 1.1 异常的捕获

```c++
try
{
    throw 异常值;
}

catch(异常类型1 异常值1)
{
    处理异常的代码1;
}
catch(异常类型2 异常值2)
{
    处理异常的代码2;
}
catch(...)//任何异常都捕获
{
    处理异常的代码3;
}
```



案例 1:

```c++
int ret = 0;
try
{
    //throw 1;
    //throw 'A';
    throw 2.14f;
}

catch(int e)//捕获
{
    cout<<"int异常值为:"<<e<<endl;
}
catch(char e)//捕获
{
    cout<<"char异常值为:"<<e<<endl;
}
catch(...)//任何异常都捕获
{
    cout<<"其他异常值为:"<<endl;
}
```

![image-20220723111022936](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723111023.png)



## 2. 栈解旋

异常被抛出后，从进入try块起，到异常被抛掷前，这期间在栈上构造的所有对象，**都会被自动析构**。析构的顺序与构造的顺序**相反**，这一过程称为栈的解旋.  

```c++
try
{
    Data ob1;
    Data ob2;
    Data ob4;
    throw 1;//抛出异常后 ob3 ob2 ob1依次自动释放（栈解旋）
}
catch(int e)//捕获
{
    cout<<"int异常值为:"<<e<<endl;
}
```

```c++
 class Data
 {
public:
     int a;
public:
     Data(){}
     Data(int a)
     {
         this‐>a = a;
         cout<<"构造函数"<<a<<endl;
     }
     ~Data()
     {
         cout<<"析构函数"<<a<<endl;
     }
 };
```

![image-20220723112454753](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723112454.png)



## 3. 异常的接口说明

异常的接口声明：描述的是 可以抛出哪些类型的异常 

```c++
//函数默认 可以抛出任何类型的异常（推荐）
void fun01(){}

//只能抛出int,char异常
void fun02() throw(int,char){}

//不能抛出 任何异常
void fun03() throw(){}
```

> 这里如果抛出了声明之外的异常  try 是不会对该异常进行捕获的
>
> 这个异常将交给终端处理  最终的结果一般是 程序结束



## 4. 异常变量的声明周期

```c++
class MyException
{
public:
    MyException(){
        cout << "异常变量构造" << endl;
    };
    MyException(const MyException & e)
    {
        cout << "拷贝构造" << endl;
    }
    ~MyException()
    {
        cout << "异常变量析构" << endl;
    }
};

void doWork()
{
	throw  MyException();
}

//情况一
catch (MyException & e)
{
	cout << "异常捕获" << endl;
}
//情况二
catch (MyException e)
{
	cout << "异常捕获" << endl;
}
//情况三
catch (MyException* e)
{
	cout << "异常捕获" << endl;
	delete e;
}

```

> 情况一，对应于调用构造 异常捕获后的析构
>
> 情况二，对应于调用构造和拷贝构造 异常捕获 析构
>
> 情况三，对应于new构造 异常捕获 析构（注意throw的时候不能使用&获取匿名对象地址的方式，会直接进行析构，无法捕获异常）



### 4.1 普通对象 接异常值

![image-20220723113733051](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723113733.png)



### 4.2 以对象指针 接异常值

![image-20220723113828741](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723113828.png)



### 4.3 对象引用 接异常值 (推荐)

![image-20220723113916918](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723113917.png)



## 5.异常的多态

```c++
//异常基类
class BaseException{
public:
    virtual void printError(){};
}

//空指针异常
 class NullPointerException : public BaseException{
 public:
     virtual void printError(){
     cout << "空指针异常!" << endl;
     }
 }

//越界异常
class OutOfRangeException : public BaseException{
public:
    virtual void printError(){
    cout << "越界异常!" << endl;
    }
}

void doWork(){
	//throw NullPointerException();
    throw OutOfRangeException();
}
void test03()
{
    try{
    	doWork();
    }
    //父类引用 可以捕获搭配该父类派生出的所有子类的子类
    catch (BaseException& ex)
    {
        ex.printError();
    }
}
```

![image-20220723140722153](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723140722.png)



## 6. c++标准异常 (重要)

![image-20220723140815548](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723140815.png)



![image-20220723140849128](https://bucketforago.oss-cn-shenzhen.aliyuncs.com/typora/20220723140849.png)

> 值得注意的是 : 有一些异常并不支持 输出异常信息



## 7.编写自己的异常

编写字节的异常：基于标准异常

自己的异常 一定要从exception上继承 

```c++
class NewException:public exception
{
private:
    string msg;
public:
    NewException(){}
    NewException(string msg)
    {
        this->msg = msg;
    }
    //重写父类的what
    //防止父类在子类前抛出标准异常  ↓  ↓  ↓  ↓ 
    virtual const char* what()const throw()
    {
        //将string类转换成char *
        return this->msg.c_str();
    }
    ~NewException(){}
};

void test05()
{
    try
    {
        throw NewException("哈哈，自己的异常");

    }
    catch(exception &e)
    {
        cout<<e.what()<<endl;
    }
}
```







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

演讲比赛



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

























