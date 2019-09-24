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
md .\obj\debug\objects
md .\obj\debug\build
md .\obj\debug\actions


clang.exe %CFLAGS% -c .\src\objects\action.c  -o .\obj\debug\objects\action.o
clang.exe %CFLAGS% -c .\src\objects\common.c  -o .\obj\debug\objects\common.o
clang.exe %CFLAGS% -c .\src\objects\configuration.c  -o .\obj\debug\objects\configuration.o
clang.exe %CFLAGS% -c .\src\objects\environment.c  -o .\obj\debug\objects\environment.o
clang.exe %CFLAGS% -c .\src\objects\lexer.c  -o .\obj\debug\objects\lexer.o
clang.exe %CFLAGS% -c .\src\objects\parser.c  -o .\obj\debug\objects\parser.o
clang.exe %CFLAGS% -c .\src\objects\preset.c  -o .\obj\debug\objects\preset.o
clang.exe %CFLAGS% -c .\src\objects\target.c  -o .\obj\debug\objects\target.o
clang.exe %CFLAGS% -c .\src\objects\toolchain.c  -o .\obj\debug\objects\toolchain.o
clang.exe %CFLAGS% -c .\src\objects\file.c  -o .\obj\debug\objects\file.o
clang.exe %CFLAGS% -c .\src\actions\actions.c  -o .\obj\debug\actions\actions.o
clang.exe %CFLAGS% -c .\src\build\build.c  -o .\obj\debug\build\build.o
clang.exe %CFLAGS% -c .\src\build\archive.c  -o .\obj\debug\build\archive.o
clang.exe %CFLAGS% -c .\src\build\compile.c  -o .\obj\debug\build\compile.o
clang.exe %CFLAGS% -c .\src\commands.c  -o .\obj\debug\commands.o
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
    obj\debug\objects\action.o ^
    obj\debug\objects\common.o ^
    obj\debug\objects\configuration.o ^
    obj\debug\objects\environment.o ^
    obj\debug\objects\lexer.o ^
    obj\debug\objects\parser.o ^
    obj\debug\objects\preset.o ^
    obj\debug\objects\target.o ^
    obj\debug\objects\toolchain.o ^
    obj\debug\objects\file.o ^
    obj\debug\actions\actions.o ^
    obj\debug\build\build.o ^
    obj\debug\build\archive.o ^
    obj\debug\build\compile.o ^
    obj\debug\commands.o ^
    obj\debug\executor.o ^
    obj\debug\main.o ^
    obj\debug\result.o

GOTO :EXIT

:CLEAN

rd /s /q "obj\"
rd /s /q "build\"

:EXIT
