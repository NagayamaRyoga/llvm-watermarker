#!/bin/zsh
g() {
	FILE=$(basename ${1/-log.txt/.c})
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
	FILES=$(echo $R | wc -l)
	FUNCS=$(echo $R | awk '{a+=$2} END {print a}')
	BLOCKS=$(echo $R | awk '{a+=$3} END {print a}')
	BITS=$(echo $R | awk '{a+=$4} END {print a}')

	echo "file, funcs, blocks, bits"
	echo $R
	echo "files: $FILES, funcs: $FUNCS, blocks: $BLOCKS, bits: $BITS"
}

h obj/example/8cc/stage1-wm/*-log.txt
h obj/example/zlib/zlib-77/*-log.txt
