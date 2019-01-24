include ../../common.mk

LDFLAGS := \
	-pthread \
	-lbacktrace \
	-ldl \
	-ljemalloc

all: ${BIN_DIR}/numguess.out ${BIN_DIR}/numguess-wm.out

test: ${BIN_DIR}/numguess.out ${BIN_DIR}/numguess-wm.out

clean::
	cargo clean

${BIN_DIR}/%.out: ${OBJ_DIR}/%.ll ${OBJ_DIR}/probestack.o
	@mkdir -p ${@D}
	clang -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/numguess.ll: target/release/deps/*.ll
	@mkdir -p ${@D}
	cp $< $@

target/release/deps/*.ll:
	cargo rustc --release -v -- -Clto --emit=llvm-ir

${OBJ_DIR}/numguess-wm.ll: ${OBJ_DIR}/numguess.ll
	@mkdir -p ${@D}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -block-wm -watermark=rust-project -S -o ${@:%-wm.ll=%-block.ll} $< 2> ${@:%-wm.ll=%-block-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -inst-wm -watermark=rust-project -S -o ${@:%-wm.ll=%-inst.ll} ${@:%-wm.ll=%-block.ll} 2> ${@:%-wm.ll=%-inst-wm.txt}
	opt -load=${ROOT}/bin/${BUILD_TYPE}/src/nykk.so -func-wm -watermark=rust-project -S -o $@ ${@:%-wm.ll=%-inst.ll} 2> ${@:%-wm.ll=%-func-wm.txt}

${OBJ_DIR}/probestack.o: probestack.s
	@mkdir -p ${@D}
	clang -o $@ -c $<
