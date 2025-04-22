@echo off
REM Launch MSYS2 mingw64 shell, cd to glowing-umbrella, and run GlowingUmbrella.exe

REM Adjust this path if your MSYS2 is installed elsewhere
set MSYS2_ROOT=E:\MSYS2

REM Start mingw64.exe, cd to glowing-umbrella, and run the program
start "" "%MSYS2_ROOT%\mingw64.exe" bash -c "cd ~/glowing-umbrella && ./GlowingUmbrella.exe"
