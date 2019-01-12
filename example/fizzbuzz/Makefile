include ../../common.mk

SRCS := \
	fizzbuzz.c

LLS := ${SRCS:%.c=${OBJ_DIR}/%.ll}
LLS_0 := ${LLS:%.ll=%-0.ll}
LLS_1 := ${LLS:%.ll=%-1.ll}
LLS_2 := ${LLS:%.ll=%-2.ll}
LLS_3 := ${LLS:%.ll=%-3.ll}
BINS := \
	${LLS:${OBJ_DIR}/%.ll=${BIN_DIR}/%.out} \
	${LLS_0:${OBJ_DIR}/%.ll=${BIN_DIR}/%.out} \
	${LLS_1:${OBJ_DIR}/%.ll=${BIN_DIR}/%.out} \
	${LLS_2:${OBJ_DIR}/%.ll=${BIN_DIR}/%.out} \
	${LLS_3:${OBJ_DIR}/%.ll=${BIN_DIR}/%.out}

all: ${BINS}

test: ${BINS}
	for i in ${BINS}; \
	do $$i > $$i.txt; \
	done

${BINS}: ${LLS} ${LLS_0} ${LLS_1} ${LLS_2} ${LLS_3}

${OBJ_DIR}/%.ll: %.c
	@mkdir -p ${@D}
	clang -emit-llvm -S -DNDEBUG -O2 -o $@ $<

%-0.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=0 -S -o $@ $< 2> ${@:.ll=.txt}
%-1.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=1 -S -o $@ $< 2> ${@:.ll=.txt}
%-2.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=2 -S -o $@ $< 2> ${@:.ll=.txt}
%-3.ll: %.ll
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=3 -S -o $@ $< 2> ${@:.ll=.txt}

${BIN_DIR}/%.out: ${OBJ_DIR}/%.ll
	@mkdir -p ${@D}
	clang -o $@ $<
