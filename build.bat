@echo off

set compiler=/W4 &:: Warning level 4
set compiler=%compiler% /WX &:: Warnings as errors
set compiler=%compiler% /Z7 &:: Emit leanish debug info
set compiler=%compiler% /MT &:: Static link CRT
set compiler=%compiler% /Od &:: No optimizations
set compiler=%compiler% /nologo
set compiler=%compiler% /Oi &:: Substitute instruction with intrinsics if possible
set compiler=%compiler% /wd4100 &:: Unused function parameter
set compiler=%compiler% /I ..\src\vendor\raylib &:: Include raylib code files

set linker=/LIBPATH:..\lib &:: Look for libraries in raylib lib folder
set linker=%linker% raylib.lib &:: Static link raylib
set linker=%linker% winmm.lib gdi32.lib opengl32.lib shell32.lib user32.lib &:: Static link windows stuff Raylib wants

set code=..\src\
set build=build\

if not exist %build% mkdir %build%
cd %build%

cl %compiler% %code%*.c /Fe:game.exe /link %linker%
