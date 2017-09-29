####### Compiler, tools and options
# 定义 c 文件编译器为 gcc
CC            = gcc
# 定义 c++ 文件编译器为 g++
CXX           = g++
# 定义 c 编译参数， 无嘛实际作用
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES) -std=c++11
# 定义 c++ 编译参数， 无嘛实际作用
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES) -std=c++11
# 定义 头文件 路径 为当前路径 
INCPATH       = -I./ 
# 定义需要链接的库， ncurses, 窗口操作库
LIBS          = -lncurses
# 定义链接器
LINK          = g++
# 定义链接参数，无嘛实际作用
LFLAGS        = -std=c++11
# 定义清除命令，无嘛用，看着好看
DEL_FILE      = rm -f
####### Files




#################################################################################
# 定义cpp文件集合为当前路径的所有cpp文件， ${wildcard ./*.cpp} 遍历当前文件路径所有cpp文件
CPP_SRC		= ${wildcard ./*.cpp}

# 定义cpp文件编译后的o文件集合
CPP_OBJS	= ${patsubst %.cpp, %.o, $(CPP_SRC)}

# 定义c文件集合为当前路径的所有c文件， ${wildcard ./*.c} 遍历当前文件路径所有c文件
C_SRC		= ${wildcard ./*.c}

# 定义c文件编译后的o文件集合
C_OBJS		= ${patsubst %.c, %.o, $(C_SRC)}

# 定义所有的o文件集合
OBJECTS		= $(CPP_OBJS) \
		  $(C_OBJS)

# 定义生成的可执行文件名称
TARGET  = demining

####### Implicit rules
# 具体参考<一起来写makefile>
# 缺省目标all, 依赖生成的可执行文件
# 如果make后没有添加参数，生成saolei
all: $(TARGET)

# saolei依赖所有的o文件
$(TARGET): $(OBJECTS) 
# 把所有的o文件链接成可执行文件saolei 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

# cpp对应的o文件集合，依赖对应的cpp文件
$(CPP_OBJS): %.o :%.cpp
# 用g++编译对应的o文件
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

# c对应的o文件集合，依赖对应的cpp文件
$(C_OBJS): %.o :%.c
# 用gcc编译对应的o文件
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

# make 带 c 参数: make c，等于make clean
c: 
# 清除所有过程文件
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) $(CPP_OBJS_D) $(C_OBJS_D)
	-$(DEL_FILE) *~ *.o *.d
clean:
# 清除saolei
	-$(DEL_FILE) $(TARGET)
# 清除所有过程文件
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) $(CPP_OBJS_D) $(C_OBJS_D)
	-$(DEL_FILE) *~ *.o *.d

#########################################################################
# auto dependence
# 定义头文件依赖
CPP_OBJS_D	= ${patsubst %.cpp, %.d, $(CPP_SRC)}
C_OBJS_D	= ${patsubst %.c, %.d, $(C_SRC)}
include $(CPP_OBJS_D)
include $(C_OBJS_D)

$(CPP_OBJS_D): %.d : %.cpp
	set -e; rm -f $@;
	$(CXX) $(INCPATH) -MM $(CXXFLAGS) $< > $@;

$(C_OBJS_D): %.d : %.c
	set -e; rm -f $@;
	$(CC) $(INCPATH) -MM $(CFLAGS) $< > $@;
