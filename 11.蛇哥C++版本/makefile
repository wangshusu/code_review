
#要生成的可执行文件名
TARGET :=exe

CC := arm-linux-g++
#CC := gcc

#SRC指定 工程中所有的c源文件名列表
SRCS := $(wildcard ./*.cpp ./src/*.cpp)

#OBJS指定 工程中所有的.o文件列表
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

#INCS指定 头文件的目录
INCS += -I./inc

#LIBS指定 所需要依赖的库以及库的搜索路径
LIBS += -L./lib -lpthread

$(TARGET): $(OBJS)
	$(CC) $^ $(LIBS) -o $@
	cp ./$(TARGET) ~/tftpboot/$(TARGET)

%.o: %.cpp
	$(CC) -cpp $<  $(INCS) -o $@
	
#"清除"：所有的中间文件和最终的可执行文件
#声明clean为伪文件
clean:
#shell 命令前的 - 表示强制这个命令的执行,如果执行失败也不会终止 
	-rm -rf $(OBJS)
# rm -rf $(TARGET)

