include ../../common.mk

WATERMARK := 77

CC := clang

CFLAGS := \
	-D_LARGEFILE64_SOURCE=1 \
	-DHAVE_HIDDEN \
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
OBJS_WM := ${SRCS:zlib/%.c=${OBJ_DIR}/zlib-${WATERMARK}/%.o}

.PHONY: all test

all: ${BIN_DIR}/libz.a ${BIN_DIR}/libz-${WATERMARK}.a

test: ${BIN_DIR}/libz.a ${BIN_DIR}/libz-${WATERMARK}.a

${BIN_DIR}/libz.a: ${OBJS}
${BIN_DIR}/libz-${WATERMARK}.a: ${OBJS_WM}

${OBJ_DIR}/zlib/%.o: zlib/%.c
	@mkdir -p ${@D}
	${CC} ${CFLAGS} -o $@ -c $<

${OBJ_DIR}/zlib-${WATERMARK}/%.o: zlib/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -emit-llvm -S -o ${@:.o=.ll} $<
	opt -load=${ROOT}/bin/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%.o=%-${WATERMARK}.ll} ${@:.o=.ll} 2> ${@:%.o=%-log.txt}
	clang ${CFLAGS} -o $@ -c ${@:%.o=%-${WATERMARK}.ll}

%.a:
	@mkdir -p ${@D}
	${AR} rcs $@ $^
