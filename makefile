OS?=LINUX
#编译或者清除时是否连同处理插件
#build or clean plugins while make
PLUGIN?=n

#需要清除的临时文件
#The tempory files which need to be clear
TEMP_FILES:=src/*.o lib/*.o

TOOL_CHAIN?=gcc

STATIC:=		
CFLAGS:= -I./src -I./include -I./lib -I./plugin/timeparser/include -Llib -lc -lstdc++ -ldl -lpthread

#version
GIT_TAG:=$(shell git branch --show-current):$(shell git describe --tags)
CFLAGS+= -DGIT_TAG_VERSION=\"$(GIT_TAG)\"

#传参决定是否需要调试，如果DEBUG=exclusive，则调试的时候会删除release版本
#debug or not. if DEBUG=exclusive, release version will be delate after build
ifdef DEBUG
	CFLAGS+=-g -DDBG_PRINT_ENABLED="(4)"
endif	

#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  ./src/*.cpp ./lib/*.cpp)
SRC_C:=$(wildcard  ./src/*.c ./lib/*.c)
#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(shell pwd)/build/
#指定jrme的输出
#choose a directory to build out jrme
OUT_DIR:=$(shell pwd)/bin/
#指定可执行文件位置与名字
OUT_FILE?=jrme
OUT_TARGET:=$(OUT_DIR)$(OUT_FILE)
#调试版本的文件名
#the file name of debug version
DOUT_FILE?=jrme_debug
DOUT_TARGET:=$(OUT_DIR)$(DOUT_FILE)

#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))

OBJ=$(OBJ_SRC_CPP) $(OBJ_SRC_C)

ifeq ($(PLUGIN),y)
	MODULES+=plugin
endif

#------------------------------remove files-------------------------------#
CLEAN_TARGET:= $(OUT_DIR)* $(TEMP_FILES)
JRME_PLUGIN?=~/.jrme/plugin/
ifeq ($(PLUGIN),y)
	CLEAN_TARGET+=$(JRME_PLUGIN)*
endif

#----------------------------------build----------------------------------#
#如果定义互斥(DEBUG=exclusive)，则调试的时候会删除发行版
#if DEBUG=exclusive, release version will be delate after build
ifeq ($(DEBUG),exclusive)
#编译调试版本
#bild debug version
all:build_prepare  $(MODULES) $(OBJ_SRC_C) $(OBJ_SRC_CPP)
	$(TOOL_CHAIN) $(OBJ) -o $(DOUT_TARGET) $(CFLAGS) $(STATIC)
	rm -f $(OUT_TARGET)

else
#编译release版本
#build
all:build_prepare $(MODULES) $(OBJ_SRC_C) $(OBJ_SRC_CPP)
	$(TOOL_CHAIN) $(OBJ) -o $(OUT_TARGET) $(CFLAGS) $(STATIC)
endif

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

#编译测试用例
#compile test code
.PHONY: test
test: $(MODULES)
	$(MAKE) -C ./test test TOOL_CHAIN=$(TOOL_CHAIN) 

.PHONY: plugin
plugin:
	$(MAKE) -C ./plugin TOOL_CHAIN=$(TOOL_CHAIN)

.PHONY: build_prepare
build_prepare:
	mkdir -p $(OUT_DIR)

#清除可执行
#clean files
.PHONY: clean
clean:
	rm -rf $(CLEAN_TARGET)
ifeq ($(PLUGIN),y)
	$(MAKE) -C ./plugin clean
endif
	$(MAKE) -C ./test clean
