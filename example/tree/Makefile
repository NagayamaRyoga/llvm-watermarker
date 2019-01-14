include ../../common.mk

WATERMARK := tree-watermarking

CFLAGS         :=
CFLAGS_debug   := -O0
CFLAGS_release := -O2 -DNDEBUG
CFLAGS         += ${CFLAGS_${BUILD_TYPE}}
LDFLAGS        :=

SRCS := \
	tree/tree.c \
	tree/unix.c \
	tree/html.c \
	tree/xml.c \
	tree/json.c \
	tree/hash.c \
	tree/color.c

LLS    := ${SRCS:tree/%.c=${OBJ_DIR}/%.ll}
LLS_WM := ${LLS:%.ll=%-wm.ll}
OBJS   := ${LLS:.ll=.o}
OBJS_WM := ${LLS_WM:.ll=.o}

.PRECIOUS: ${LLS} ${LLS_WM}

all: \
	${BIN_DIR}/tree \
	${BIN_DIR}/tree-wm

test: \
	${BIN_DIR}/tree \
	${BIN_DIR}/tree-wm

${BIN_DIR}/tree: ${OBJS}
	@mkdir -p ${@D}
	clang ${CFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/tree-wm: ${OBJS_WM}
	@mkdir -p ${@D}
	clang ${CFLAGS} -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${OBJ_DIR}/%.ll
	clang ${CFLAGS} -o $@ -c $^

${OBJ_DIR}/%.ll: tree/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -S -emit-llvm -o $@ $<

%-wm.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-block-wm.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o $@ ${@:%-wm.ll=%-block-wm.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
