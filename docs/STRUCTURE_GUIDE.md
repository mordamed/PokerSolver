# PokerSolver - Texas Hold'em Equity Calculator

A complete poker engine and Monte Carlo equity calculator with both CLI and GUI interfaces.

## 📁 Project Structure

```
PokerSolver/
├── include/           # Header files (.h)
│   ├── Card.h
│   ├── Deck.h
│   ├── HandEvaluator.h
│   ├── Player.h
│   ├── PotManager.h
│   ├── PokerGame.h
│   └── PokerSolver.h
│
├── src/              # Source files (.cpp)
│   ├── Card.cpp
│   ├── Deck.cpp
│   ├── HandEvaluator.cpp
│   ├── Player.cpp
│   ├── PotManager.cpp
│   ├── PokerGame.cpp
│   ├── PokerSolver.cpp
│   └── main.cpp      # CLI entry point
│
├── gui/              # Qt GUI files
│   ├── MainWindow.h
│   ├── MainWindow.cpp
│   ├── CardSelector.h
│   └── main_gui.cpp  # GUI entry point
│
├── build/            # Object files (.o)
├── docs/             # Documentation
└── release/          # Qt GUI build output
```

## 🚀 Quick Start

### Prerequisites
- **MinGW GCC 13.1.0** (C:\Qt\Tools\mingw1310_64)
- **Qt 6.10.1** (for GUI only) (C:\Qt\6.10.1\mingw_64)

### Building the CLI Version

```powershell
# Set up PATH
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;$env:PATH"

# Build
mingw32-make -f Makefile_CLI

# Run
.\PokerSolver.exe
```

### Building the GUI Version

```powershell
# Simply run the build script
.\build_gui.bat

# Or manually:
qmake PokerSolverGUI.pro
mingw32-make -f Makefile.Release
```

## 🎮 Features

### Core Engine
- **Card & Deck Management**: Full 52-card deck with shuffling
- **Hand Evaluation**: Complete Texas Hold'em hand ranking system
- **Game Logic**: Multi-player support with blinds, betting rounds, and pot management
- **Monte Carlo Simulation**: 10,000+ iterations for accurate equity calculations

### GUI Features
- **Visual Card Selection**: Interactive card rendering with custom painting
- **Real-time Calculations**: Async Monte Carlo using QtConcurrent (no UI freeze)
- **Duplicate Detection**: Prevents selecting the same card twice
- **Smooth UX**: Scroll support, loading animations, gradient styling
- **Opponents**: Support for 1-9 opponents

### CLI Features
- **Text-based Interface**: Simple console interface for quick calculations
- **Fast Execution**: Lightweight build without Qt dependencies

## 🎯 Usage Example

### GUI
1. Launch `release\PokerSolverGUI.exe`
2. Select your two hole cards
3. Optionally select board cards (flop, turn, river)
4. Choose number of opponents (1-9)
5. Click "Calculate Equity"
6. View win/tie/loss probabilities

### CLI
1. Run `PokerSolver.exe`
2. Follow prompts to enter cards
3. View equity results in console

## 🏗️ Build System

### CLI Makefile (`Makefile_CLI`)
- Compiler: g++ with C++17
- Flags: `-std=c++17 -Wall -Wextra -O2 -Iinclude`
- Output: `PokerSolver.exe` in root directory
- Objects: Stored in `build/` folder

### GUI Project (`PokerSolverGUI.pro`)
- Build system: qmake + mingw32-make
- Qt modules: core, gui, widgets, concurrent
- Output: `release\PokerSolverGUI.exe`
- Automatically handles MOC generation

## 🔧 Development

### Adding New Features
1. Add header to `include/`
2. Add implementation to `src/`
3. Update `Makefile_CLI` SOURCES list (for CLI)
4. Update `PokerSolverGUI.pro` (for GUI)
5. Rebuild both versions

### Code Structure
- **include/**: Public API headers
- **src/**: Implementation files with `#include "../include/Header.h"`
- **gui/**: Qt-specific GUI code

## 📊 Technical Details

### Monte Carlo Simulation
- Default iterations: 10,000+
- Runs in separate thread (GUI) to prevent freezing
- Simulates all remaining community cards and opponent hands
- Calculates win/tie/loss percentages

### Hand Evaluation
- Supports all poker hands: High Card through Royal Flush
- Tie-breaking with kickers
- Optimized comparison algorithms

## 🎨 GUI Technologies
- **Qt 6.10.1**: Modern C++ framework
- **QtConcurrent**: Async calculations
- **Custom Painting**: CardWidget for visual cards
- **QScrollArea**: Handles overflow
- **Modern Styling**: Gradients, rounded corners, animations

## 📝 File Organization

### Header Files (include/)
All header files use simple relative includes:
```cpp
#include "Card.h"
#include "Player.h"
```

### Source Files (src/)
All source files reference headers from include/:
```cpp
#include "../include/Deck.h"
```

### GUI Files (gui/)
GUI files reference both core headers and Qt:
```cpp
#include "../include/PokerSolver.h"
#include <QWidget>
```

## 🐛 Troubleshooting

### CLI won't compile
- Ensure MinGW is in PATH
- Run: `$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;$env:PATH"`
- Use: `mingw32-make -f Makefile_CLI clean` then rebuild

### GUI won't compile
- Ensure Qt's qmake is in PATH
- Run `build_gui.bat` which sets up the environment
- Check that Qt 6.10.1 is installed at C:\Qt\6.10.1\mingw_64

### Errors about missing headers
- Verify files are in correct folders (include/, src/, gui/)
- Check #include paths use correct relative paths
- For GUI: INCLUDEPATH in .pro file should have `include`

---
**Compiled with**: MinGW 13.1.0 + Qt 6.10.1  
**Language**: C++17  
**Build tested**: Windows 10/11
