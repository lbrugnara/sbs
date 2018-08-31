#!/bin/bash
set -x

CC=${CC:-gcc}
AR=${AR:-ar}

cd ../fllib
#make clean
make CC=$CC AR=$AR

cd ../sbs


# fllib
# =====
INCLUDES+=" -I../fllib/include"
LIBS+=" -L../fllib/build/debug -lfl"

# Sources
# =======
SOURCES="src/main.c src/sbs.c src/lexer.c src/parser.c"

FLAGS="-Wall -Wextra -Wno-switch -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=gnu99 -pedantic -fstrict-aliasing -finput-charset=UTF-8 -fexec-charset=UTF-8 -DFL_UNICODE_DB -ggdb -DFL_DEBUG -D__USE_MINGW_ANSI_STDIO=1"

# $CC $FLAGS $INCLUDES -c $SOURCES -o sbs.o
$CC $FLAGS $INCLUDES $SOURCES -o sbs.exe ../fllib/build/debug/libfl.a