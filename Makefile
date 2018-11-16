CXX := g++

CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	$(shell llvm-config-6.0 --cxxflags)

LDFLAGS := \
	$(shell llvm-config-6.0 --ldflags) \
	$(shell llvm-config-6.0 --libs)

.PHONY: all clean

all: testpass.so

testpass.so: src/nykk/pass/TestPass.o src/nykk/pass/BlockCounterPass.o
	${CXX} ${CXXFLAGS} -shared -o $@ $^ ${LDFLAGS}

clean:
	${RM} *.so src/*/*.o
