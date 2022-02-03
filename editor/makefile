OUT_TARGET:=libeditor.a
OS?=LINUX

#需要清除的临时文件
#The tempory files which need to be clear
TEMP_FILES=src/*.o lib/*.o

ifeq ($(OS),LINUX)
	TOOL_CHAIN?=gcc
else 
	TOOL_CHAIN?=x86_64-w64-mingw32-gcc
endif
					
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS:= -I./src -I./include -I./lib -L./lib -lc
else
	CFLAGS:= -I./src -I./include -I./lib -L./lib -lc -DWINDOWS
endif

#传参决定是否需要调试，如果DEBUG=exclusive，则调试的时候会删除release版本
#debug or not. if DEBUG=exclusive, release version will be delate after build
ifdef DEBUG
	CFLAGS+=-g
endif	

#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  *.cpp )
SRC_C:=$(wildcard  *.c)

#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))

OBJ=$(OBJ_SRC_CPP) $(OBJ_SRC_C)

#----------------------------------build----------------------------------#
#如果定义互斥(DEBUG=exclusive)，则调试的时候会删除发行版
#if DEBUG=exclusive, release version will be delate after build
ifdef $(DEBUG)
#编译调试版本
#bild debug version
all:$(OBJ_SRC_C) $(OBJ_SRC_CPP)
	ar rs $(OUT_TARGET) $(OBJ)

else
#编译release版本
#build
all:$(OBJ_SRC_C) $(OBJ_SRC_CPP)
	ar rs $(OUT_TARGET) $(OBJ)
endif

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS) -lstdc++

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

#清除可执行
#clean files
.PHONY: clean
clean:
	rm -rf *.o *.a
