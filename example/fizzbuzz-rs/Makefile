include ../../common.mk

LDFLAGS := \
	-pthread \
	-lbacktrace \
	-ldl \
	-ljemalloc

all: ${BIN_DIR}/fizzbuzz.out #${BIN_DIR}/fizzbuzz-wm.out

${BIN_DIR}/fizzbuzz.out: ${OBJ_DIR}/fizzbuzz.ll ${OBJ_DIR}/probestack.o
	@mkdir -p ${@D}
	clang -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/fizzbuzz.ll: fizzbuzz.rs
	@mkdir -p ${@D}
	rustc -O -Clto --emit=llvm-ir -o $@ $<

${OBJ_DIR}/probestack.o: probestack.s
	@mkdir -p ${@D}
	clang -o $@ -c $<
