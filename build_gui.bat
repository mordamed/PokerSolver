@echo off
SET PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.1\mingw_64\bin;%PATH%
echo Cleaning previous build...
rmdir /s /q release debug 2>nul
del /q Makefile* .qmake.stash 2>nul

echo Running qmake...
qmake PokerSolverGUI.pro

echo Building GUI...
mingw32-make

if exist release\PokerSolverGUI.exe (
    echo.
    echo ========================================
    echo BUILD SUCCESS!
    echo ========================================
    echo Executable: release\PokerSolverGUI.exe
    echo.
    echo Launching GUI...
    start release\PokerSolverGUI.exe
) else (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
)
pause
