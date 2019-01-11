#!/bin/zsh
g() {
	FILE=$(basename $1)
	SIZE=$(stat -c %s $1)
	LINES=$(wc -l $1 | awk '{print $1+1}')

	echo "$FILE, $SIZE, $LINES"
}

f() {
	for i in $@
	do g $i
	done
}

h() {
	R=$(f $@)

	echo "name, size, lines"
	echo "$R"
	echo "$R" | awk '{f++; s+=$2; l+=$3} END {print "files: " f ", size: " s ", lines: " l}'
}

h `ls example/8cc/8cc/*.c | awk -F 'example/8cc/8cc/utiltest.c' '{print $NF}'`
h example/zlib/zlib/*.c
h example/lua/lua/*.c
