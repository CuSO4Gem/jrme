#插件名，timeparser插件要求插件名的前缀必须是“timeparser”
#the name of plugin.The prefix of the timeparser plugin must be timeparser
PLAGING_NAME:=timeparser_en


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


#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  ./*.cpp  ../lib/*.cpp)
SRC_C:=$(wildcard  ./*.c ../lib/*.c)

#所有的.cpp和.c文件的文件名
#the file name of all .cpp and .c
CPP_FILE:=$(notdir $(SRC_CPP))
C_FILE=$(notdir $(SRC_C))

#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))
#.o文件的输出路径
#the output path of .o files
OBJ_CPP:=$(patsubst %.cpp, $(BUILD_DIR)%.o, $(CPP_FILE))
OBJ_C:=$(patsubst %.c, $(BUILD_DIR)%.o, $(C_FILE))

OBJ=$(OBJ_CPP) $(OBJ_C)


all:build_prepare $(OBJ_SRC_CPP) $(OBJ_SRC_C)
	$(TOOL_CHAIN) $(OBJ) -o $(JRME_PLUGIN)$(OUT_FILE) -shared $(CFLAGS)

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c  $(INCLUDE_DIR) -fPIC $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c $(INCLUDE_DIR) -fPIC $(CFLAGS)


#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(JRME_PLUGIN)