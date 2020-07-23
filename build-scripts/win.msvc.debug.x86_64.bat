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
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\io.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\io.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\command.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\configuration.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\environment.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\file.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\for.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\for.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\command.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\command.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\configuration.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\environment.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\file.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\file.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\for.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\for.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\helpers.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\helpers.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\lexer.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\lexer.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\parser.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\parser.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\preset.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\source.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\target.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\toolchain.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\parsers\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\variable.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\preset.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\source.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\source.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\target.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\toolchain.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\lang\variable.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\variable.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\main.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\main.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\result.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\result.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\action.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\action.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\configuration.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\configuration.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\context.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\context.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\environment.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\environment.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\eval.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\eval.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\executor.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\executor.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\host.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\host.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\preset.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\preset.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\target.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\target.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\runtime\toolchain.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\toolchain.o /Od /DEBUG:FULL 
cl.exe  /I..\fllib\include  /Zi /W1 /c .\src\utils.c /Fo.\\obj\win-cmd\x86_64\msvc\msvc-debug\src\utils.o /Od /DEBUG:FULL 
link.exe /OUT:build\win-cmd\x86_64\msvc\msvc-debug\sbs.exe /DEFAULTLIB:libcmt.lib /NOLOGO /SUBSYSTEM:CONSOLE   obj\win-cmd\x86_64\msvc\msvc-debug\src\build\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\archive.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\build.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\compile.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\executable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\build\shared.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\args.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cli.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdbuild.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdhelp.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdinit.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdlist.o obj\win-cmd\x86_64\msvc\msvc-debug\src\cli\cmdrun.o obj\win-cmd\x86_64\msvc\msvc-debug\src\io.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\command.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\file.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\for.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\command.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\file.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\for.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\helpers.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\lexer.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\parser.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\source.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\target.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\parsers\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\source.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\target.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\src\lang\variable.o obj\win-cmd\x86_64\msvc\msvc-debug\src\main.o obj\win-cmd\x86_64\msvc\msvc-debug\src\result.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\action.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\configuration.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\context.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\environment.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\eval.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\executor.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\host.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\preset.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\target.o obj\win-cmd\x86_64\msvc\msvc-debug\src\runtime\toolchain.o obj\win-cmd\x86_64\msvc\msvc-debug\src\utils.o .\lib\fllib\win-cmd\x86_64\msvc\msvc-debug\libfl.lib 
