#所需要编译的插件，注意模组名要和文件夹同名
#The of plugins which would be build. Noticing, plugins name should be as directory name
PLUGINS+=md_journal

#依次编译所有的插件
#build every plugins one by one
all:
	@ for i in $(PLUGINS); \
	do \
	$(MAKE) -C $$i TOOL_CHAIN=$(TOOL_CHAIN); \
	done

.PHONY: clean
clean:
	@ for i in $(PLUGINS); \
	do \
	$(MAKE) -C $$i clean; \
	done