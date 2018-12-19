include ../../common.mk

WATERMARK := 77

CC := clang
CXX := clang++

CFLAGS := \
	-D_LARGEFILE64_SOURCE=1 \
	-DHAVE_HIDDEN \
	-O2

CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	-Izlib \
	-O2

SRCS := \
	zlib/adler32.c \
	zlib/compress.c \
	zlib/crc32.c \
	zlib/deflate.c \
	zlib/gzclose.c \
	zlib/gzlib.c \
	zlib/gzread.c \
	zlib/gzwrite.c \
	zlib/inflate.c \
	zlib/infback.c \
	zlib/inftrees.c \
	zlib/inffast.c \
	zlib/trees.c \
	zlib/uncompr.c \
	zlib/zutil.c

OBJS := ${SRCS:zlib/%.c=${OBJ_DIR}/zlib/%.o}
OBJS_BWM := ${SRCS:zlib/%.c=${OBJ_DIR}/zlib-block-wm/%.o}
OBJS_IWM := ${SRCS:zlib/%.c=${OBJ_DIR}/zlib-inst-wm/%.o}
OBJS_WM := ${SRCS:zlib/%.c=${OBJ_DIR}/zlib-wm/%.o}

.PHONY: all test

all: \
	${BIN_DIR}/test \
	${BIN_DIR}/test-block-wm \
	${BIN_DIR}/test-inst-wm \
	${BIN_DIR}/test-wm

test: \
	${BIN_DIR}/test \
	${BIN_DIR}/test-block-wm \
	${BIN_DIR}/test-inst-wm \
	${BIN_DIR}/test-wm

${BIN_DIR}/libz.a: ${OBJS}
${BIN_DIR}/libz-block-wm.a: ${OBJS_BWM}
${BIN_DIR}/libz-inst-wm.a: ${OBJS_IWM}
${BIN_DIR}/libz-wm.a: ${OBJS_WM}

${BIN_DIR}/test: ${OBJ_DIR}/test.o ${BIN_DIR}/libz.a
${BIN_DIR}/test-block-wm: ${OBJ_DIR}/test.o ${BIN_DIR}/libz-block-wm.a
${BIN_DIR}/test-inst-wm: ${OBJ_DIR}/test.o ${BIN_DIR}/libz-inst-wm.a
${BIN_DIR}/test-wm: ${OBJ_DIR}/test.o ${BIN_DIR}/libz-wm.a

${OBJ_DIR}/zlib/%.o: zlib/%.c
	@mkdir -p ${@D}
	${CC} ${CFLAGS} -o $@ -c $<

${OBJ_DIR}/zlib-block-wm/%.o: zlib/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -emit-llvm -S -o ${@:.o=.ll} $<
	opt -load=${ROOT}/bin/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%.o=%-block-wm.ll} ${@:.o=.ll} 2> ${@:%.o=%-block-wm.txt}
	clang ${CFLAGS} -o $@ -c ${@:%.o=%-block-wm.ll}

${OBJ_DIR}/zlib-inst-wm/%.o: zlib/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -emit-llvm -S -o ${@:.o=.ll} $<
	opt -load=${ROOT}/bin/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o ${@:%.o=%-inst-wm.ll} ${@:.o=.ll} 2> ${@:%.o=%-inst-wm.txt}
	clang ${CFLAGS} -o $@ -c ${@:%.o=%-inst-wm.ll}

${OBJ_DIR}/zlib-wm/%.o: zlib/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -emit-llvm -S -o ${@:.o=.ll} $<
	opt -load=${ROOT}/bin/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%.o=%-block-wm.ll} ${@:.o=.ll} 2> /dev/null
	opt -load=${ROOT}/bin/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o ${@:%.o=%-inst-wm.ll} ${@:.o=-block-wm.ll} 2> /dev/null
	clang ${CFLAGS} -o $@ -c ${@:%.o=%-inst-wm.ll}

${OBJ_DIR}/test.o: test.cpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ -c $<

${BIN_DIR}/test:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/test-block-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/test-inst-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/test-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

%.a:
	@mkdir -p ${@D}
	${AR} rcs $@ $^
