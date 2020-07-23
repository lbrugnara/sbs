#!/bin/bash
set -x
mkdir -p ./include
if [ -f "../fllib/include/fllib.h" ]; then cp ../fllib/include/fllib.h ./include/ ; fi
mkdir -p ./lib/fllib/linux-bash/x86_64/gcc/clang-debug
if [ -f "../fllib/build/linux-bash/x86_64/gcc/clang-debug/libfl.a" ]; then             cp  ../fllib/build/linux-bash/x86_64/gcc/clang-debug/libfl.a                            ./lib/fllib/linux-bash/x86_64/gcc/clang-debug/ ;                            fi
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/action.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/action.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/archive.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/archive.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/build.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/build.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/compile.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/compile.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/executable.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/executable.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/build/shared.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/build/shared.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/args.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/args.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cli.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cli.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cmdbuild.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdbuild.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cmdhelp.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdhelp.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cmdinit.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdinit.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cmdlist.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdlist.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/cli/cmdrun.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdrun.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/io.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/io.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/action.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/action.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/command.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/command.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/configuration.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/configuration.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/environment.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/environment.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/file.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/file.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/for.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/for.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/action.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/action.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/command.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/command.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/configuration.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/configuration.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/environment.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/environment.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/file.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/file.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/for.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/for.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/helpers.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/helpers.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/lexer.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/lexer.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/parser.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/parser.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/preset.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/preset.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/source.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/source.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/target.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/target.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/toolchain.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/toolchain.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/parsers/variable.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/variable.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/preset.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/preset.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/source.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/source.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/target.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/target.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/toolchain.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/toolchain.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/lang/variable.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/variable.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/main.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/main.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/result.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/result.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/action.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/action.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/configuration.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/configuration.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/context.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/context.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/environment.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/environment.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/eval.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/eval.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/executor.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/executor.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/host.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/host.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/preset.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/preset.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/target.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/target.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/runtime/toolchain.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/toolchain.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -c ./src/utils.c -o obj/linux-bash/x86_64/gcc/clang-debug/src/utils.o -fPIC -ggdb -O0 -DFL_DEBUG 
gcc -o build/linux-bash/x86_64/gcc/clang-debug/sbs   obj/linux-bash/x86_64/gcc/clang-debug/src/build/action.o obj/linux-bash/x86_64/gcc/clang-debug/src/build/archive.o obj/linux-bash/x86_64/gcc/clang-debug/src/build/build.o obj/linux-bash/x86_64/gcc/clang-debug/src/build/compile.o obj/linux-bash/x86_64/gcc/clang-debug/src/build/executable.o obj/linux-bash/x86_64/gcc/clang-debug/src/build/shared.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/args.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cli.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdbuild.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdhelp.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdinit.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdlist.o obj/linux-bash/x86_64/gcc/clang-debug/src/cli/cmdrun.o obj/linux-bash/x86_64/gcc/clang-debug/src/io.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/action.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/command.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/configuration.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/environment.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/file.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/for.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/action.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/command.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/configuration.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/environment.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/file.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/for.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/helpers.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/lexer.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/parser.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/preset.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/source.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/target.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/toolchain.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/parsers/variable.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/preset.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/source.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/target.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/toolchain.o obj/linux-bash/x86_64/gcc/clang-debug/src/lang/variable.o obj/linux-bash/x86_64/gcc/clang-debug/src/main.o obj/linux-bash/x86_64/gcc/clang-debug/src/result.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/action.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/configuration.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/context.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/environment.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/eval.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/executor.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/host.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/preset.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/target.o obj/linux-bash/x86_64/gcc/clang-debug/src/runtime/toolchain.o obj/linux-bash/x86_64/gcc/clang-debug/src/utils.o ./lib/fllib/linux-bash/x86_64/gcc/clang-debug/libfl.a -lm -lpthread 
