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
    mkdir -p ./obj/debug/{objects,build,actions}
    
    gcc $CFLAGS -c ./src/objects/action.c  -o ./obj/debug/objects/action.o
    gcc $CFLAGS -c ./src/objects/common.c  -o ./obj/debug/objects/common.o
    gcc $CFLAGS -c ./src/objects/configuration.c  -o ./obj/debug/objects/configuration.o
    gcc $CFLAGS -c ./src/objects/environment.c  -o ./obj/debug/objects/environment.o
    gcc $CFLAGS -c ./src/objects/lexer.c  -o ./obj/debug/objects/lexer.o
    gcc $CFLAGS -c ./src/objects/parser.c  -o ./obj/debug/objects/parser.o
    gcc $CFLAGS -c ./src/objects/preset.c  -o ./obj/debug/objects/preset.o
    gcc $CFLAGS -c ./src/objects/target.c  -o ./obj/debug/objects/target.o
    gcc $CFLAGS -c ./src/objects/toolchain.c  -o ./obj/debug/objects/toolchain.o
    gcc $CFLAGS -c ./src/objects/file.c  -o ./obj/debug/objects/file.o
    gcc $CFLAGS -c ./src/actions/actions.c  -o ./obj/debug/actions/actions.o
    gcc $CFLAGS -c ./src/build/build.c  -o ./obj/debug/build/build.o
    gcc $CFLAGS -c ./src/build/archive.c  -o ./obj/debug/build/archive.o
    gcc $CFLAGS -c ./src/build/compile.c  -o ./obj/debug/build/compile.o
    gcc $CFLAGS -c ./src/commands.c  -o ./obj/debug/commands.o
    gcc $CFLAGS -c ./src/executor.c  -o ./obj/debug/executor.o
    gcc $CFLAGS -c ./src/main.c  -o ./obj/debug/main.o
    gcc $CFLAGS -c ./src/result.c  -o ./obj/debug/result.o
}

function target_link() {
    mkdir -p ./build/debug

    gcc obj/debug/objects/action.o \
        obj/debug/objects/common.o \
        obj/debug/objects/configuration.o \
        obj/debug/objects/environment.o \
        obj/debug/objects/lexer.o \
        obj/debug/objects/parser.o \
        obj/debug/objects/preset.o \
        obj/debug/objects/target.o \
        obj/debug/objects/toolchain.o \
        obj/debug/objects/file.o \
        obj/debug/actions/actions.o \
        obj/debug/build/build.o \
        obj/debug/build/archive.o \
        obj/debug/build/compile.o \
        obj/debug/commands.o \
        obj/debug/executor.o \
        obj/debug/main.o \
        obj/debug/result.o \
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
