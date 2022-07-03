cd "$(dirname "$0")"
FILES=`ls|grep test_`
for test_elf in $FILES; \
	do \
	./$test_elf; \
	done