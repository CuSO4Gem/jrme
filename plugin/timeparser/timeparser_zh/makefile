#The prefix of timeparser plugin must be timeparser
PLAGING_NAME:=timeparser_zh
OUT_FILE:=$(PLAGING_NAME).so

OS?=LINUX
ifeq ($(OS),LINUX)								#选择gcc还是arm-linux-gccc
	TOOL_CHAIN?=gcc
else 
	TOOL_CHAIN?=x86_64-w64-mingw32-gcc
endif

#build temporary dir
BUILD_DIR:=$(UPPER_BUILD_DIR)$(PLAGING_NAME)/
INCLUDE_DIR:=-I$(shell pwd)/../include -I$(shell pwd)/../../include 

ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS:=-lc 
else
	CFLAGS:=-lc  -DWINDOWS
endif


#source code
#源文件
SRC:=$(wildcard  ./*.c  $(UPPER_BUILD_DIR)/lib/*.c)
C_FILE=$(notdir $(SRC))
OBJ_SRC=$(patsubst %.c, %.o, $(SRC))
OBJ=$(patsubst %.c, $(BUILD_DIR)%.o, $(C_FILE))


all:build_prepare $(OBJ_SRC)
	$(TOOL_CHAIN) $(OBJ) -o $(NOTE_IT_PLUGIN)$(OUT_FILE) -shared $(CFLAGS)

$(OBJ_SRC) : %.o:%.c
	$(TOOL_CHAIN) $< -o $(BUILD_DIR)$(notdir $@) -c  $(INCLUDE_DIR) -fPIC $(CFLAGS)
	
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)