#!/bin/zsh
g1() {
	FILE=$(basename ${1/-block-wm.txt/.c})
	FUNCS=$(tail -n +2 $1 | wc -l)
	BLOCKS=$(tail -n +2 $1 | awk '{a+=$2} END {print a}')
	BITS=$(tail -n +2 $1 | awk '{a+=$3} END {print a}')

	echo "$FILE, $FUNCS, $BLOCKS, $BITS"
}

f1() {
	for i in "$@"
	do g1 $i
	done
}

h1() {
	R=$(f1 $@)

	echo "file, funcs, blocks, bits"
	echo "$R"
	echo "$R" | awk '{f++; a+=$2; b+=$3; c+=$4} END {print "files: " f ", funcs: " a ", blocks: " b ", bits: " c}'
}

g2() {
	FILE=$(basename ${1/-inst-wm.txt/.c})
	INSTS=$(tail -n +2 $1 | awk '{a+=$2} END {print a}')
	BITS=$(tail -n +2 $1 | awk '{a+=$3} END {print a}')

	echo "$FILE, $INSTS, $BITS"
}

f2() {
	for i in "$@"
	do g2 $i
	done
}

h2() {
	R=$(f2 $@)

	echo "file, insts, bits"
	echo "$R"
	echo "$R" | awk '{f++; a+=$2; b+=$3} END {print "files: " f ", insts: " a ", bits: " b}'
}

h1 obj/example/8cc/stage1-wm/*-block-wm.txt
h1 obj/example/zlib/zlib-wm/*-block-wm.txt

h2 obj/example/8cc/stage1-wm/*-inst-wm.txt
h2 obj/example/zlib/zlib-wm/*-inst-wm.txt
