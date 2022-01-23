
CFLAGS:= -I./ -L./lib -lc -lstdc++ -ldl -lgtest -lm -pthread

#需要清除的临时文件
#The tempory files which need to be clear
CLEAN_TARGET:=*.o *.elf *.exe

#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  $(shell pwd)/*.cpp)
SRC_C:=$(wildcard  $(shell pwd)*.c)
#指定可执行文件位置与名字
OUT_FILE?=timeparser_test.elf
OUT_TARGET:=$(OUT_FILE)
#调试版本的文件名
#the file name of debug version
DOUT_FILE?=timeparser_test_debug.elf
DOUT_TARGET:=$(DOUT_FILE)

#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))
#所有的.o文件
#all .o files
OBJ:=$(OBJ_SRC_CPP) $(OBJ_SRC_C)

#----------------------------------build----------------------------------#
#如果定义互斥(DEBUG=exclusive)，则调试的时候会删除发行版
#if DEBUG=exclusive, release version will be delate after build
ifeq ($(DEBUG),exclusive)
#编译调试版本
#bild debug version
all:$(MODULES) $(OBJ)
	$(TOOL_CHAIN) $(OBJ) -o $(DOUT_TARGET) $(CFLAGS)
	rm -f $(OUT_TARGET)

else
#编译release版本
#build
all:$(MODULES) $(OBJ)
	$(TOOL_CHAIN) $(OBJ) -o $(OUT_TARGET) $(CFLAGS)
endif

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(CLEAN_TARGET)
