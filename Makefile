.PHONY: all test clean

all:
	${MAKE} -C src     all
	${MAKE} -C test    all
	${MAKE} -C example all

test:
	${MAKE} -C src     test
	${MAKE} -C test    test
	${MAKE} -C example test

clean:
	${RM} -r bin obj
