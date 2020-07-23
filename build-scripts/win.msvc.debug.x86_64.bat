mkdir obj\\x86_64 
if not exist .\\include (md .\\include)
if exist ..\\fllib\\include\\fllib.h (copy ..\\fllib\\include\\fllib.h .\\include\\)
if not exist .\\lib\\fllib\\win-cmd\\x86_64\\msvc\\msvc-debug (md .\\lib\\fllib\\win-cmd\\x86_64\\msvc\\msvc-debug)
if exist ..\\fllib\\build\\win-cmd\\x86_64\\msvc\\msvc-debug\\libfl.lib (copy ..\\fllib\\build\\win-cmd\\x86_64\\msvc\\msvc-debug\\libfl.lib .\\lib\\fllib\\win-cmd\\x86_64\\msvc\\msvc-debug\\)
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\archive.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\archive.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\build.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\build.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\compile.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\compile.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\executable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\executable.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\build\shared.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\build\shared.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\args.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\args.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cli.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cli.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cmdbuild.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdbuild.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cmdhelp.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdhelp.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cmdinit.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdinit.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cmdlist.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdlist.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\cli\cmdrun.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdrun.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\common\common.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\common\common.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\common\result.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\common\result.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\context.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\context.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\executor.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\executor.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\host.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\host.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\io.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\io.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\main.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\configuration.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\environment.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\for.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\for.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\preset.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\target.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\objects\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\toolchain.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\configuration.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\environment.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\file.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\for.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\for.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\helpers.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\helpers.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\lexer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\lexer.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\parser.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\parser.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\preset.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\target.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\toolchain.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\parser\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\variable.o /Od /DEBUG:FULL 
link.exe /OUT:build\win-cmd\x86_64\msvc\msvc-debug\sbs.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE   obj\win-cmd\x86_64\msvc\msvc-debug\src\build\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\archive.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\build.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\compile.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\executable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\shared.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\args.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cli.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdbuild.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdhelp.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdinit.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdlist.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdrun.o obj\win-cmd\x86_64\msvc\msvc-debug\src\common\common.o obj\win-cmd\x86_64\msvc\msvc-debug\src\common\result.o obj\win-cmd\x86_64\msvc\msvc-debug\src\context.o obj\win-cmd\x86_64\msvc\msvc-debug\src\executor.o obj\win-cmd\x86_64\msvc\msvc-debug\src\host.o obj\win-cmd\x86_64\msvc\msvc-debug\src\io.o obj\win-cmd\x86_64\msvc\msvc-debug\src\main.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\for.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\target.o obj\win-cmd\x86_64\msvc\msvc-debug\src\objects\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\file.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\for.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\helpers.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\lexer.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\parser.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\target.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\src\parser\variable.o .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug\libfl.lib 
