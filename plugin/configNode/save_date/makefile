
PLAGING_NAME:=save_date

JRME_PLUGIN?=~/.jrme/plugin/
TOOL_CHAIN?=gcc
#需要清除的临时文件
#The tempory files which need to be clear

OUT_FILE:=$(PLAGING_NAME).so

CLEAN_TARGET:=*.o $(JRME_PLUGIN)$(OUT_FILE)

#头文件目录
#include directories
INCLUDE_DIR:=-I$(shell pwd)/../../include -I$(shell pwd)/../lib


#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  ./*.cpp)
SRC_C:=$(wildcard  ./*.c)

#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))

OBJ=$(OBJ_SRC_CPP) $(OBJ_SRC_C)
all:build_prepare $(OBJ)
	$(TOOL_CHAIN) $(OBJ) -o $(JRME_PLUGIN)$(OUT_FILE) -shared $(CFLAGS)

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $@ -c  $(INCLUDE_DIR) -fPIC $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $@ -c $(INCLUDE_DIR) -fPIC $(CFLAGS)


#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(JRME_PLUGIN)

.PHONY: clean
clean:
	rm -f $(CLEAN_TARGET)
