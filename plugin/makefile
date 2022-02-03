#所需要编译的插件类型，注意模组名要和文件夹同名
#The type of plugin modules which would be build. Noticing, plugin modules name should be as directory name
MODULES+=configNode

#依次编译所有的插件模组
#build every plugin modules one by one
all:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i TOOL_CHAIN=$(TOOL_CHAIN); \
	done

.PHONY: clean
clean:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i clean; \
	done