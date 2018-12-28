include ../../common.mk

LDFLAGS := \
	-pthread \
	-lbacktrace \
	-ldl \
	-ljemalloc

all: ${BIN_DIR}/numguess.out

test: ${BIN_DIR}/numguess.out

clean::
	cargo clean

${BIN_DIR}/numguess.out:
	@mkdir -p ${@D}
	cargo rustc --release -v -- -Clto --emit=llvm-ir
	clang -o $@ target/release/deps/*.ll probestack.s ${LDFLAGS}
