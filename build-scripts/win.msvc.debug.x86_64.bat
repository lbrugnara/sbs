mkdir obj\\x86_64 
if not exist .\include (md .\include)
if exist ..\fllib\include\fllib.h (copy ..\fllib\include\fllib.h .\include\ )
if not exist .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug (            md .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug )
if exist ..\fllib\build\win-cmd\x86_64\msvc\msvc-debug\libfl.lib (             copy /Y ..\fllib\build\win-cmd\x86_64\msvc\msvc-debug\libfl.lib                  .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug\ )
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\action.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\archive.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\archive.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\build.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\build.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\compile.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\compile.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\executable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\executable.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\shared.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\shared.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\args.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\args.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cli.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cli.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdbuild.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdbuild.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdhelp.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdhelp.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdinit.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdinit.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdlist.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdlist.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdrun.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdrun.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\io.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\io.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\action.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\command.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\conditional.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\conditional.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\configuration.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\environment.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\file.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\action.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\command.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\conditional.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\conditional.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\configuration.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\environment.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\file.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\helpers.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\helpers.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\lexer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\lexer.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\parser.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\parser.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\preset.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\source.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\string.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\target.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\toolchain.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\variable.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\preset.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\source.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\string.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\target.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\toolchain.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\variable.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\main.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\result.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\result.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\action.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\configuration.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\context.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\context.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\environment.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\eval.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\eval.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\executor.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\executor.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\host.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\host.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\preset.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\resolve.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\resolve.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\string.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\target.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\toolchain.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\triplet.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\triplet.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\variable.o 
cl.exe  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\utils.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\utils.o 
link.exe /OUT:build\win-cmd\x86_64\msvc\msvc-debug\sbs.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL   obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\archive.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\build.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\compile.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\executable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\build\shared.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\args.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cli.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdbuild.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdhelp.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdinit.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdlist.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\cli\cmdrun.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\io.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\command.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\conditional.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\file.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\command.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\conditional.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\file.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\helpers.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\lexer.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\parser.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\source.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\parsers\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\source.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\lang\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\main.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\result.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\context.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\eval.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\executor.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\host.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\resolve.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\triplet.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\runtime\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs\src\utils.o .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug\libfl.lib 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\action.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\archive.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\archive.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\build.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\build.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\compile.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\compile.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\executable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\executable.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\build\shared.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\shared.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\args.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\args.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cli.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cli.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdbuild.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdbuild.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdhelp.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdhelp.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdinit.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdinit.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdlist.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdlist.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\cli\cmdrun.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdrun.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\io.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\io.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\action.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\command.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\conditional.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\conditional.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\configuration.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\environment.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\file.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\action.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\command.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\conditional.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\conditional.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\configuration.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\environment.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\file.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\helpers.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\helpers.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\lexer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\lexer.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\parser.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\parser.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\preset.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\source.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\string.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\target.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\toolchain.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\parsers\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\variable.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\preset.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\source.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\string.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\target.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\toolchain.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\lang\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\variable.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\main.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\result.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\result.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\action.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\configuration.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\context.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\context.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\environment.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\eval.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\eval.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\executor.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\executor.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\host.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\host.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\preset.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\resolve.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\resolve.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\string.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\string.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\target.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\toolchain.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\triplet.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\triplet.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\runtime\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\variable.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\src\utils.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\utils.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\integration\basic.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\integration\basic.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\Main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Main.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\parser\basic.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\parser\basic.o 
cl.exe /DSBS_TESTS  /I..\fllib\include  /W1 /Od /Z7 /DEBUG:FULL /c .\tests\Test.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test.o 
link.exe /OUT:build\win-cmd\x86_64\msvc\msvc-debug\tests.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE /DEBUG:FULL   obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\archive.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\build.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\compile.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\executable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\build\shared.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\args.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cli.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdbuild.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdhelp.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdinit.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdlist.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\cli\cmdrun.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\io.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\command.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\conditional.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\file.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\command.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\conditional.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\file.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\helpers.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\lexer.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\parser.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\source.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\parsers\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\source.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\lang\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\main.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\result.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\action.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\context.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\eval.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\executor.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\host.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\resolve.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\string.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\target.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\triplet.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\runtime\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\sbs-tests\src\utils.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\integration\basic.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Main.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\parser\basic.o obj\win-cmd\x86_64\msvc\msvc-debug\tests\Test.o .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug\libfl.lib 
