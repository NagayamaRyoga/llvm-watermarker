include ../../common.mk

RUSTFLAGS :=
RUSTFLAGS_debug :=
RUSTFLAGS_release := -O
RUSTFLAGS += ${RUSTFLAGS_${BUILD_TYPE}}

LDFLAGS := \
	-pthread \
	-lbacktrace \
	-ldl \
	-ljemalloc

all: ${BIN_DIR}/fizzbuzz.out ${BIN_DIR}/fizzbuzz-wm.out

test: ${BIN_DIR}/fizzbuzz.out ${BIN_DIR}/fizzbuzz-wm.out

${BIN_DIR}/%.out: ${OBJ_DIR}/%.ll ${OBJ_DIR}/probestack.o
	@mkdir -p ${@D}
	clang -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/fizzbuzz.ll: fizzbuzz.rs
	@mkdir -p ${@D}
	rustc ${RUSTFLAGS} -Clto --emit=llvm-ir -o $@ $<

${OBJ_DIR}/fizzbuzz-wm.ll: ${OBJ_DIR}/fizzbuzz.ll
	@mkdir -p ${@D}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=rust-project -S -o ${@:%-wm.ll=%-block.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=rust-project -S -o ${@:%-wm.ll=%-inst.ll} ${@:%-wm.ll=%-block.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -func-wm -watermark=rust-project -S -o $@ ${@:%-wm.ll=%-inst.ll} 2> ${@:%-wm.ll=%-func-wm.txt}

${OBJ_DIR}/probestack.o: probestack.s
	@mkdir -p ${@D}
	clang -o $@ -c $<
