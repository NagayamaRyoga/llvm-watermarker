#!/bin/zsh
g() {
	NAME=$1
	SIZE=$(stat -c %s $NAME)
	LINES=$(wc -l $NAME | awk '{print $1+1}')

	echo "name, $1, size, $SIZE, lines, $LINES"
}

f() {
	for i in $@
	do g $i
	done
}

f example/8cc/8cc/*.c
f example/zlib/zlib/*.c
