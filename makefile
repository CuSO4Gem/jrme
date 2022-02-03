OS?=LINUX

ifeq ($(OS),LINUX)								#选择gcc还是arm-linux-gccc
	TOOL?=gcc
else 
	TOOL?=x86_64-w64-mingw32-gcc
endif
					
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS=-I./include -I./lib -L./lib -lc -lstdc++ -ldl -static 
else
	CFLAGS=-I./include -I./lib -L./lib -lstdc++ -ldl -static -DWINDOWS
endif

#传参觉得是否需要调试，如果DEBUG=exclusive，则调试的时候会删除release版本
ifdef DEBUG
	CFLAGS+=-g
endif	

#寻找所有.c和.cpp文件的路径
SRC=$(wildcard  ./src/*.c  ./src/*.cpp  ./lib/*.c ./lib/*.cpp)
#指定中间文件存放位置
DIR=./build/
#指定输出目录
OUT_DIR=`
#指定可执行文件位置与名字
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	OUT_FILE?=main.elf
else
	OUT_FILE?=main.exe
endif
OUT_TARGET:=$(OUT_DIR)$(OUT_FILE)
#调试版本的文件名

DOUT_FILE?=main_debug.elf
DOUT_TARGET:=$(OUT_DIR)$(DOUT_FILE)


CFILE=$(notdir $(SRC))
#把.cpp 换成.o
OBJ_SRC=$(patsubst %.cpp, %.o, $(SRC))
OBJ=$(patsubst %.cpp, $(DIR)%.o, $(CFILE))


#^:所有依赖-->OBJ ,@:生成目标放置地方，把.o放到OUT_TARGET中,CFLAGS:选择库
#根据是否由如果定义互斥，则调试的时候会删除发行版
ifeq ($(DEBUG),exclusive)
all:$(OBJ_SRC)
	$(TOOL) $(OBJ) -o $(DOUT_TARGET) $(CFLAGS)
	rm -f $(OUT_TARGET)

else
all:$(OBJ_SRC)
	$(TOOL) $(OBJ) -o $(OUT_TARGET) $(CFLAGS)
endif

#^:由.o生成.c，CFLAGS:选择库
$(OBJ_SRC) : %.o:%.cpp
	$(TOOL) $< -o $(DIR)$(notdir $@) -c  $(CFLAGS)

#清除可执行文件，重新编译
clean:
	rm -f $(DIR)* $(OUT_DIR)*
.PHONY: clean