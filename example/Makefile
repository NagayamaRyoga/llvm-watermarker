SRCS := \
	fizzbuzz.c

LLS := ${SRCS:.c=.ll}
LLS_0 := ${SRCS:%.c=%-0.ll}
LLS_1 := ${SRCS:%.c=%-1.ll}
LLS_2 := ${SRCS:%.c=%-2.ll}
BINS := ${LLS:.ll=} ${LLS_0:.ll=} ${LLS_1:.ll=} ${LLS_2:.ll=}

.PHONY: all test clean

all: ${BINS}

test: ${BINS}
	./fizzbuzz > fizzbuzz.txt
	./fizzbuzz-0 > fizzbuzz-0.txt
	./fizzbuzz-1 > fizzbuzz-1.txt
	./fizzbuzz-2 > fizzbuzz-2.txt

${BINS}: ${LLS} ${LLS_0} ${LLS_1} ${LLS_2}

%.ll: %.c
	clang -emit-llvm -S -O2 -o $@ $<

%-0.ll: %.ll
	opt-6.0 -load ../nykk.so -block-wm -watermark=0 -S -o $@ $<
%-1.ll: %.ll
	opt-6.0 -load ../nykk.so -block-wm -watermark=1 -S -o $@ $<
%-2.ll: %.ll
	opt-6.0 -load ../nykk.so -block-wm -watermark=2 -S -o $@ $<

%: %.ll
	clang -o $@ $<

clean:
	${RM} ${BINS} *.ll *.txt
