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

testpass.so: src/nykk/TestPass.o
	${CXX} ${CXXFLAGS} -shared -o $@ $< ${LDFLAGS}

src/nykk/TestPass.o: src/nykk/TestPass.cpp src/nykk/TestPass.hpp

clean:
	${RM} *.so src/*/*.o
