mkdir     obj\win-cmd\x86_64\clang\clang-debug\sbs\src     obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build     obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli     obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang     obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers     obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime 
mkdir     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src\build     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src\cli     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src\lang     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src\lang\parsers     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\src\runtime 
mkdir     obj\win-cmd\x86_64\clang\clang-debug\sbs\tests     obj\win-cmd\x86_64\clang\clang-debug\sbs\tests\integration     obj\win-cmd\x86_64\clang\clang-debug\sbs\tests\parser 
mkdir     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\tests     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\tests\integration     obj\win-cmd\x86_64\clang\clang-debug\sbs-tests\tests\parser 
mkdir build\win-cmd\x86_64\clang\clang-debug 
if not exist .\include (md .\include)
if exist ..\fllib\include\fllib.h (copy /Y ..\fllib\include\fllib.h .\include\ )
if not exist .\lib\fllib\win-cmd\x86_64\clang\clang-debug (            md .\lib\fllib\win-cmd\x86_64\clang\clang-debug )
if exist ..\fllib\build\win-cmd\x86_64\clang\clang-debug\libfl.lib (             copy /Y ..\fllib\build\win-cmd\x86_64\clang\clang-debug\libfl.lib                  .\lib\fllib\win-cmd\x86_64\clang\clang-debug\ )
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\action.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\action.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\archive.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\archive.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\build.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\build.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\compile.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\compile.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\executable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\executable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\build\shared.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\shared.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\args.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\args.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cli.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cli.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cmdbuild.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdbuild.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cmdhelp.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdhelp.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cmdinit.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdinit.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cmdlist.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdlist.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\cli\cmdrun.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdrun.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\io.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\io.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\action.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\action.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\command.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\command.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\conditional.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\conditional.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\configuration.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\configuration.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\environment.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\environment.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\file.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\file.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\action.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\action.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\command.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\command.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\conditional.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\conditional.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\configuration.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\configuration.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\environment.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\environment.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\file.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\file.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\helpers.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\helpers.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\lexer.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\lexer.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\parser.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\parser.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\preset.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\preset.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\source.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\source.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\string.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\string.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\target-archive.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-archive.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\target-compile.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-compile.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\target-executable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-executable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\target-shared.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-shared.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\target.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\toolchain.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\toolchain.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\parsers\variable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\variable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\preset.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\preset.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\source.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\source.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\string.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\string.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\target-archive.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-archive.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\target-compile.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-compile.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\target-executable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-executable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\target-shared.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-shared.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\target.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\toolchain.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\toolchain.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\lang\variable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\variable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\main.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\main.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\result.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\result.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\action.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\action.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\command.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\command.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\configuration.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\configuration.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\context.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\context.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\environment.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\environment.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\eval.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\eval.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\executor.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\executor.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\host.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\host.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\preset.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\preset.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\resolve.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\resolve.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\source.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\source.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\string.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\string.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\target-archive.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-archive.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\target-compile.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-compile.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\target-executable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-executable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\target-shared.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-shared.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\target.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\toolchain.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\toolchain.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\triplet.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\triplet.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\runtime\variable.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\variable.o 
clang  -I..\fllib\include  -std=c99 -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -ggdb -O0 -c .\src\utils.c -o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\utils.o 
link.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL /OUT:build\win-cmd\x86_64\clang\clang-debug\sbs.exe   obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\action.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\archive.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\build.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\compile.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\executable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\build\shared.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\args.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cli.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdbuild.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdhelp.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdinit.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdlist.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\cli\cmdrun.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\io.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\action.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\command.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\conditional.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\configuration.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\environment.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\file.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\action.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\command.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\conditional.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\configuration.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\environment.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\file.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\helpers.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\lexer.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\parser.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\preset.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\source.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\string.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-archive.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-compile.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-executable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target-shared.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\target.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\toolchain.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\parsers\variable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\preset.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\source.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\string.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-archive.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-compile.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-executable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target-shared.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\target.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\toolchain.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\lang\variable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\main.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\result.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\action.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\command.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\configuration.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\context.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\environment.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\eval.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\executor.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\host.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\preset.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\resolve.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\source.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\string.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-archive.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-compile.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-executable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target-shared.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\target.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\toolchain.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\triplet.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\runtime\variable.o obj\win-cmd\x86_64\clang\clang-debug\sbs\src\utils.o .\lib\fllib\win-cmd\x86_64\clang\clang-debug\libfl.lib 
