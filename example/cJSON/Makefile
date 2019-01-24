include ../../common.mk

WATERMARK := cJSON-watermarking

CFLAGS         :=
CFLAGS_debug   := -O0
CFLAGS_release := -O2 -DNDEBUG
CFLAGS         += ${CFLAGS_${BUILD_TYPE}}

SRCS := \
	cJSON/cJSON.c \
	cJSON/cJSON_Utils.c

LLS    := ${SRCS:cJSON/%.c=${OBJ_DIR}/%.ll}
LLS_WM := ${LLS:%.ll=%-wm.ll}
OBJS   := ${LLS:.ll=.o}
OBJS_WM := ${LLS_WM:.ll=.o}

.PRECIOUS: ${LLS} ${LLS_WM}

all: \
	${BIN_DIR}/libcjson.a \
	${BIN_DIR}/libcjson-wm.a

test: \
	${BIN_DIR}/libcjson.a \
	${BIN_DIR}/libcjson-wm.a

${BIN_DIR}/libcjson.a: ${OBJS}
${BIN_DIR}/libcjson-wm.a: ${OBJS_WM}

%.a:
	@mkdir -p ${@D}
	${AR} rcs $@ $^

${OBJ_DIR}/%.o: ${OBJ_DIR}/%.ll
	clang ${CFLAGS} -o $@ -c $^

${OBJ_DIR}/%.ll: cJSON/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -S -emit-llvm -o $@ $<

%-wm.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-block-wm.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-inst-wm.ll} ${@:%-wm.ll=%-block-wm.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -func-wm -watermark=${WATERMARK} -S -o $@ ${@:%-wm.ll=%-inst-wm.ll} 2> ${@:%-wm.ll=%-func-wm.txt}
