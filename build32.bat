REM Assumes environment set for visual c compiler.
mkdir Debug\assets >nul 2>nul
mkdir msvc\Debug >nul 2>nul
xcopy /Y /E assets Debug\assets
for /f "delims=" %%a in ( 'cd' ) do set LIBS=%%a\libs
for /f "delims=" %%a in ( 'dir libs\SDL2-* /b' ) do set SDL2=%LIBS%\%%a
for /f "delims=" %%a in ( 'dir libs\SDL2_ttf* /b' ) do set TTF=%LIBS%\%%a
for /f "delims=" %%a in ( 'dir libs\SDL2_mixer* /b' ) do set MIXER=%LIBS%\%%a
copy %SDL2%\lib\x86\*.dll Debug\
copy %MIXER%\lib\x86\*.dll Debug\
copy %TTF%\lib\x86\*.dll Debug\
cl -EHsc /DDEBUG /MD src\*.cpp /Fo.\msvc\Debug\  ^
/I %SDL2%\Include /I %TTF%\Include /I %MIXER%\Include  /link /LIBPATH:%SDL2%\lib\x86 /LIBPATH:%MIXER%\lib\x86 /LIBPATH:%TTF%\lib\x86  sdl2.lib sdl2main.lib SDL2_mixer.lib SDL2_ttf.lib /OUT:.\Debug\SDLP.exe /SUBSYSTEM:CONSOLE
