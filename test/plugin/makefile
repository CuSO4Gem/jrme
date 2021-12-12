#选择想要编译测试模块
#select test modules which want to compile
MODULES+=timeparser

#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(UPPER_BUILD_DIR)plugin/

#依次编译所有的测试模组
#build every test modules one by one
all:build_prepare
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS) OUT_DIR=$(OUT_DIR); \
	done

#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)