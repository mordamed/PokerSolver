@echo off
REM Clean script for PokerSolver project
REM Removes all build artifacts and temporary files

echo ========================================
echo Cleaning PokerSolver Project...
echo ========================================
echo.

REM Clean Qt GUI builds
echo [1/6] Cleaning Qt GUI builds (debug, release)...
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release

REM Clean CLI builds
echo [2/6] Cleaning CLI builds...
if exist Makefile_CLI (
    C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe -f Makefile_CLI clean 2>nul
)

REM Clean Qt generated Makefiles
echo [3/6] Cleaning Qt generated files...
if exist Makefile del /q Makefile 2>nul
if exist Makefile.Debug del /q Makefile.Debug 2>nul
if exist Makefile.Release del /q Makefile.Release 2>nul
if exist .qmake.stash del /q .qmake.stash 2>nul

REM Clean executables
echo [4/6] Cleaning executables...
if exist PokerSolver.exe del /q PokerSolver.exe 2>nul
if exist PokerSolverGUI.exe del /q PokerSolverGUI.exe 2>nul
if exist test.exe del /q test.exe 2>nul

REM Clean build directory
echo [5/6] Cleaning build directory...
if exist build (
    del /q build\*.o 2>nul
)

REM Clean object files from root (legacy)
echo [6/6] Cleaning legacy object files...
del /q *.o 2>nul

echo.
echo ========================================
echo Clean complete!
echo ========================================

