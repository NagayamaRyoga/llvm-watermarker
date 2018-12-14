include common.mk

.PHONY: all test clean

all:
	${MAKE} -C src     all
	${MAKE} -C test    all
	${MAKE} -C example all

test:
	${MAKE} -C src     test
	${MAKE} -C test    test
	${MAKE} -C example test

benchmark: test
	bin/example/zlib/test | tee bin/example/zlib/test.txt
	bin/example/zlib/test-block-wm | tee bin/example/zlib/test-block-wm.txt
	bin/example/zlib/test-inst-wm | tee bin/example/zlib/test-inst-wm.txt
	bin/example/zlib/test-wm | tee bin/example/zlib/test-wm.txt
	paste -d"," bin/example/zlib/test.txt bin/example/zlib/test-block-wm.txt bin/example/zlib/test-inst-wm.txt bin/example/zlib/test-wm.txt > bin/example/zlib/test.csv
