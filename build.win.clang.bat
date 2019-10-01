set ARCH=%VSCMD_ARG_HOST_ARCH%

set CFLAGS= -std=c99 ^
            -Wall ^
            -Werror ^
            -Wextra ^
            -pedantic ^
            -O0 ^
            -Wmissing-field-initializers ^
            -Wno-unused-parameter ^
            -Wno-unused-variable ^
            -Wno-unused-function ^
            -Wno-missing-braces ^
            -fstrict-aliasing ^
            -Wvla ^
            -finput-charset=UTF-8 ^
            -fexec-charset=UTF-8 ^
            -ggdb ^
            -DFL_UNICODE_DB ^
            -D_FORTIFY_SOURCE=2 ^
            -I.\src -I..\fllib\include

set LIBSPATH=
set LIBS=..\fllib\build\debug\libfl.lib 

if "%~2%"=="sanitize" (
    set CFLAGS=%CFLAGS% -fsanitize=address,undefined
    if "%ARCH%"=="x64" (
        set LIBSPATH=%LIBSPATH% -LIBPATH:D:\dev\LLVM\x64\lib\clang\8.0.0\lib\windows
        set LIBS=%LIBS% clang_rt.ubsan_standalone-x86_64.lib clang_rt.asan-x86_64.lib clang_rt.builtins-x86_64.lib
    )
    if "%ARCH%"=="x86" (
        set LIBSPATH=%LIBSPATH% -LIBPATH:D:\dev\LLVM\x86\lib\clang\8.0.0\lib\windows
        set LIBS=%LIBS% clang_rt.ubsan_standalone-i386.lib clang_rt.asan-i386.lib clang_rt.builtins-i386.lib
    )
)

if "%~1%"=="sbs" (
    GOTO :SBS
)

if "%~1%"=="link" (
    GOTO :LINK
)

if "%~1%"=="clean" (
    GOTO :CLEAN
)

:: sbs
:SBS

rd /s /q "obj\"
rd /s /q "build\"
md .\obj\debug\objects .\obj\debug\parser .\obj\debug\build

clang.exe %CFLAGS% -c .\src\build\build.c  -o .\obj\debug\build\build.o
clang.exe %CFLAGS% -c .\src\build\archive.c  -o .\obj\debug\build\archive.o
clang.exe %CFLAGS% -c .\src\build\compile.c  -o .\obj\debug\build\compile.o
clang.exe %CFLAGS% -c .\src\build\executable.c  -o .\obj\debug\build\executable.o
clang.exe %CFLAGS% -c .\src\build\shared.c  -o .\obj\debug\build\shared.o

clang.exe %CFLAGS% -c .\src\parser\action.c  -o .\obj\debug\parser\action.o
clang.exe %CFLAGS% -c .\src\parser\common.c  -o .\obj\debug\parser\common.o
clang.exe %CFLAGS% -c .\src\parser\configuration.c  -o .\obj\debug\parser\configuration.o
clang.exe %CFLAGS% -c .\src\parser\environment.c  -o .\obj\debug\parser\environment.o
clang.exe %CFLAGS% -c .\src\parser\lexer.c  -o .\obj\debug\parser\lexer.o
clang.exe %CFLAGS% -c .\src\parser\parser.c  -o .\obj\debug\parser\parser.o
clang.exe %CFLAGS% -c .\src\parser\preset.c  -o .\obj\debug\parser\preset.o
clang.exe %CFLAGS% -c .\src\parser\target.c  -o .\obj\debug\parser\target.o
clang.exe %CFLAGS% -c .\src\parser\toolchain.c  -o .\obj\debug\parser\toolchain.o
clang.exe %CFLAGS% -c .\src\parser\file.c  -o .\obj\debug\parser\file.o

clang.exe %CFLAGS% -c .\src\objects\action.c  -o .\obj\debug\objects\action.o
clang.exe %CFLAGS% -c .\src\objects\configuration.c  -o .\obj\debug\objects\configuration.o
clang.exe %CFLAGS% -c .\src\objects\environment.c  -o .\obj\debug\objects\environment.o
clang.exe %CFLAGS% -c .\src\objects\preset.c  -o .\obj\debug\objects\preset.o
clang.exe %CFLAGS% -c .\src\objects\target.c  -o .\obj\debug\objects\target.o
clang.exe %CFLAGS% -c .\src\objects\toolchain.c  -o .\obj\debug\objects\toolchain.o

clang.exe %CFLAGS% -c .\src\commands.c  -o .\obj\debug\commands.o
clang.exe %CFLAGS% -c .\src\common.c  -o .\obj\debug\common.o
clang.exe %CFLAGS% -c .\src\executor.c  -o .\obj\debug\executor.o
clang.exe %CFLAGS% -c .\src\main.c  -o .\obj\debug\main.o
clang.exe %CFLAGS% -c .\src\result.c  -o .\obj\debug\result.o

:: Link
:LINK

md .\build\debug

lld-link.exe ^
    -OUT:build\debug\sbs.exe ^
    -DEFAULTLIB:libcmt ^
    -DEFAULTLIB:userenv ^
    -DEFAULTLIB:advapi32 ^
    -NOLOGO ^
    -DEBUG:FULL ^
    %LIBSPATH% ^
    %LIBS% ^
    obj\debug\build\build.o ^
    obj\debug\build\archive.o ^
    obj\debug\build\compile.o ^
    obj\debug\build\executable.o ^
    obj\debug\build\shared.o ^
    obj\debug\parser\action.o ^
    obj\debug\parser\common.o ^
    obj\debug\parser\configuration.o ^
    obj\debug\parser\environment.o ^
    obj\debug\parser\lexer.o ^
    obj\debug\parser\parser.o ^
    obj\debug\parser\preset.o ^
    obj\debug\parser\target.o ^
    obj\debug\parser\toolchain.o ^
    obj\debug\parser\file.o ^
    obj\debug\objects\action.o ^
    obj\debug\objects\configuration.o ^
    obj\debug\objects\environment.o ^
    obj\debug\objects\preset.o ^
    obj\debug\objects\target.o ^
    obj\debug\objects\toolchain.o ^
    obj\debug\common.o ^
    obj\debug\commands.o ^
    obj\debug\executor.o ^
    obj\debug\main.o ^
    obj\debug\result.o

GOTO :EXIT

:CLEAN

rd /s /q "obj\"
rd /s /q "build\"

:EXIT
