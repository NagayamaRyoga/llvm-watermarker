include ../../common.mk

CC := clang
CXX := clang++

CFLAGS := \
	-D_LARGEFILE64_SOURCE=1 \
	-DHAVE_HIDDEN

CFLAGS_debug := -O0
CFLAGS_release := -O2 -DNDEBUG
CFLAGS += ${CFLAGS_${BUILD_TYPE}}

CXXFLAGS := \
	-std=c++17 \
	-Wall \
	-Wextra \
	-pedantic \
	-Werror \
	-Izlib

CXXFLAGS_debug := -O0
CXXFLAGS_release := -O2 -DNDEBUG
CXXFLAGS += ${CXXFLAGS_${BUILD_TYPE}}

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

# $(call watermark,<watermark>)
define watermark
.PRECIOUS: \
	$${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-block-wm.ll} \
	$${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-inst-wm.ll} \
	$${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-wm.ll}

all: \
	$${BIN_DIR}/$1/benchmark-block-wm \
	$${BIN_DIR}/$1/benchmark-inst-wm \
	$${BIN_DIR}/$1/benchmark-wm

test: \
	$${BIN_DIR}/$1/benchmark-block-wm \
	$${BIN_DIR}/$1/benchmark-inst-wm \
	$${BIN_DIR}/$1/benchmark-wm

benchmark:: \
	$${BIN_DIR}/$1/benchmark-block-wm \
	$${BIN_DIR}/$1/benchmark-inst-wm \
	$${BIN_DIR}/$1/benchmark-wm
	$${BIN_DIR}/$1/benchmark-block-wm | tee $${BIN_DIR}/$1/benchmark-block-wm.txt
	$${BIN_DIR}/$1/benchmark-inst-wm | tee $${BIN_DIR}/$1/benchmark-inst-wm.txt
	$${BIN_DIR}/$1/benchmark-wm | tee $${BIN_DIR}/$1/benchmark-wm.txt
	paste -d "\t" $${BIN_DIR}/benchmark.txt $${BIN_DIR}/$1/benchmark-block-wm.txt $${BIN_DIR}/$1/benchmark-inst-wm.txt $${BIN_DIR}/$1/benchmark-wm.txt > $${BIN_DIR}/$1/benchmark.csv

$${BIN_DIR}/$1/libz-block-wm.a: $${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-block-wm.o}
$${BIN_DIR}/$1/libz-inst-wm.a: $${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-inst-wm.o}
$${BIN_DIR}/$1/libz-wm.a: $${SRCS:zlib/%.c=$${OBJ_DIR}/$1/%-wm.o}

$${BIN_DIR}/$1/benchmark-%: $${OBJ_DIR}/benchmark.o $${BIN_DIR}/$1/libz-%.a
	@mkdir -p $${@D}
	$${CXX} -o $$@ $$^ $${LDFLAGS}

$${OBJ_DIR}/$1/%-block-wm.ll: $${OBJ_DIR}/%.ll
	@mkdir -p $${@D}
	opt -load=$${ROOT}/bin/$${BUILD_TYPE}/src/nykk.so -block-wm -watermark="$1" -S -o $$@ $$< 2> $${@:%.ll=%.txt}

$${OBJ_DIR}/$1/%-inst-wm.ll: $${OBJ_DIR}/%.ll
	@mkdir -p $${@D}
	opt -load=$${ROOT}/bin/$${BUILD_TYPE}/src/nykk.so -inst-wm -watermark="$1" -S -o $$@ $$< 2> $${@:%.ll=%.txt}

$${OBJ_DIR}/$1/%-wm.ll: $${OBJ_DIR}/$1/%-block-wm.ll
	opt -load=$${ROOT}/bin/$${BUILD_TYPE}/src/nykk.so -inst-wm -watermark="$1" -S -o $$@ $$< 2> /dev/null

$${OBJ_DIR}/$1/%.o: $${OBJ_DIR}/$1/%.ll
	clang -o $$@ -c $$<
endef

.PRECIOUS: ${SRCS:zlib/%.c=${OBJ_DIR}/%.ll}

all: ${BIN_DIR}/benchmark
test: ${BIN_DIR}/benchmark

benchmark:: ${BIN_DIR}/benchmark
	${BIN_DIR}/benchmark | tee ${BIN_DIR}/benchmark.txt

${BIN_DIR}/libz.a: ${SRCS:zlib/%.c=${OBJ_DIR}/%.o}

${BIN_DIR}/benchmark: ${OBJ_DIR}/benchmark.o ${BIN_DIR}/libz.a
	${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${OBJ_DIR}/%.ll: zlib/%.c
	@mkdir -p ${@D}
	clang ${CFLAGS} -S -emit-llvm -o $@ -c $<

${OBJ_DIR}/benchmark.o: benchmark.cpp
	@mkdir -p ${@D}
	${CXX} ${CXXFLAGS} -o $@ -c $<

%.a:
	@mkdir -p ${@D}
	${AR} rcs $@ $^

${OBJ_DIR}/%.o: ${OBJ_DIR}/%.ll
	clang ${CFLAGS} -o $@ -c $<

$(eval $(call watermark,zlib-watermark))
$(eval $(call watermark,92bb9dcfd3b9f053d35c77c4c8796c6a))
$(eval $(call watermark,48d92373664b84ba9399cb6072220ee5))
$(eval $(call watermark,28e2fb60e8dc7c19cf6f9a06a7582ef3))
#$(eval $(call watermark,e94d310dfb9d89a0f2dd50ce5132493d))
