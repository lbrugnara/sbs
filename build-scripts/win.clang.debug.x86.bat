mkdir     obj\win\x86\clang\clang-debug\sbs\src     obj\win\x86\clang\clang-debug\sbs\src\build     obj\win\x86\clang\clang-debug\sbs\src\cli     obj\win\x86\clang\clang-debug\sbs\src\lang     obj\win\x86\clang\clang-debug\sbs\src\lang\parsers     obj\win\x86\clang\clang-debug\sbs\src\runtime 
mkdir     obj\win\x86\clang\clang-debug\sbs-tests\src     obj\win\x86\clang\clang-debug\sbs-tests\src\build     obj\win\x86\clang\clang-debug\sbs-tests\src\cli     obj\win\x86\clang\clang-debug\sbs-tests\src\lang     obj\win\x86\clang\clang-debug\sbs-tests\src\lang\parsers     obj\win\x86\clang\clang-debug\sbs-tests\src\runtime 
mkdir     obj\win\x86\clang\clang-debug\sbs\tests     obj\win\x86\clang\clang-debug\sbs\tests\integration     obj\win\x86\clang\clang-debug\sbs\tests\parser 
mkdir     obj\win\x86\clang\clang-debug\sbs-tests\tests     obj\win\x86\clang\clang-debug\sbs-tests\tests\integration     obj\win\x86\clang\clang-debug\sbs-tests\tests\parser 
mkdir build\win\x86\clang\clang-debug 
if not exist .\include (md .\include)
if exist ..\fllib\include\fllib.h (copy /Y ..\fllib\include\fllib.h .\include\ )
if not exist .\lib\fllib\win\x86\clang\clang-debug (            md .\lib\fllib\win\x86\clang\clang-debug )
if exist ..\fllib\build\win\x86\clang\clang-debug\libfl.lib (             copy /Y ..\fllib\build\win\x86\clang\clang-debug\libfl.lib                  .\lib\fllib\win\x86\clang\clang-debug\ )
clang-10.0.0-x86.bat
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\action.c -o obj\win\x86\clang\clang-debug\sbs\src\build\action.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\archive.c -o obj\win\x86\clang\clang-debug\sbs\src\build\archive.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\build.c -o obj\win\x86\clang\clang-debug\sbs\src\build\build.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\compile.c -o obj\win\x86\clang\clang-debug\sbs\src\build\compile.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\executable.c -o obj\win\x86\clang\clang-debug\sbs\src\build\executable.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\build\shared.c -o obj\win\x86\clang\clang-debug\sbs\src\build\shared.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\args.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\args.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cli.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cli.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdbuild.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdbuild.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdhelp.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdhelp.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdinit.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdinit.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdlist.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdlist.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdrun.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdrun.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\cli\cmdver.c -o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdver.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\io.c -o obj\win\x86\clang\clang-debug\sbs\src\io.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\action.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\action.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\command.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\command.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\configuration.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\configuration.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\cstring.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\cstring.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\environment.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\environment.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\expression.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\expression.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\file.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\file.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\helpers.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\helpers.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\lexer.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\lexer.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\parser.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\parser.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\parsers\expression.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\parsers\expression.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\preset.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\preset.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\source.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\source.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\string.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\string.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\target-archive.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\target-archive.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\target-compile.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\target-compile.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\target-executable.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\target-executable.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\target-shared.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\target-shared.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\target.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\target.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\lang\toolchain.c -o obj\win\x86\clang\clang-debug\sbs\src\lang\toolchain.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\main.c -o obj\win\x86\clang\clang-debug\sbs\src\main.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\result.c -o obj\win\x86\clang\clang-debug\sbs\src\result.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\action.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\action.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\configuration.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\configuration.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\context.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\context.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\environment.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\environment.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\eval.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\eval.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\executor.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\executor.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\host.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\host.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\preset.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\preset.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\resolve.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\resolve.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\target-archive.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-archive.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\target-compile.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-compile.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\target-executable.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-executable.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\target-shared.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-shared.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\target.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\target.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\toolchain.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\toolchain.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\runtime\triplet.c -o obj\win\x86\clang\clang-debug\sbs\src\runtime\triplet.o 
clang  -I..\fllib\include  -Wall -Werror -Wextra -pedantic -Wmissing-field-initializers -Wno-unused-value -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -fstrict-aliasing -Wvla -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_FORTIFY_SOURCE=2 -std=c99 --target=i386-pc-win32 -ggdb -O0 -DSBS_BUILD_ARCH=\"x86\" -DSBS_BUILD_OS=\"win\" -DSBS_BUILD_HOST_ARCH=\"x86_64\" -DSBS_BUILD_HOST_OS=\"win\" -DSBS_BUILD_COMPILER=\"clang\" -DSBS_BUILD_LINKER=\"link.exe\" -DSBS_BUILD_CONFIGURATION=\"clang-debug\" -c .\src\utils.c -o obj\win\x86\clang\clang-debug\sbs\src\utils.o 
link.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL /OUT:build\win\x86\clang\clang-debug\sbs.exe   obj\win\x86\clang\clang-debug\sbs\src\build\action.o obj\win\x86\clang\clang-debug\sbs\src\build\archive.o obj\win\x86\clang\clang-debug\sbs\src\build\build.o obj\win\x86\clang\clang-debug\sbs\src\build\compile.o obj\win\x86\clang\clang-debug\sbs\src\build\executable.o obj\win\x86\clang\clang-debug\sbs\src\build\shared.o obj\win\x86\clang\clang-debug\sbs\src\cli\args.o obj\win\x86\clang\clang-debug\sbs\src\cli\cli.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdbuild.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdhelp.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdinit.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdlist.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdrun.o obj\win\x86\clang\clang-debug\sbs\src\cli\cmdver.o obj\win\x86\clang\clang-debug\sbs\src\io.o obj\win\x86\clang\clang-debug\sbs\src\lang\action.o obj\win\x86\clang\clang-debug\sbs\src\lang\command.o obj\win\x86\clang\clang-debug\sbs\src\lang\configuration.o obj\win\x86\clang\clang-debug\sbs\src\lang\cstring.o obj\win\x86\clang\clang-debug\sbs\src\lang\environment.o obj\win\x86\clang\clang-debug\sbs\src\lang\expression.o obj\win\x86\clang\clang-debug\sbs\src\lang\file.o obj\win\x86\clang\clang-debug\sbs\src\lang\helpers.o obj\win\x86\clang\clang-debug\sbs\src\lang\lexer.o obj\win\x86\clang\clang-debug\sbs\src\lang\parser.o obj\win\x86\clang\clang-debug\sbs\src\lang\parsers\expression.o obj\win\x86\clang\clang-debug\sbs\src\lang\preset.o obj\win\x86\clang\clang-debug\sbs\src\lang\source.o obj\win\x86\clang\clang-debug\sbs\src\lang\string.o obj\win\x86\clang\clang-debug\sbs\src\lang\target-archive.o obj\win\x86\clang\clang-debug\sbs\src\lang\target-compile.o obj\win\x86\clang\clang-debug\sbs\src\lang\target-executable.o obj\win\x86\clang\clang-debug\sbs\src\lang\target-shared.o obj\win\x86\clang\clang-debug\sbs\src\lang\target.o obj\win\x86\clang\clang-debug\sbs\src\lang\toolchain.o obj\win\x86\clang\clang-debug\sbs\src\main.o obj\win\x86\clang\clang-debug\sbs\src\result.o obj\win\x86\clang\clang-debug\sbs\src\runtime\action.o obj\win\x86\clang\clang-debug\sbs\src\runtime\configuration.o obj\win\x86\clang\clang-debug\sbs\src\runtime\context.o obj\win\x86\clang\clang-debug\sbs\src\runtime\environment.o obj\win\x86\clang\clang-debug\sbs\src\runtime\eval.o obj\win\x86\clang\clang-debug\sbs\src\runtime\executor.o obj\win\x86\clang\clang-debug\sbs\src\runtime\host.o obj\win\x86\clang\clang-debug\sbs\src\runtime\preset.o obj\win\x86\clang\clang-debug\sbs\src\runtime\resolve.o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-archive.o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-compile.o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-executable.o obj\win\x86\clang\clang-debug\sbs\src\runtime\target-shared.o obj\win\x86\clang\clang-debug\sbs\src\runtime\target.o obj\win\x86\clang\clang-debug\sbs\src\runtime\toolchain.o obj\win\x86\clang\clang-debug\sbs\src\runtime\triplet.o obj\win\x86\clang\clang-debug\sbs\src\utils.o .\lib\fllib\win\x86\clang\clang-debug\libfl.lib 
