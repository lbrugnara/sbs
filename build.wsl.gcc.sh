#!/bin/bash

set -x

ARCH=$HOSTTYPE

CFLAGS="-std=gnu99 \
        -Wall \
        -Werror \
        -Wextra \
        -pedantic \
        -O0 \
        -Wmissing-field-initializers \
        -Wno-unused-parameter \
        -Wno-unused-variable \
        -Wno-unused-function \
        -Wno-missing-braces \
        -Wno-unused-value \
        -fstrict-aliasing \
        -finput-charset=UTF-8 \
        -fexec-charset=UTF-8 \
        -ggdb \
        -DFL_UNICODE_DB \
        -D_FORTIFY_SOURCE=2 \
        -I./src -I../fllib/include"

LIBSPATH=
LIBS=../fllib/build/debug/libfl.a 

if [ "$2" == "sanitize" ]
then
    CFLAGS+=-fsanitize=address,undefined
fi

function target_sbs() {
    rm -rf obj/
    rm -rf build/
    mkdir -p ./obj/debug

    gcc $CFLAGS -c ./src/file.c  -o ./obj/debug/file.o
    gcc $CFLAGS -c ./src/result.c  -o ./obj/debug/result.o
    gcc $CFLAGS -c ./src/lexer.c  -o ./obj/debug/lexer.o
    gcc $CFLAGS -c ./src/parser.c  -o ./obj/debug/parser.o
    gcc $CFLAGS -c ./src/executor.c  -o ./obj/debug/executor.o
    gcc $CFLAGS -c ./src/build.c  -o ./obj/debug/build.o
    gcc $CFLAGS -c ./src/commands.c  -o ./obj/debug/commands.o
    gcc $CFLAGS -c ./src/main.c  -o ./obj/debug/main.o
}

function target_link() {
    mkdir -p ./build/debug

    gcc obj/debug/file.o \
        obj/debug/result.o \
        obj/debug/lexer.o \
        obj/debug/parser.o \
        obj/debug/executor.o \
        obj/debug/build.o \
        obj/debug/commands.o \
        obj/debug/main.o \
        -L../fllib/build/debug -lfl \
        -L/lib/x86_64-linux-gnu \
        -lpthread \
        -lm \
        -o build/debug/sbs
}

function target_clean() {

    rm -rf obj/
    rm -rf build/
}


if [ "$1" == "sbs" ]
then
    target_sbs;
    target_link;
elif [ "$1" == "link" ]
then
    target_link;
elif [ "$1" == "clean" ]
then
    target_clean;
else
    target_sbs;
    target_link;
fi
