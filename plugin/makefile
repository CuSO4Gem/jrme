MODULE_NAME+=timeparser

OS?=LINUX
ifeq ($(OS),LINUX)								#选择gcc还是arm-linux-gccc
	TOOL_CHAIN?=gcc
else 
	TOOL_CHAIN?=x86_64-w64-mingw32-gcc
endif

BUILD_DIR:=$(UPPER_BUILD_DIR)plugin/

all:build_prepare
	@ for i in $(MODULE_NAME); \
	do \
	$(MAKE) -C $$i UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS); \
	done

.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)