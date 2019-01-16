include ../../common.mk

WATERMARK := jemalloc-watermarking

CFLAGS         := -std=gnu11 -Wno-ignored-attributes -D_GNU_SOURCE -Ijemalloc/include -Iinclude
CFLAGS_debug   := -O0
CFLAGS_release := -O2 -DNDEBUG
CFLAGS         += ${CFLAGS_${BUILD_TYPE}}

SRCS    := \
	jemalloc/src/arena.c \
	jemalloc/src/background_thread.c \
	jemalloc/src/base.c \
	jemalloc/src/bin.c \
	jemalloc/src/bitmap.c \
	jemalloc/src/ckh.c \
	jemalloc/src/ctl.c \
	jemalloc/src/div.c \
	jemalloc/src/extent.c \
	jemalloc/src/extent_dss.c \
	jemalloc/src/extent_mmap.c \
	jemalloc/src/hash.c \
	jemalloc/src/hook.c \
	jemalloc/src/jemalloc.c \
	jemalloc/src/large.c \
	jemalloc/src/log.c \
	jemalloc/src/malloc_io.c \
	jemalloc/src/mutex.c \
	jemalloc/src/mutex_pool.c \
	jemalloc/src/nstime.c \
	jemalloc/src/pages.c \
	jemalloc/src/prng.c \
	jemalloc/src/prof.c \
	jemalloc/src/rtree.c \
	jemalloc/src/sc.c \
	jemalloc/src/stats.c \
	jemalloc/src/sz.c \
	jemalloc/src/tcache.c \
	jemalloc/src/test_hooks.c \
	jemalloc/src/ticker.c \
	jemalloc/src/tsd.c \
	jemalloc/src/witness.c

OBJS    := ${SRCS:jemalloc/src/%.c=${OBJ_DIR}/%.o}
OBJS_WM := ${OBJS:%.o=%-wm.o}
LLS     := ${OBJS:.o=.ll}
LLS_WM  := ${OBJS_WM:.o=.ll}

.PRECIOUS: ${LLS} ${LLS_WM}

all: \
	${BIN_DIR}/libjemalloc.a \
	${BIN_DIR}/libjemalloc-wm.a

${BIN_DIR}/libjemalloc.a: ${OBJS}
${BIN_DIR}/libjemalloc-wm.a: ${OBJS_WM}

${BIN_DIR}/%.a:
	@mkdir -p ${@D}
	ar rcs $@ $^

${OBJ_DIR}/%.ll: jemalloc/src/%.c
	@mkdir -p ${@D}
	clang -S -emit-llvm ${CFLAGS} -o $@ $<

${OBJ_DIR}/%.o: ${OBJ_DIR}/%.ll
	@mkdir -p ${@D}
	clang -o $@ -c $<

${OBJ_DIR}/%-wm.ll: ${OBJ_DIR}/%.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-block-wm.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o $@ ${@:%-wm.ll=%-block-wm.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
