mkdir     obj\win\x86\msvc\msvc-debug\sbs\src     obj\win\x86\msvc\msvc-debug\sbs\src\build     obj\win\x86\msvc\msvc-debug\sbs\src\cli     obj\win\x86\msvc\msvc-debug\sbs\src\lang     obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers     obj\win\x86\msvc\msvc-debug\sbs\src\runtime 
mkdir     obj\win\x86\msvc\msvc-debug\sbs-tests\src     obj\win\x86\msvc\msvc-debug\sbs-tests\src\build     obj\win\x86\msvc\msvc-debug\sbs-tests\src\cli     obj\win\x86\msvc\msvc-debug\sbs-tests\src\lang     obj\win\x86\msvc\msvc-debug\sbs-tests\src\lang\parsers     obj\win\x86\msvc\msvc-debug\sbs-tests\src\runtime 
mkdir     obj\win\x86\msvc\msvc-debug\sbs\tests     obj\win\x86\msvc\msvc-debug\sbs\tests\integration     obj\win\x86\msvc\msvc-debug\sbs\tests\parser 
mkdir     obj\win\x86\msvc\msvc-debug\sbs-tests\tests     obj\win\x86\msvc\msvc-debug\sbs-tests\tests\integration     obj\win\x86\msvc\msvc-debug\sbs-tests\tests\parser 
mkdir build\win\x86\msvc\msvc-debug 
if not exist .\include (md .\include)
if exist ..\fllib\include\fllib.h (copy /Y ..\fllib\include\fllib.h .\include\ )
if not exist .\lib\fllib\win\x86\msvc\msvc-debug (            md .\lib\fllib\win\x86\msvc\msvc-debug )
if exist ..\fllib\build\win\x86\msvc\msvc-debug\libfl.lib (             copy /Y ..\fllib\build\win\x86\msvc\msvc-debug\libfl.lib                  .\lib\fllib\win\x86\msvc\msvc-debug\ )
clang-10.0.0-x86.bat
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\action.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\action.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\archive.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\archive.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\build.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\build.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\compile.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\compile.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\executable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\executable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\shared.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\build\shared.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\args.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\args.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cli.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cli.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdbuild.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdbuild.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdhelp.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdhelp.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdinit.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdinit.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdlist.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdlist.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdrun.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdrun.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdver.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdver.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\io.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\io.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\action.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\action.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\command.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\command.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\conditional.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\conditional.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\configuration.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\configuration.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\environment.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\environment.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\file.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\file.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\action.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\action.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\command.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\command.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\conditional.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\conditional.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\configuration.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\configuration.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\environment.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\environment.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\expression.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\expression.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\file.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\file.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\helpers.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\helpers.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\lexer.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\lexer.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\parser.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\parser.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\preset.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\preset.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\source.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\source.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\string.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\string.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target-archive.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-archive.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target-compile.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-compile.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target-executable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-executable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target-shared.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-shared.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\toolchain.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\toolchain.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\variable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\variable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\preset.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\preset.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\source.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\source.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\string.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\string.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target-archive.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-archive.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target-compile.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-compile.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target-executable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-executable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target-shared.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-shared.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\target.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\toolchain.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\toolchain.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\variable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\lang\variable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\main.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\main.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\result.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\result.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\action.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\action.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\command.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\command.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\configuration.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\configuration.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\context.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\context.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\environment.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\environment.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\eval.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\eval.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\executor.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\executor.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\host.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\host.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\preset.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\preset.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\resolve.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\resolve.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\source.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\source.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target-archive.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-archive.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target-compile.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-compile.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target-executable.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-executable.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target-shared.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-shared.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\toolchain.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\toolchain.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\triplet.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\runtime\triplet.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\utils.c /Fo.\obj\win\x86\msvc\msvc-debug\sbs\src\utils.o /DSBS_BUILD_ARCH=\"x86\" /DSBS_BUILD_OS=\"win\" /DSBS_BUILD_HOST_ARCH=\"x86_64\" /DSBS_BUILD_HOST_OS=\"win\" /DSBS_BUILD_COMPILER=\"cl.exe\" /DSBS_BUILD_LINKER=\"link.exe\" /DSBS_BUILD_CONFIGURATION=\"msvc-debug\" 
link.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /MACHINE:x86 /DEBUG:FULL /OUT:build\win\x86\msvc\msvc-debug\sbs.exe   obj\win\x86\msvc\msvc-debug\sbs\src\build\action.o obj\win\x86\msvc\msvc-debug\sbs\src\build\archive.o obj\win\x86\msvc\msvc-debug\sbs\src\build\build.o obj\win\x86\msvc\msvc-debug\sbs\src\build\compile.o obj\win\x86\msvc\msvc-debug\sbs\src\build\executable.o obj\win\x86\msvc\msvc-debug\sbs\src\build\shared.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\args.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cli.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdbuild.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdhelp.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdinit.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdlist.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdrun.o obj\win\x86\msvc\msvc-debug\sbs\src\cli\cmdver.o obj\win\x86\msvc\msvc-debug\sbs\src\io.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\action.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\command.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\conditional.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\configuration.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\environment.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\file.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\action.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\command.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\conditional.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\configuration.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\environment.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\expression.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\file.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\helpers.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\lexer.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\parser.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\preset.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\source.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\string.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-archive.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-compile.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-executable.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target-shared.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\target.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\toolchain.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\parsers\variable.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\preset.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\source.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\string.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-archive.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-compile.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-executable.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\target-shared.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\target.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\toolchain.o obj\win\x86\msvc\msvc-debug\sbs\src\lang\variable.o obj\win\x86\msvc\msvc-debug\sbs\src\main.o obj\win\x86\msvc\msvc-debug\sbs\src\result.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\action.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\command.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\configuration.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\context.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\environment.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\eval.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\executor.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\host.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\preset.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\resolve.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\source.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-archive.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-compile.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-executable.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target-shared.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\target.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\toolchain.o obj\win\x86\msvc\msvc-debug\sbs\src\runtime\triplet.o obj\win\x86\msvc\msvc-debug\sbs\src\utils.o .\lib\fllib\win\x86\msvc\msvc-debug\libfl.lib 
