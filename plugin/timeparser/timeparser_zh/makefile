#插件名，timeparser插件要求插件名的前缀必须是“timeparser”
#the name of plugin.The prefix of the timeparser plugin must be timeparser
PLAGING_NAME:=timeparser_zh


OUT_FILE:=$(PLAGING_NAME).so

#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(UPPER_BUILD_DIR)$(PLAGING_NAME)/

#头文件目录
#include directories
INCLUDE_DIR:=-I$(shell pwd)/../include -I$(shell pwd)/../../include 

ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS:=
else
	CFLAGS:= -DWINDOWS
endif


#寻找所有.c文件的路径
#find all path of .c files
SRC:=$(wildcard  ./*.c  $(UPPER_BUILD_DIR)/lib/*.c)
C_FILE=$(notdir $(SRC))
OBJ_SRC=$(patsubst %.c, %.o, $(SRC))
OBJ=$(patsubst %.c, $(BUILD_DIR)%.o, $(C_FILE))


all:build_prepare $(OBJ_SRC)
	$(TOOL_CHAIN) $(OBJ) -o $(NOTE_IT_PLUGIN)$(OUT_FILE) -shared $(CFLAGS)

#将.c编译为.o
#compile .c files to .o
$(OBJ_SRC) : %.o:%.c
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c  $(INCLUDE_DIR) -fPIC $(CFLAGS)

#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)