#!/bin/zsh
g1() {
	FILE=$(basename ${1/-block-wm.txt/.c})
	FUNCS=$(cat $1 | wc -l)
	BLOCKS=$(awk '{a+=$2} END {print a}' $1)
	BITS=$(awk '{a+=$3} END {print a}' $1)

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
	INSTS=$(awk '{a+=$2} END {print a}' $1)
	BITS=$(awk '{a+=$3} END {print a}' $1)

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

echo "==== 8cc block-wm ===="
h1 obj/$1/example/8cc/stage1-wm/*-block-wm.txt
echo "==== zlib block-wm ===="
h1 obj/$1/example/zlib/zlib-block-wm/*-block-wm.txt
echo "==== lua block-wm ===="
h1 obj/$1/example/lua/*-block-wm.txt
echo "==== fizzbuzz-rs block-wm ===="
h1 obj/$1/example/fizzbuzz-rs/*-block-wm.txt
echo "==== numguess-rs block-wm ===="
h1 obj/$1/example/numguess-rs/*-block-wm.txt

echo "==== 8cc inst-wm ===="
h2 obj/$1/example/8cc/stage1-wm/*-inst-wm.txt
echo "==== zlib inst-wm ===="
h2 obj/$1/example/zlib/zlib-inst-wm/*-inst-wm.txt
echo "==== lua inst-wm ===="
h2 obj/$1/example/lua/*-inst-wm.txt
echo "==== fizzbuzz-rs inst-wm ===="
h2 obj/$1/example/fizzbuzz-rs/*-inst-wm.txt
echo "==== numguess-rs inst-wm ===="
h2 obj/$1/example/numguess-rs/*-inst-wm.txt
