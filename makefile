OS?=LINUX

ifeq ($(OS),LINUX)								#选择gcc还是arm-linux-gccc
	TOOL_CHAIN?=gcc
else 
	TOOL_CHAIN?=x86_64-w64-mingw32-gcc
endif
					
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS:= -I./src -I./include -I./lib -I./plugin/timeparser/include -L./lib -lc -lstdc++ -ldl -static
else
	CFLAGS:= -I./src -I./include -I./lib -L./lib -lstdc++ -ldl  -static -DWINDOWS
endif

#传参觉得是否需要调试，如果DEBUG=exclusive，则调试的时候会删除release版本
ifdef DEBUG
	CFLAGS+=-g
endif	

#寻找所有.c和.cpp文件的路径
SRC_CPP:=$(wildcard  ./src/*.cpp ./lib/*.cpp)
SRC_C:=$(wildcard  ./src/*.c ./lib/*.c)
#指定中间文件存放位置
BUILD_DIR:=$(shell pwd)/build/
#指定输出目录
OUT_DIR:=./bin/
#指定可执行文件位置与名字
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	OUT_FILE?=note-it.elf
else
	OUT_FILE?=note-it.exe
endif
OUT_TARGET:=$(OUT_DIR)$(OUT_FILE)
#调试版本的文件名

DOUT_FILE?=note-it_debug.elf
DOUT_TARGET:=$(OUT_DIR)$(DOUT_FILE)


CPP_FILE:=$(notdir $(SRC_CPP))
C_FILE:=$(notdir $(SRC_C))
#把.cpp和c 换成.o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))
OBJ_CPP:=$(patsubst %.cpp, $(BUILD_DIR)%.o, $(CPP_FILE))
OBJ_C:=$(patsubst %.c, $(BUILD_DIR)%.o, $(C_FILE))
OBJ:=$(OBJ_CPP) $(OBJ_C)

#^:所有依赖-->OBJ ,@:生成目标放置地方，把.o放到OUT_TARGET中,CFLAGS:选择库
#根据是否由如果定义互斥，则调试的时候会删除发行版
ifeq ($(DEBUG),exclusive)
all:plugin $(OBJ_SRC_CPP) $(OBJ_SRC_C)
	$(TOOL_CHAIN) $(OBJ) -o $(DOUT_TARGET) $(CFLAGS)
	rm -f $(OUT_TARGET)

else
all:plugin $(OBJ_SRC_CPP) $(OBJ_SRC_C)
	echo $(OBJ_C)
	$(TOOL_CHAIN) $(OBJ) -o $(OUT_TARGET) $(CFLAGS)
endif

#^:由.cpp生成.o，CFLAGS:选择库
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c  $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c  $(CFLAGS)

.PHONY: plugin
plugin:
	$(MAKE) -C ./plugin UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS)

#清除可执行文件，重新编译
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)* $(OUT_DIR)* $(NOTE_IT_PLUGIN)*