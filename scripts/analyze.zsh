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
	FILES=$(echo $R | wc -l)
	SIZE=$(echo $R | awk '{a+=$2} END {print a}')
	LINES=$(echo $R | awk '{a+=$3} END {print a}')

	echo "name, size, lines"
	echo "$R"
	echo "files: $FILES, size: $SIZE, lines: $LINES"
}

h `ls example/8cc/8cc/*.c | awk -F 'example/8cc/8cc/utiltest.c' '{print $NF}'`
h example/zlib/zlib/*.c
