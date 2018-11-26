#!/bin/zsh
g() {
	SIZE=$(stat -c %s $1)
	LINES=$(wc -l $1 | awk '{print $1+1}')

	echo "$1, $SIZE, $LINES"
}

f() {
	for i in $@
	do g $i
	done
}

echo "name, size, lines"
f example/8cc/8cc/*.c
f example/zlib/zlib/*.c
