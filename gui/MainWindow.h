// Example Qt Main Window Header
// This is a template to get started with Qt GUI

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFrame>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QFont>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QKeyEvent>
#include <QShortcut>

#include "../include/PokerSolver.h"
#include "../include/Card.h"

// Custom Card Widget for beautiful card rendering
class CardWidget : public QWidget {
    Q_OBJECT
public:
    explicit CardWidget(QWidget* parent = nullptr);
    void setCard(const QString& rank, const QString& suit);
    void clear();
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    QString rank;
    QString suit;
    bool isEmpty;
    QColor getSuitColor() const;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked();
    void onClearClicked();
    void onCard1Changed();
    void onCard2Changed();
    void updateVisualIndicators();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    // Setup methods
    void setupUI();
    void setupConnections();
    void applyModernStyle();
    void animateResultsPanel();
    void setupShortcuts();
    
    // Helper methods
    std::vector<Card> parseHoleCards();
    std::vector<Card> parseBoardCards();
    void displayResults(const DecisionResult& result);
    void clearResults();
    void updateCardWidgets();
    bool hasDuplicateCards();
    std::vector<std::pair<QComboBox*, QComboBox*>> getAllCardCombos();
    
    // UI Components - Card Selection
    QComboBox* holeCard1Rank;
    QComboBox* holeCard1Suit;
    QComboBox* holeCard2Rank;
    QComboBox* holeCard2Suit;
    
    // Board cards (5 cards)
    QComboBox* boardCard1Rank;
    QComboBox* boardCard1Suit;
    QComboBox* boardCard2Rank;
    QComboBox* boardCard2Suit;
    QComboBox* boardCard3Rank;
    QComboBox* boardCard3Suit;
    QComboBox* boardCard4Rank;
    QComboBox* boardCard4Suit;
    QComboBox* boardCard5Rank;
    QComboBox* boardCard5Suit;
    
    // Card Widgets for visual display
    CardWidget* holeCardWidget1;
    CardWidget* holeCardWidget2;
    CardWidget* boardCardWidget1;
    CardWidget* boardCardWidget2;
    CardWidget* boardCardWidget3;
    CardWidget* boardCardWidget4;
    CardWidget* boardCardWidget5;
    
    // Input fields
    QSpinBox* potSizeInput;
    QSpinBox* callAmountInput;
    QSpinBox* numOpponentsInput;
    QSpinBox* numSimulationsInput;
    
    // Action buttons
    QPushButton* calculateButton;
    QPushButton* clearButton;
    
    // Results display
    QLabel* equityLabel;
    QLabel* potOddsLabel;
    QLabel* evLabel;
    QLabel* decisionLabel;
    QTextEdit* reasoningText;
    QProgressBar* progressBar;
    QFrame* resultsFrame;
    QWidget* resultsContainer;
    
    // Visual enhancements
    QLabel* equityPercentageLabel;
    QProgressBar* equityBar;
    QProgressBar* potOddsBar;
    
    // Visual indicators
    QLabel* equityIndicator;
    QLabel* strengthIndicator;
    QLabel* shortcutsHint;
    
    // Poker Solver
    PokerSolver* solver;
    
    // Async calculation
    QFutureWatcher<DecisionResult>* calculationWatcher;
    void onCalculationFinished();
    
    // Constants
    static const QStringList RANKS;
    static const QStringList SUITS;
};

#endif // MAINWINDOW_H
