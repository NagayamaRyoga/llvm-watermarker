ROOT := $(realpath ${PWD})
CD   := $(realpath ${CURDIR})

BIN_DIR := ${ROOT}/bin/${CD:${ROOT}/%=%}
OBJ_DIR := ${ROOT}/obj/${CD:${ROOT}/%=%}

.PHONY: all test clean
