#-------------------------------------------------
# Poker Solver - Qt GUI Project File
# This file configures the project for Qt Creator
#-------------------------------------------------

QT       += core gui widgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokerSolverGUI
TEMPLATE = app

# C++17 standard
CONFIG += c++17

# Include paths
INCLUDEPATH += include

# Optimization
QMAKE_CXXFLAGS += -O2

# Force use of Qt's MinGW compiler
QMAKE_CC = $$[QT_INSTALL_PREFIX]/../../Tools/mingw1310_64/bin/gcc.exe
QMAKE_CXX = $$[QT_INSTALL_PREFIX]/../../Tools/mingw1310_64/bin/g++.exe
QMAKE_LINK = $$[QT_INSTALL_PREFIX]/../../Tools/mingw1310_64/bin/g++.exe

# Core solver files
SOURCES += \
    src/Card.cpp \
    src/Deck.cpp \
    src/HandEvaluator.cpp \
    src/Player.cpp \
    src/PotManager.cpp \
    src/PokerGame.cpp \
    src/PokerSolver.cpp

HEADERS += \
    include/Card.h \
    include/Deck.h \
    include/HandEvaluator.h \
    include/Player.h \
    include/PotManager.h \
    include/PokerGame.h \
    include/PokerSolver.h

# GUI files
SOURCES += \
    gui/MainWindow.cpp \
    gui/main_gui.cpp

HEADERS += \
    gui/MainWindow.h \
    gui/CardSelector.h

# Resources (images, icons)
# RESOURCES += resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
