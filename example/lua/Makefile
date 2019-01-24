include ../../common.mk

WATERMARK := lua-watermarking

CFLAGS         := -DLUA_USE_LINUX
CFLAGS_debug   := -O0
CFLAGS_release := -O2 -DNDEBUG
CFLAGS         += ${CFLAGS_${BUILD_TYPE}}
LDFLAGS        := -lm -ldl

SRCS := \
	lua/lapi.c \
	lua/lauxlib.c \
	lua/lbaselib.c \
	lua/lcode.c \
	lua/lcorolib.c \
	lua/lctype.c \
	lua/ldblib.c \
	lua/ldebug.c \
	lua/ldo.c \
	lua/ldump.c \
	lua/lfunc.c \
	lua/lgc.c \
	lua/linit.c \
	lua/liolib.c \
	lua/llex.c \
	lua/lmathlib.c \
	lua/lmem.c \
	lua/loadlib.c \
	lua/lobject.c \
	lua/lopcodes.c \
	lua/loslib.c \
	lua/lparser.c \
	lua/lstate.c \
	lua/lstring.c \
	lua/lstrlib.c \
	lua/ltable.c \
	lua/ltablib.c \
	lua/ltests.c \
	lua/ltm.c \
	lua/lua.c \
	lua/lundump.c \
	lua/lutf8lib.c \
	lua/lvm.c \
	lua/lzio.c

LLS    := ${SRCS:lua/%.c=${OBJ_DIR}/%.ll}
LLS_WM := ${LLS:%.ll=%-wm.ll}
OBJS   := ${LLS:.ll=.o}
OBJS_WM := ${LLS_WM:.ll=.o}

.PRECIOUS: ${LLS} ${LLS_WM}

all: \
	${BIN_DIR}/lua \
	${BIN_DIR}/lua-wm

test: \
	${BIN_DIR}/lua \
	${BIN_DIR}/lua-wm

${BIN_DIR}/lua: ${OBJS}
	@mkdir -p ${@D}
	clang ${CFLAGS} -o $@ $^ ${LDFLAGS}

${BIN_DIR}/lua-wm: ${OBJS_WM}
	@mkdir -p ${@D}
	clang ${CFLAGS} -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${OBJ_DIR}/%.ll
	clang ${CFLAGS} -o $@ -c $^

${OBJ_DIR}/%.ll: lua/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -S -emit-llvm -o $@ $<

%-wm.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-block-wm.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=${WATERMARK} -S -o ${@:%-wm.ll=%-inst-wm.ll} ${@:%-wm.ll=%-block-wm.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -func-wm -watermark=${WATERMARK} -S -o $@ ${@:%-wm.ll=%-inst-wm.ll} 2> ${@:%-wm.ll=%-func-wm.txt}
