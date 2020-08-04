#!/bin/bash
set -x
mkdir -p obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/src/build obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/src/cli obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/src/lang/parsers obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/src/runtime
mkdir -p obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/tests/integration obj/linux-bash/x86_64/clang/clang-debug/sbs-tests/tests/parser
mkdir -p build/linux-bash/x86_64/clang/clang-debug
mkdir -p ./include
if [ -f "../fllib/include/fllib.h" ]; then cp ../fllib/include/fllib.h ./include/ ; fi
mkdir -p ./lib/fllib/linux-bash/x86_64/clang/clang-debug
if [ -f "../fllib/build/linux-bash/x86_64/clang/clang-debug/libfl.a" ]; then             cp  ../fllib/build/linux-bash/x86_64/clang/clang-debug/libfl.a                            ./lib/fllib/linux-bash/x86_64/clang/clang-debug/ ;                            fi
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/action.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/action.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/archive.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/archive.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/build.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/build.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/compile.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/compile.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/executable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/executable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/build/shared.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/shared.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/args.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/args.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cli.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cli.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cmdbuild.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdbuild.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cmdhelp.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdhelp.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cmdinit.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdinit.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cmdlist.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdlist.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/cli/cmdrun.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdrun.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/io.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/io.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/action.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/action.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/command.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/command.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/conditional.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/conditional.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/configuration.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/configuration.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/environment.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/environment.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/file.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/file.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/action.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/action.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/command.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/command.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/conditional.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/conditional.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/configuration.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/configuration.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/environment.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/environment.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/file.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/file.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/helpers.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/helpers.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/lexer.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/lexer.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/parser.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/parser.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/preset.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/preset.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/source.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/source.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/string.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/string.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/target-archive.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-archive.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/target-compile.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-compile.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/target-executable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-executable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/target-shared.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-shared.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/target.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/toolchain.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/toolchain.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/parsers/variable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/variable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/preset.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/preset.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/source.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/source.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/string.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/string.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/target-archive.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-archive.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/target-compile.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-compile.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/target-executable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-executable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/target-shared.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-shared.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/target.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/toolchain.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/toolchain.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/lang/variable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/variable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/main.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/main.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/result.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/result.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/action.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/action.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/command.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/command.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/configuration.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/configuration.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/context.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/context.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/environment.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/environment.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/eval.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/eval.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/executor.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/executor.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/host.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/host.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/preset.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/preset.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/resolve.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/resolve.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/source.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/source.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/string.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/string.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/target-archive.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-archive.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/target-compile.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-compile.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/target-executable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-executable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/target-shared.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-shared.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/target.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/toolchain.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/toolchain.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/triplet.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/triplet.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/runtime/variable.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/variable.o 
clang  -I../fllib/include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -fPIC -ggdb -O0 -c ./src/utils.c -o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/utils.o 
clang -o build/linux-bash/x86_64/clang/clang-debug/sbs   obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/action.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/archive.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/build.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/compile.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/executable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/build/shared.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/args.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cli.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdbuild.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdhelp.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdinit.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdlist.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/cli/cmdrun.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/io.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/action.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/command.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/conditional.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/configuration.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/environment.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/file.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/action.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/command.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/conditional.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/configuration.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/environment.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/file.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/helpers.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/lexer.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/parser.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/preset.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/source.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/string.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-archive.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-compile.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-executable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target-shared.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/target.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/toolchain.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/parsers/variable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/preset.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/source.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/string.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-archive.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-compile.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-executable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target-shared.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/target.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/toolchain.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/lang/variable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/main.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/result.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/action.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/command.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/configuration.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/context.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/environment.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/eval.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/executor.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/host.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/preset.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/resolve.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/source.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/string.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-archive.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-compile.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-executable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target-shared.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/target.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/toolchain.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/triplet.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/runtime/variable.o obj/linux-bash/x86_64/clang/clang-debug/sbs/src/utils.o ./lib/fllib/linux-bash/x86_64/clang/clang-debug/libfl.a -lm -lpthread 
