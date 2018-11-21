WATERMARK := 77

CC := clang

SRCS := \
	8cc/buffer.c \
	8cc/cpp.c \
	8cc/debug.c \
	8cc/dict.c \
	8cc/encoding.c \
	8cc/error.c \
	8cc/file.c \
	8cc/gen.c \
	8cc/lex.c \
	8cc/main.c \
	8cc/map.c \
	8cc/parse.c \
	8cc/path.c \
	8cc/set.c \
	8cc/vector.c

.PHONY: all test clean

all: stage3 stage3-wm

stage3: stage2 8cc-stage3
stage2: stage1 8cc-stage2
stage1: 8cc-stage1

stage3-wm: stage2-wm 8cc-stage3-wm
stage2-wm: stage1-wm 8cc-stage2-wm
stage1-wm: 8cc-stage1-wm

# Normal 8cc
8cc-stage1: ${SRCS:8cc/%.c=test/stage1/%.o}
	${CC} -o $@ $^ -no-pie

8cc-stage2: ${SRCS:8cc/%.c=test/stage2/%.o}
	${CC} -o $@ $^ -no-pie

8cc-stage3: ${SRCS:8cc/%.c=test/stage3/%.o}
	${CC} -o $@ $^ -no-pie

test/stage1/%.o: 8cc/%.c
	@mkdir -p ${@D}
	${CC} -DBUILD_DIR="\"$$PWD/8cc\"" -O2 -o $@ -c $<

test/stage2/%.o: 8cc/%.c 8cc-stage1
	@mkdir -p ${@D}
	./8cc-stage1 -DBUILD_DIR="\"$$PWD/8cc\"" -w -o $@ -c $<

test/stage3/%.o: 8cc/%.c 8cc-stage2
	@mkdir -p ${@D}
	./8cc-stage2 -DBUILD_DIR="\"$$PWD/8cc\"" -w -o $@ -c $<

# Watermarked 8cc
8cc-stage1-wm: ${SRCS:8cc/%.c=test/stage1-wm/%.o}
	${CC} -o $@ $^ -no-pie

8cc-stage2-wm: ${SRCS:8cc/%.c=test/stage2-wm/%.o}
	${CC} -o $@ $^ -no-pie

8cc-stage3-wm: ${SRCS:8cc/%.c=test/stage3-wm/%.o}
	${CC} -o $@ $^ -no-pie

test/stage1-wm/%.o: test/stage1-wm/%-wm.ll
	clang -o $@ -c $<

test/stage1-wm/%-wm.ll: test/stage1-wm/%.ll
	opt -load=../../nykk.so -O2 -block-wm -watermark=${WATERMARK} -S -o $@ $< 2> ${@:%.ll=%-stderr.txt}

test/stage1-wm/%.ll: 8cc/%.c
	@mkdir -p ${@D}
	clang -DBUILD_DIR="\"$$PWD/8cc\"" -O2 -o $@ -emit-llvm -S $<

test/stage2-wm/%.o: 8cc/%.c 8cc-stage1-wm
	@mkdir -p ${@D}
	./8cc-stage1-wm -DBUILD_DIR="\"$$PWD/8cc\"" -w -o $@ -c $<

test/stage3-wm/%.o: 8cc/%.c 8cc-stage2-wm
	@mkdir -p ${@D}
	./8cc-stage2-wm -DBUILD_DIR="\"$$PWD/8cc\"" -w -o $@ -c $<

test: stage3 stage3-wm
	objdump -D 8cc-stage1 > stage1-asm.s
	objdump -D 8cc-stage2 > stage2-asm.s
	objdump -D 8cc-stage3 > stage3-asm.s
	objdump -D 8cc-stage1-wm > stage1-wm-asm.s
	objdump -D 8cc-stage2-wm > stage2-wm-asm.s
	objdump -D 8cc-stage3-wm > stage3-wm-asm.s

.PRECIOUS: test/stage1-wm/%.ll test/stage1-wm/%-wm.ll

clean:
	${RM} -r 8cc-* test *.s
