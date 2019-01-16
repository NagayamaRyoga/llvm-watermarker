include ../common.mk

all:
	${MAKE} -C fizzbuzz    all
	${MAKE} -C 8cc         all
	${MAKE} -C zlib        all
	${MAKE} -C lua         all
	${MAKE} -C cJSON       all
	${MAKE} -C tree        all
	${MAKE} -C jemalloc    all
	${MAKE} -C fizzbuzz-rs all
	${MAKE} -C numguess-rs all

test:
	${MAKE} -C fizzbuzz    test
	${MAKE} -C 8cc         test
	${MAKE} -C zlib        test
	${MAKE} -C lua         test
	${MAKE} -C cJSON       test
	${MAKE} -C tree        test
	${MAKE} -C jemalloc    test
	${MAKE} -C fizzbuzz-rs test
	${MAKE} -C numguess-rs test

benchmark:
	${MAKE} -C zlib        benchmark

clean::
	${MAKE} -C numguess-rs clean
