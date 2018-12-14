#!/bin/zsh
g() {
	FILE=$(basename ${1/-block-wm.txt/.c})
	FUNCS=$(tail -n +2 $1 | wc -l)
	BLOCKS=$(tail -n +2 $1 | awk '{a+=$2} END {print a}')
	BITS=$(tail -n +2 $1 | awk '{a+=$3} END {print a}')

	echo "$FILE, $FUNCS, $BLOCKS, $BITS"
}

f() {
	for i in "$@"
	do g $i
	done
}

h() {
	R=$(f $@)

	echo "file, funcs, blocks, bits"
	echo "$R"
	echo "$R" | awk '{f++; a+=$2; b+=$3; c+=$4} END {print "files: " f ", funcs: " a ", blocks: " b ", bits: " c}'
}

h obj/example/8cc/stage1-wm/*-block-wm.txt
h obj/example/zlib/zlib-wm/*-block-wm.txt
