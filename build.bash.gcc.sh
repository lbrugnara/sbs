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
        -Wvla \
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
    mkdir -p ./obj/debug/{parser,objects,build}
    
    gcc $CFLAGS -c ./src/build/build.c  -o ./obj/debug/build/build.o
    gcc $CFLAGS -c ./src/build/archive.c  -o ./obj/debug/build/archive.o
    gcc $CFLAGS -c ./src/build/compile.c  -o ./obj/debug/build/compile.o
    gcc $CFLAGS -c ./src/build/executable.c  -o ./obj/debug/build/executable.o
    gcc $CFLAGS -c ./src/build/shared.c  -o ./obj/debug/build/shared.o

    gcc $CFLAGS -c ./src/parser/action.c  -o ./obj/debug/parser/action.o
    gcc $CFLAGS -c ./src/parser/common.c  -o ./obj/debug/parser/common.o
    gcc $CFLAGS -c ./src/parser/configuration.c  -o ./obj/debug/parser/configuration.o
    gcc $CFLAGS -c ./src/parser/environment.c  -o ./obj/debug/parser/environment.o
    gcc $CFLAGS -c ./src/parser/lexer.c  -o ./obj/debug/parser/lexer.o
    gcc $CFLAGS -c ./src/parser/parser.c  -o ./obj/debug/parser/parser.o
    gcc $CFLAGS -c ./src/parser/preset.c  -o ./obj/debug/parser/preset.o
    gcc $CFLAGS -c ./src/parser/target.c  -o ./obj/debug/parser/target.o
    gcc $CFLAGS -c ./src/parser/toolchain.c  -o ./obj/debug/parser/toolchain.o
    gcc $CFLAGS -c ./src/parser/file.c  -o ./obj/debug/parser/file.o

    gcc $CFLAGS -c ./src/objects/action.c  -o ./obj/debug/objects/action.o
    gcc $CFLAGS -c ./src/objects/configuration.c  -o ./obj/debug/objects/configuration.o
    gcc $CFLAGS -c ./src/objects/environment.c  -o ./obj/debug/objects/environment.o
    gcc $CFLAGS -c ./src/objects/preset.c  -o ./obj/debug/objects/preset.o
    gcc $CFLAGS -c ./src/objects/target.c  -o ./obj/debug/objects/target.o
    gcc $CFLAGS -c ./src/objects/toolchain.c  -o ./obj/debug/objects/toolchain.o
    
    gcc $CFLAGS -c ./src/commands.c  -o ./obj/debug/commands.o
    gcc $CFLAGS -c ./src/common.c  -o ./obj/debug/common.o
    gcc $CFLAGS -c ./src/executor.c  -o ./obj/debug/executor.o
    gcc $CFLAGS -c ./src/main.c  -o ./obj/debug/main.o
    gcc $CFLAGS -c ./src/result.c  -o ./obj/debug/result.o
}

function target_link() {
    mkdir -p ./build/debug

    gcc obj/debug/build/build.o \
        obj/debug/build/archive.o \
        obj/debug/build/compile.o \
        obj/debug/build/executable.o \
        obj/debug/build/shared.o \
        obj/debug/parser/action.o \
        obj/debug/parser/common.o \
        obj/debug/parser/configuration.o \
        obj/debug/parser/environment.o \
        obj/debug/parser/lexer.o \
        obj/debug/parser/parser.o \
        obj/debug/parser/preset.o \
        obj/debug/parser/target.o \
        obj/debug/parser/toolchain.o \
        obj/debug/parser/file.o \
        obj/debug/objects/action.o \
        obj/debug/objects/configuration.o \
        obj/debug/objects/environment.o \
        obj/debug/objects/preset.o \
        obj/debug/objects/target.o \
        obj/debug/objects/toolchain.o \
        obj/debug/commands.o \
        obj/debug/common.o \
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
