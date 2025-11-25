#-------------------------------------------------
# Poker Solver - Qt GUI Project File
# This file configures the project for Qt Creator
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokerSolverGUI
TEMPLATE = app

# C++17 standard
CONFIG += c++17

# Optimization
QMAKE_CXXFLAGS += -O2

# Core solver files (no changes needed!)
SOURCES += \
    Card.cpp \
    Deck.cpp \
    HandEvaluator.cpp \
    Player.cpp \
    PotManager.cpp \
    PokerGame.cpp \
    PokerSolver.cpp

HEADERS += \
    Card.h \
    Deck.h \
    HandEvaluator.h \
    Player.h \
    PotManager.h \
    PokerGame.h \
    PokerSolver.h

# GUI files (to be created)
# Uncomment when you create these files:
# SOURCES += \
#     gui/MainWindow.cpp \
#     gui/CardWidget.cpp \
#     gui/ResultsWidget.cpp \
#     main_gui.cpp

# HEADERS += \
#     gui/MainWindow.h \
#     gui/CardWidget.h \
#     gui/ResultsWidget.h

# Resources (images, icons)
# RESOURCES += resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
