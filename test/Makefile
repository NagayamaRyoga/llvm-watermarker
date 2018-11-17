CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	-fPIC

LDFLAGS :=

.PHONY: all run clean

all: test_pass

run: test_pass
	./test_pass

test_pass: Test.o
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

Test.o: \
	Test.cpp \
	TestPermutationTable.hpp \
	../src/nykk/PermutationTable.hpp

clean:
	${RM} test_pass *.o
