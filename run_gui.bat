@echo off
REM Launch PokerSolver GUI
REM Simple launcher script

echo Starting PokerSolver GUI...
echo.

REM Check if the GUI executable exists
if exist "release\PokerSolverGUI.exe" (
    start "" "release\PokerSolverGUI.exe"
    echo GUI launched successfully!
) else (
    echo Error: GUI executable not found!
    echo Please build the GUI first using build_gui.bat
    echo.
    pause
)
