TARGET:=snake
#定义变量，保存最终的目标名

CC:=arm-linux-g++
#定义变量，保存编译器

SRCS:=$(wildcard *.cpp)
#定义变量 SRCS 用来保存当前目录下的所有 .cpp文件列表

#SRCS+=$(wildcard path/*.cpp)
#如果说 .cpp文件在多个目录下，就根据情况去增加 path就是文件的路径

OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
#把 SRCS 字符串中的 .cpp 替换成了 .o

INC:=-I inc
#指定头文件的搜索路径

#INC+=-I path
#如果头文件存在不同的目录下，需要用多个-I 去指定

LIB:=-l pthread
#指定库的搜索路径及库的名字
#LIB+=-L path -l xxxx
#如果有多个库文件，就分别指定即可

$(TARGET) : $(OBJS)
	$(CC) $^ $(LIB) -o $(TARGET) 
#请注意：如果在开发板运行时报错说库版本不兼容的话，请在上面的命令后面加上 -static-libstdc++
#	$(CC) $^ $(LIB) -o $(TARGET) -static-libstdc++

%.o : %.cpp
#%是通配 %.o匹配所有的.o文件
	$(CC) -c $^ $(INC) -o $@ -std=c++11
	
clean :
	rm $(OBJS) $(TARGET)