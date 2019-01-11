include ../../common.mk

WATERMARK := 77

CC := clang
CXX := clang++

CFLAGS := \
	-DNDEBUG \
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
	${BIN_DIR}/benchmark \
	${BIN_DIR}/benchmark-block-wm \
	${BIN_DIR}/benchmark-inst-wm \
	${BIN_DIR}/benchmark-wm

test: \
	${BIN_DIR}/benchmark \
	${BIN_DIR}/benchmark-block-wm \
	${BIN_DIR}/benchmark-inst-wm \
	${BIN_DIR}/benchmark-wm

benchmark:
	${BIN_DIR}/benchmark | tee ${BIN_DIR}/benchmark.txt
	${BIN_DIR}/benchmark-block-wm | tee ${BIN_DIR}/benchmark-block-wm.txt
	${BIN_DIR}/benchmark-inst-wm | tee ${BIN_DIR}/benchmark-inst-wm.txt
	${BIN_DIR}/benchmark-wm | tee ${BIN_DIR}/benchmark-wm.txt
	paste -d"," ${BIN_DIR}/benchmark.txt ${BIN_DIR}/benchmark-block-wm.txt ${BIN_DIR}/benchmark-inst-wm.txt ${BIN_DIR}/benchmark-wm.txt > ${BIN_DIR}/benchmark.csv

${BIN_DIR}/libz.a: ${OBJS}
${BIN_DIR}/libz-block-wm.a: ${OBJS_BWM}
${BIN_DIR}/libz-inst-wm.a: ${OBJS_IWM}
${BIN_DIR}/libz-wm.a: ${OBJS_WM}

${BIN_DIR}/benchmark: ${OBJ_DIR}/benchmark.o ${BIN_DIR}/libz.a
${BIN_DIR}/benchmark-block-wm: ${OBJ_DIR}/benchmark.o ${BIN_DIR}/libz-block-wm.a
${BIN_DIR}/benchmark-inst-wm: ${OBJ_DIR}/benchmark.o ${BIN_DIR}/libz-inst-wm.a
${BIN_DIR}/benchmark-wm: ${OBJ_DIR}/benchmark.o ${BIN_DIR}/libz-wm.a

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

${OBJ_DIR}/benchmark.o: benchmark.cpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ -c $<

${BIN_DIR}/benchmark:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/benchmark-block-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/benchmark-inst-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/benchmark-wm:
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

%.a:
	@mkdir -p ${@D}
	${AR} rcs $@ $^
