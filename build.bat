mkdir Debug\assets >nul 2>nul
xcopy /Y /E assets Debug\assets
set SDL2=libs\SDL2-2.0.5
set MIXR=libs\SDL2_mixer-2.0.0
set TTF=libs\SDL2_ttf-2.0.12
copy %SDL2%\lib\x86\*.dll Debug\
copy %MIXR%\lib\x86\*.dll Debug\
copy %TTF%\lib\x86\*.dll Debug\
cl -EHsc /DDEBUG /MD src\*.cpp /Fo.\vs2012\vs2012\Debug\  /I %SDL2%\Include /I %TTF%\Include /I %MIXR%\Include  /link /LIBPATH:%SDL2%\lib\x86 /LIBPATH:%MIXR%\lib\x86 /LIBPATH:%TTF%\lib\x86  sdl2.lib sdl2main.lib SDL2_mixer.lib SDL2_ttf.lib /OUT:.\Debug\SDLP.exe /SUBSYSTEM:CONSOLE
