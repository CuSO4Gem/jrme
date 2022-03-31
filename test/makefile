#选择想要编译测试模块
#select test modules which want to compile
MODULES+=configNode filter JournalIOFactory MdJournalIO SfJournalBook timeparser TxtJournalIO

TOOL_CHAIN?=gcc

#依次编译所有的测试模组
#build every test modules one by one
all:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i TOOL_CHAIN=$(TOOL_CHAIN) CODE_DIR=$(shell pwd); \
	done

test: all
	@ for i in $(MODULES); \
	do \
	cd $(shell pwd)/$$i && ./main.elf;\
	done

.PHONY: clean
clean:
	@ for i in $(MODULES); \
	do \
	$(MAKE) -C $$i clean; \
	done