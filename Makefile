LLVM_INCLUDE_DIRS := $(shell llvm-config-6.0 --includedir)

CXX := g++

CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	${LLVM_INCLUDE_DIRS:%=-I%} \
	-fPIC

LDFLAGS := \
	$(shell llvm-config-6.0 --ldflags) \
	$(shell llvm-config-6.0 --libs)

.PHONY: all test clean

all: testpass.so test_pass

test: test_pass
	./test_pass

testpass.so: \
	src/nykk/pass/TestPass.o \
	src/nykk/pass/BlockCounterPass.o \
	src/nykk/pass/BlockWatermarkPass.o

test/Test.o: \
	test/Test.cpp \
	test/TestPermutationTable.hpp \
	src/nykk/PermutationTable.hpp

test_pass: test/Test.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

%.so:
	${CXX} ${CXXFLAGS} -shared -o $@ $^ ${LDFLAGS}

clean:
	${RM} *.so test_pass src/*/*.o src/*/*/*.o test/*.o
