#所需要编译的插件，注意模组名要和文件夹同名
#The of plugins which would be build. Noticing, plugins name should be as directory name
PLUGINS+=timeparser_zh

#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(UPPER_BUILD_DIR)timeparser/

#依次编译所有的插件
#build every plugins one by one
all:build_prepare
	@ for i in $(PLUGINS); \
	do \
	$(MAKE) -C $$i UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS); \
	done

#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)