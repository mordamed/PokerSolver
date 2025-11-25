// Example Qt Main Window Implementation
// This shows how to integrate the PokerSolver with Qt

#include "MainWindow.h"

// ==================== CardWidget Implementation ====================

CardWidget::CardWidget(QWidget* parent) 
    : QWidget(parent), isEmpty(true) {
    setFixedSize(80, 110);
    
    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadow);
}

void CardWidget::setCard(const QString& r, const QString& s) {
    rank = r;
    suit = s;
    isEmpty = (r == "None" || s == "None");
    update();
}

void CardWidget::clear() {
    isEmpty = true;
    update();
}

QColor CardWidget::getSuitColor() const {
    if (suit == "h" || suit == "d") return QColor(220, 20, 60); // Red
    return QColor(30, 30, 30); // Black
}

void CardWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (isEmpty) {
        // Empty card slot
        painter.setBrush(QColor(240, 240, 245));
        painter.setPen(QPen(QColor(180, 180, 190), 2, Qt::DashLine));
        painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 8, 8);
        
        painter.setPen(QColor(150, 150, 160));
        painter.setFont(QFont("Arial", 10));
        painter.drawText(rect(), Qt::AlignCenter, "?");
    } else {
        // Card background
        painter.setBrush(Qt::white);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 8, 8);
        
        // Draw rank
        painter.setPen(getSuitColor());
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(QRect(10, 10, 40, 40), Qt::AlignCenter, rank);
        
        // Draw suit symbol
        QFont suitFont("Arial", 32);
        painter.setFont(suitFont);
        QString suitSymbol;
        if (suit == "h") suitSymbol = "♥";
        else if (suit == "d") suitSymbol = "♦";
        else if (suit == "c") suitSymbol = "♣";
        else if (suit == "s") suitSymbol = "♠";
        
        painter.drawText(QRect(20, 50, 40, 40), Qt::AlignCenter, suitSymbol);
    }
}

// ==================== MainWindow Implementation ====================

const QStringList MainWindow::RANKS = {
    "None", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"
};

const QStringList MainWindow::SUITS = {
    "None", "h", "d", "c", "s"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), solver(new PokerSolver(10000))
{
    setupUI();
    setupConnections();
    applyModernStyle();
    setWindowTitle("♠ Poker Solver Pro - Monte Carlo Calculator");
    resize(900, 850);
    
    // Center window on screen
    setGeometry(
        QApplication::desktop()->screen()->rect().center().x() - width() / 2,
        QApplication::desktop()->screen()->rect().center().y() - height() / 2,
        width(), height()
    );
}

MainWindow::~MainWindow() {
    delete solver;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // === Title Header ===
    QLabel* titleLabel = new QLabel("♠ ♥ POKER SOLVER PRO ♣ ♦", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #1a1a2e; padding: 15px;");
    mainLayout->addWidget(titleLabel);
    
    // === Your Cards Section ===
    QGroupBox* holeCardsGroup = new QGroupBox("🎴 Your Hole Cards", this);
    holeCardsGroup->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; color: #16213e; }");
    QVBoxLayout* holeCardsMainLayout = new QVBoxLayout();
    
    // Card widgets display
    QHBoxLayout* holeCardsVisualLayout = new QHBoxLayout();
    holeCardWidget1 = new CardWidget();
    holeCardWidget2 = new CardWidget();
    holeCardsVisualLayout->addStretch();
    holeCardsVisualLayout->addWidget(holeCardWidget1);
    holeCardsVisualLayout->addSpacing(15);
    holeCardsVisualLayout->addWidget(holeCardWidget2);
    holeCardsVisualLayout->addStretch();
    holeCardsMainLayout->addLayout(holeCardsVisualLayout);
    
    // Card selectors
    QHBoxLayout* holeCardsLayout = new QHBoxLayout();
    
    holeCard1Rank = new QComboBox();
    holeCard1Rank->addItems(RANKS);
    holeCard1Suit = new QComboBox();
    holeCard1Suit->addItems(SUITS);
    
    holeCard2Rank = new QComboBox();
    holeCard2Rank->addItems(RANKS);
    holeCard2Suit = new QComboBox();
    holeCard2Suit->addItems(SUITS);
    
    holeCardsLayout->addStretch();
    holeCardsLayout->addWidget(new QLabel("Card 1:"));
    holeCardsLayout->addWidget(holeCard1Rank);
    holeCardsLayout->addWidget(holeCard1Suit);
    holeCardsLayout->addSpacing(30);
    holeCardsLayout->addWidget(new QLabel("Card 2:"));
    holeCardsLayout->addWidget(holeCard2Rank);
    holeCardsLayout->addWidget(holeCard2Suit);
    holeCardsLayout->addStretch();
    
    holeCardsMainLayout->addLayout(holeCardsLayout);
    holeCardsGroup->setLayout(holeCardsMainLayout);
    mainLayout->addWidget(holeCardsGroup);
    
    // === Board Section ===
    QGroupBox* boardGroup = new QGroupBox("🃏 Community Cards (Board)", this);
    boardGroup->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; color: #16213e; }");
    QVBoxLayout* boardMainLayout = new QVBoxLayout();
    
    // Card widgets display
    QHBoxLayout* boardCardsVisualLayout = new QHBoxLayout();
    boardCardWidget1 = new CardWidget();
    boardCardWidget2 = new CardWidget();
    boardCardWidget3 = new CardWidget();
    boardCardWidget4 = new CardWidget();
    boardCardWidget5 = new CardWidget();
    
    boardCardsVisualLayout->addStretch();
    boardCardsVisualLayout->addWidget(boardCardWidget1);
    boardCardsVisualLayout->addSpacing(10);
    boardCardsVisualLayout->addWidget(boardCardWidget2);
    boardCardsVisualLayout->addSpacing(10);
    boardCardsVisualLayout->addWidget(boardCardWidget3);
    boardCardsVisualLayout->addSpacing(20); // Extra space before turn/river
    boardCardsVisualLayout->addWidget(boardCardWidget4);
    boardCardsVisualLayout->addSpacing(10);
    boardCardsVisualLayout->addWidget(boardCardWidget5);
    boardCardsVisualLayout->addStretch();
    boardMainLayout->addLayout(boardCardsVisualLayout);
    
    // Card selectors in a grid
    QGridLayout* boardSelectorsLayout = new QGridLayout();
    
    boardCard1Rank = new QComboBox();
    boardCard1Rank->addItems(RANKS);
    boardCard1Suit = new QComboBox();
    boardCard1Suit->addItems(SUITS);
    boardSelectorsLayout->addWidget(new QLabel("Flop 1:"), 0, 0);
    boardSelectorsLayout->addWidget(boardCard1Rank, 0, 1);
    boardSelectorsLayout->addWidget(boardCard1Suit, 0, 2);
    
    boardCard2Rank = new QComboBox();
    boardCard2Rank->addItems(RANKS);
    boardCard2Suit = new QComboBox();
    boardCard2Suit->addItems(SUITS);
    boardSelectorsLayout->addWidget(new QLabel("Flop 2:"), 0, 3);
    boardSelectorsLayout->addWidget(boardCard2Rank, 0, 4);
    boardSelectorsLayout->addWidget(boardCard2Suit, 0, 5);
    
    boardCard3Rank = new QComboBox();
    boardCard3Rank->addItems(RANKS);
    boardCard3Suit = new QComboBox();
    boardCard3Suit->addItems(SUITS);
    boardSelectorsLayout->addWidget(new QLabel("Flop 3:"), 0, 6);
    boardSelectorsLayout->addWidget(boardCard3Rank, 0, 7);
    boardSelectorsLayout->addWidget(boardCard3Suit, 0, 8);
    
    boardCard4Rank = new QComboBox();
    boardCard4Rank->addItems(RANKS);
    boardCard4Suit = new QComboBox();
    boardCard4Suit->addItems(SUITS);
    boardSelectorsLayout->addWidget(new QLabel("Turn:"), 1, 0);
    boardSelectorsLayout->addWidget(boardCard4Rank, 1, 1);
    boardSelectorsLayout->addWidget(boardCard4Suit, 1, 2);
    
    boardCard5Rank = new QComboBox();
    boardCard5Rank->addItems(RANKS);
    boardCard5Suit = new QComboBox();
    boardCard5Suit->addItems(SUITS);
    boardSelectorsLayout->addWidget(new QLabel("River:"), 1, 3);
    boardSelectorsLayout->addWidget(boardCard5Rank, 1, 4);
    boardSelectorsLayout->addWidget(boardCard5Suit, 1, 5);
    
    boardMainLayout->addLayout(boardSelectorsLayout);
    boardGroup->setLayout(boardMainLayout);
    mainLayout->addWidget(boardGroup);
    
    // === Input Parameters Section ===
    QGroupBox* paramsGroup = new QGroupBox("⚙ Parameters", this);
    paramsGroup->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; color: #16213e; }");
    QGridLayout* paramsLayout = new QGridLayout();
    paramsLayout->setSpacing(12);
    
    // Pot Size
    QLabel* potIcon = new QLabel("💰");
    potIcon->setStyleSheet("font-size: 20px;");
    paramsLayout->addWidget(potIcon, 0, 0);
    paramsLayout->addWidget(new QLabel("Pot Size:"), 0, 1);
    potSizeInput = new QSpinBox();
    potSizeInput->setRange(0, 999999);
    potSizeInput->setValue(100);
    potSizeInput->setSuffix(" chips");
    potSizeInput->setMinimumWidth(150);
    paramsLayout->addWidget(potSizeInput, 0, 2);
    
    // Call Amount
    QLabel* callIcon = new QLabel("🎯");
    callIcon->setStyleSheet("font-size: 20px;");
    paramsLayout->addWidget(callIcon, 0, 3);
    paramsLayout->addWidget(new QLabel("Call Amount:"), 0, 4);
    callAmountInput = new QSpinBox();
    callAmountInput->setRange(0, 999999);
    callAmountInput->setValue(30);
    callAmountInput->setSuffix(" chips");
    callAmountInput->setMinimumWidth(150);
    paramsLayout->addWidget(callAmountInput, 0, 5);
    
    // Opponents
    QLabel* oppsIcon = new QLabel("👥");
    oppsIcon->setStyleSheet("font-size: 20px;");
    paramsLayout->addWidget(oppsIcon, 1, 0);
    paramsLayout->addWidget(new QLabel("Opponents:"), 1, 1);
    numOpponentsInput = new QSpinBox();
    numOpponentsInput->setRange(1, 9);
    numOpponentsInput->setValue(1);
    numOpponentsInput->setSuffix(" players");
    numOpponentsInput->setMinimumWidth(150);
    paramsLayout->addWidget(numOpponentsInput, 1, 2);
    
    // Simulations
    QLabel* simsIcon = new QLabel("🔄");
    simsIcon->setStyleSheet("font-size: 20px;");
    paramsLayout->addWidget(simsIcon, 1, 3);
    paramsLayout->addWidget(new QLabel("Simulations:"), 1, 4);
    numSimulationsInput = new QSpinBox();
    numSimulationsInput->setRange(1000, 100000);
    numSimulationsInput->setValue(10000);
    numSimulationsInput->setSingleStep(1000);
    numSimulationsInput->setMinimumWidth(150);
    paramsLayout->addWidget(numSimulationsInput, 1, 5);
    
    paramsGroup->setLayout(paramsLayout);
    mainLayout->addWidget(paramsGroup);
    
    // === Buttons ===
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(15);
    
    calculateButton = new QPushButton("🎲 Calculate Equity");
    calculateButton->setMinimumHeight(50);
    calculateButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4CAF50, stop:1 #45a049);"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5CBF60, stop:1 #4CAF50);"
        "}"
        "QPushButton:pressed {"
        "   background: #3d8b40;"
        "}"
    );
    
    clearButton = new QPushButton("🗑 Clear");
    clearButton->setMinimumHeight(50);
    clearButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f44336, stop:1 #da190b);"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ff5347, stop:1 #f44336);"
        "}"
        "QPushButton:pressed {"
        "   background: #c62828;"
        "}"
    );
    
    buttonsLayout->addWidget(calculateButton);
    buttonsLayout->addWidget(clearButton);
    mainLayout->addLayout(buttonsLayout);
    
    // === Progress Bar ===
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #bbb;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "   background-color: #f0f0f0;"
        "}"
        "QProgressBar::chunk {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4CAF50, stop:1 #8BC34A);"
        "   border-radius: 3px;"
        "}"
    );
    mainLayout->addWidget(progressBar);
    
    // === Results Section ===
    QGroupBox* resultsGroup = new QGroupBox("📊 Analysis Results", this);
    resultsGroup->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; color: #16213e; }");
    
    resultsContainer = new QWidget();
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsContainer);
    resultsLayout->setSpacing(15);
    
    // Decision label (most prominent)
    decisionLabel = new QLabel("Decision: --");
    decisionLabel->setAlignment(Qt::AlignCenter);
    decisionLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; padding: 15px; "
        "border: 3px solid #ccc; border-radius: 10px; background-color: #f5f5f5;"
    );
    resultsLayout->addWidget(decisionLabel);
    
    // Equity section with bar
    QVBoxLayout* equityLayout = new QVBoxLayout();
    equityLabel = new QLabel("Win Equity: --");
    equityLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2196F3;");
    equityLayout->addWidget(equityLabel);
    
    equityBar = new QProgressBar();
    equityBar->setRange(0, 100);
    equityBar->setValue(0);
    equityBar->setTextVisible(true);
    equityBar->setFormat("%p%");
    equityBar->setMinimumHeight(30);
    equityBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #2196F3;"
        "   border-radius: 8px;"
        "   text-align: center;"
        "   background-color: #e3f2fd;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        "QProgressBar::chunk {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2196F3, stop:1 #64B5F6);"
        "   border-radius: 6px;"
        "}"
    );
    equityLayout->addWidget(equityBar);
    resultsLayout->addLayout(equityLayout);
    
    // Pot Odds section with bar
    QVBoxLayout* potOddsLayout = new QVBoxLayout();
    potOddsLabel = new QLabel("Required Equity (Pot Odds): --");
    potOddsLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #FF9800;");
    potOddsLayout->addWidget(potOddsLabel);
    
    potOddsBar = new QProgressBar();
    potOddsBar->setRange(0, 100);
    potOddsBar->setValue(0);
    potOddsBar->setTextVisible(true);
    potOddsBar->setFormat("%p%");
    potOddsBar->setMinimumHeight(30);
    potOddsBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #FF9800;"
        "   border-radius: 8px;"
        "   text-align: center;"
        "   background-color: #fff3e0;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        "QProgressBar::chunk {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF9800, stop:1 #FFB74D);"
        "   border-radius: 6px;"
        "}"
    );
    potOddsLayout->addWidget(potOddsBar);
    resultsLayout->addLayout(potOddsLayout);
    
    // EV label
    evLabel = new QLabel("Expected Value (EV): --");
    evLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #9C27B0; "
        "padding: 10px; background-color: #f3e5f5; border-radius: 5px;"
    );
    evLabel->setAlignment(Qt::AlignCenter);
    resultsLayout->addWidget(evLabel);
    
    // Reasoning section
    QLabel* reasoningLabel = new QLabel("💡 Reasoning:");
    reasoningLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #555;");
    resultsLayout->addWidget(reasoningLabel);
    
    reasoningText = new QTextEdit();
    reasoningText->setReadOnly(true);
    reasoningText->setMaximumHeight(120);
    reasoningText->setStyleSheet(
        "QTextEdit {"
        "   border: 2px solid #ddd;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   background-color: #fafafa;"
        "   font-size: 13px;"
        "}"
    );
    resultsLayout->addWidget(reasoningText);
    
    resultsGroup->setLayout(resultsLayout);
    mainLayout->addWidget(resultsGroup);
    
    mainLayout->addStretch();
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections() {
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    
    // Update card widgets when selection changes
    connect(holeCard1Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(holeCard1Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(holeCard2Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(holeCard2Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    
    connect(boardCard1Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard1Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard2Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard2Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard3Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard3Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard4Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard4Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard5Rank, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
    connect(boardCard5Suit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCardWidgets);
}

void MainWindow::applyModernStyle() {
    setStyleSheet(
        "QMainWindow {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "       stop:0 #f5f7fa, stop:1 #c3cfe2);"
        "}"
        "QGroupBox {"
        "   background-color: white;"
        "   border: 2px solid #ddd;"
        "   border-radius: 10px;"
        "   margin-top: 10px;"
        "   padding: 15px;"
        "   font-size: 14px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 15px;"
        "   padding: 0 5px;"
        "}"
        "QLabel {"
        "   color: #333;"
        "   font-size: 13px;"
        "}"
        "QComboBox {"
        "   border: 2px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   background-color: white;"
        "   min-width: 60px;"
        "}"
        "QComboBox:hover {"
        "   border: 2px solid #2196F3;"
        "}"
        "QSpinBox {"
        "   border: 2px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   background-color: white;"
        "}"
        "QSpinBox:hover {"
        "   border: 2px solid #2196F3;"
        "}"
    );
}

void MainWindow::updateCardWidgets() {
    // Update hole cards
    holeCardWidget1->setCard(holeCard1Rank->currentText(), holeCard1Suit->currentText());
    holeCardWidget2->setCard(holeCard2Rank->currentText(), holeCard2Suit->currentText());
    
    // Update board cards
    boardCardWidget1->setCard(boardCard1Rank->currentText(), boardCard1Suit->currentText());
    boardCardWidget2->setCard(boardCard2Rank->currentText(), boardCard2Suit->currentText());
    boardCardWidget3->setCard(boardCard3Rank->currentText(), boardCard3Suit->currentText());
    boardCardWidget4->setCard(boardCard4Rank->currentText(), boardCard4Suit->currentText());
    boardCardWidget5->setCard(boardCard5Rank->currentText(), boardCard5Suit->currentText());
}

void MainWindow::onCalculateClicked() {
    try {
        // Parse cards
        std::vector<Card> holeCards = parseHoleCards();
        std::vector<Card> board = parseBoardCards();
        
        // Get parameters
        int potSize = potSizeInput->value();
        int callAmount = callAmountInput->value();
        int opponents = numOpponentsInput->value();
        int simulations = numSimulationsInput->value();
        
        // Update solver
        delete solver;
        solver = new PokerSolver(simulations);
        
        // Show progress
        progressBar->setVisible(true);
        progressBar->setRange(0, 0); // Indeterminate
        
        // Calculate
        DecisionResult result = solver->analyzeDecision(
            holeCards, board, potSize, callAmount, opponents, simulations
        );
        
        // Display results
        displayResults(result);
        
        progressBar->setVisible(false);
        
    } catch (const std::exception& e) {
        progressBar->setVisible(false);
        QMessageBox::warning(this, "Error", QString::fromStdString(e.what()));
    }
}

void MainWindow::onClearClicked() {
    // Reset all inputs
    holeCard1Rank->setCurrentIndex(0);
    holeCard1Suit->setCurrentIndex(0);
    holeCard2Rank->setCurrentIndex(0);
    holeCard2Suit->setCurrentIndex(0);
    
    boardCard1Rank->setCurrentIndex(0);
    boardCard1Suit->setCurrentIndex(0);
    boardCard2Rank->setCurrentIndex(0);
    boardCard2Suit->setCurrentIndex(0);
    boardCard3Rank->setCurrentIndex(0);
    boardCard3Suit->setCurrentIndex(0);
    boardCard4Rank->setCurrentIndex(0);
    boardCard4Suit->setCurrentIndex(0);
    boardCard5Rank->setCurrentIndex(0);
    boardCard5Suit->setCurrentIndex(0);
    
    potSizeInput->setValue(100);
    callAmountInput->setValue(30);
    numOpponentsInput->setValue(1);
    
    clearResults();
}

std::vector<Card> MainWindow::parseHoleCards() {
    std::vector<Card> cards;
    
    QString card1Str = holeCard1Rank->currentText() + holeCard1Suit->currentText();
    QString card2Str = holeCard2Rank->currentText() + holeCard2Suit->currentText();
    
    if (card1Str.contains("None") || card2Str.contains("None")) {
        throw std::invalid_argument("Please select both hole cards");
    }
    
    cards.push_back(Card::fromString(card1Str.toStdString()));
    cards.push_back(Card::fromString(card2Str.toStdString()));
    
    return cards;
}

std::vector<Card> MainWindow::parseBoardCards() {
    std::vector<Card> board;
    
    auto addCard = [&board](QComboBox* rank, QComboBox* suit) {
        QString cardStr = rank->currentText() + suit->currentText();
        if (!cardStr.contains("None")) {
            board.push_back(Card::fromString(cardStr.toStdString()));
        }
    };
    
    addCard(boardCard1Rank, boardCard1Suit);
    addCard(boardCard2Rank, boardCard2Suit);
    addCard(boardCard3Rank, boardCard3Suit);
    addCard(boardCard4Rank, boardCard4Suit);
    addCard(boardCard5Rank, boardCard5Suit);
    
    return board;
}

void MainWindow::displayResults(const DecisionResult& result) {
    // Update equity
    equityLabel->setText(QString("Win Equity: %1%").arg(result.equity, 0, 'f', 2));
    equityBar->setValue(static_cast<int>(result.equity));
    
    // Update pot odds
    potOddsLabel->setText(QString("Required Equity (Pot Odds): %1%").arg(result.potOdds, 0, 'f', 2));
    potOddsBar->setValue(static_cast<int>(result.potOdds));
    
    // Update EV
    QString evText = QString("Expected Value: %1%2 chips")
        .arg(result.expectedValue >= 0 ? "+" : "")
        .arg(result.expectedValue, 0, 'f', 2);
    evLabel->setText(evText);
    
    if (result.expectedValue > 0) {
        evLabel->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #4CAF50; "
            "padding: 10px; background-color: #e8f5e9; border: 2px solid #4CAF50; border-radius: 5px;"
        );
    } else {
        evLabel->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #f44336; "
            "padding: 10px; background-color: #ffebee; border: 2px solid #f44336; border-radius: 5px;"
        );
    }
    
    // Update decision
    QString decision;
    if (result.shouldCall) {
        decision = "✅ CALL - Profitable Play!";
        decisionLabel->setStyleSheet(
            "font-size: 24px; font-weight: bold; color: white; padding: 15px; "
            "border: 3px solid #4CAF50; border-radius: 10px; "
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #66BB6A, stop:1 #4CAF50);"
        );
    } else {
        decision = "❌ FOLD - Not Profitable";
        decisionLabel->setStyleSheet(
            "font-size: 24px; font-weight: bold; color: white; padding: 15px; "
            "border: 3px solid #f44336; border-radius: 10px; "
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #EF5350, stop:1 #f44336);"
        );
    }
    decisionLabel->setText(decision);
    
    // Update reasoning
    reasoningText->setText(QString::fromStdString(result.reasoning));
    
    // Animate results
    animateResultsPanel();
}

void MainWindow::animateResultsPanel() {
    // Simple fade-in effect
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(resultsContainer);
    resultsContainer->setGraphicsEffect(effect);
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::clearResults() {
    equityLabel->setText("Win Equity: --");
    equityBar->setValue(0);
    potOddsLabel->setText("Required Equity (Pot Odds): --");
    potOddsBar->setValue(0);
    evLabel->setText("Expected Value (EV): --");
    evLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #9C27B0; "
        "padding: 10px; background-color: #f3e5f5; border-radius: 5px;"
    );
    decisionLabel->setText("Decision: --");
    decisionLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; padding: 15px; "
        "border: 3px solid #ccc; border-radius: 10px; background-color: #f5f5f5;"
    );
    reasoningText->clear();
    
    // Clear card widgets
    holeCardWidget1->clear();
    holeCardWidget2->clear();
    boardCardWidget1->clear();
    boardCardWidget2->clear();
    boardCardWidget3->clear();
    boardCardWidget4->clear();
    boardCardWidget5->clear();
}

void MainWindow::onCard1Changed() {
    // Could add validation here
}

void MainWindow::onCard2Changed() {
    // Could add validation here
}
