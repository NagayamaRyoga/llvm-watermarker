LLVM_INCLUDE_DIRS := $(shell llvm-config --includedir)

CXX := g++

CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	${LLVM_INCLUDE_DIRS:%=-I%} \
	-fPIC \
	-fno-rtti

LDFLAGS := \
	$(shell llvm-config --ldflags)

.PHONY: all test example clean

all: nykk.so example

test: nykk.so
	${MAKE} -C test run
	${MAKE} -C example test

example: nykk.so
	${MAKE} -C example

nykk.so: \
	src/nykk/pass/TestPass.o \
	src/nykk/pass/BlockCounterPass.o \
	src/nykk/pass/BlockWatermarkPass.o

src/nykk/pass/BlockWatermarkPass.o: \
	src/nykk/pass/BlockWatermarkPass.cpp \
	src/nykk/PermutationTable.hpp

%.so:
	${CXX} ${CXXFLAGS} -shared -o $@ $^ ${LDFLAGS}

clean:
	${MAKE} -C test clean
	${MAKE} -C example clean
	${RM} *.so src/*/*.o src/*/*/*.o
