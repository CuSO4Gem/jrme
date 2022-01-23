#选择想要编译测试模块
#select test modules which want to compile
MODULES+=plugin txtJournalIO

#依次编译所有的测试模组
#build every test modules one by one
all:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS) CODE_DIR=$(shell pwd); \
	done

.PHONY: clean
clean:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i clean; \
	done