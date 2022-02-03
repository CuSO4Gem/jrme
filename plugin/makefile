#所需要编译的插件类型，注意模组名要和文件夹同名
#The type of plugin modules which would be build. Noticing, plugin modules name should be as directory name
MODULES+=timeparser

#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(UPPER_BUILD_DIR)plugin/

#依次编译所有的插件模组
#build every plugin modules one by one
all:build_prepare
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS); \
	done

#创建生产.o文件的目录
#make directory of saving .o files
.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)