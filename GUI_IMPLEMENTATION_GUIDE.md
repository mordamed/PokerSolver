# GUI Implementation Guide for Poker Solver

## 🎨 Recommended Approach: Qt Framework

### Why Qt?
- **Professional** - Industry standard for C++ GUIs
- **Cross-platform** - Works on Windows, Linux, Mac
- **Rich widgets** - Perfect for card games
- **Visual Designer** - Qt Designer for drag-and-drop UI
- **Active community** - Extensive documentation

---

## 📋 Step-by-Step Implementation Plan

### **Phase 1: Setup (1-2 hours)**

#### 1.1 Install Qt
```bash
# Download Qt from: https://www.qt.io/download-open-source-installer
# Or with package manager:
winget install -e --id Qt.QtCreator
```

#### 1.2 Create Qt Project
```bash
cd PokerSolver
qmake -project
# Add to PokerSolver.pro:
QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
```

### **Phase 2: Design UI (2-3 days)**

#### 2.1 Main Window Layout
```
┌─────────────────────────────────────────────────┐
│  Poker Solver - Monte Carlo Calculator         │
├─────────────────────────────────────────────────┤
│  ┌───────────────────────────────────────────┐  │
│  │  Your Cards:  [Ah] [Kd]                   │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │  Board:      [9h] [5h] [2c] [ ] [ ]       │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │  Pot Size:    [   100  ]  chips           │  │
│  │  Call Amount: [    30  ]  chips           │  │
│  │  Opponents:   [     1  ]  players         │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │       [Calculate]      [Clear]            │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │  Results:                                 │  │
│  │  ═══════════════════════════════════════  │  │
│  │  Equity:       67.42%                     │  │
│  │  Pot Odds:     23.08%                     │  │
│  │  EV:          +18.45 chips                │  │
│  │  Decision:     CALL ✓ (Profitable)        │  │
│  └───────────────────────────────────────────┘  │
└─────────────────────────────────────────────────┘
```

#### 2.2 Key Widgets Needed
```cpp
// mainwindow.h
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // Card selection
    QComboBox* holeCard1Rank;
    QComboBox* holeCard1Suit;
    QComboBox* holeCard2Rank;
    QComboBox* holeCard2Suit;
    
    // Board cards (5 pairs of rank/suit combos)
    std::vector<QComboBox*> boardCardRanks;
    std::vector<QComboBox*> boardCardSuits;
    
    // Input fields
    QSpinBox* potSize;
    QSpinBox* callAmount;
    QSpinBox* numOpponents;
    QSpinBox* numSimulations;
    
    // Results display
    QLabel* equityLabel;
    QLabel* potOddsLabel;
    QLabel* evLabel;
    QLabel* decisionLabel;
    QTextEdit* reasoningText;
    
    // Progress bar for simulation
    QProgressBar* progressBar;
    
    // Poker Solver instance
    PokerSolver* solver;

private slots:
    void onCalculate();
    void onClear();
};
```

### **Phase 3: Core Integration (1-2 days)**

#### 3.1 Connect Qt to Existing Code
```cpp
// mainwindow.cpp
#include "PokerSolver.h"
#include "Card.h"

void MainWindow::onCalculate() {
    try {
        // Parse cards from UI
        std::vector<Card> holeCards = parseHoleCards();
        std::vector<Card> board = parseBoardCards();
        
        // Get values
        int pot = potSize->value();
        int call = callAmount->value();
        int opponents = numOpponents->value();
        int sims = numSimulations->value();
        
        // Show progress
        progressBar->setVisible(true);
        
        // Calculate (in separate thread to avoid UI freeze)
        DecisionResult result = solver->analyzeDecision(
            holeCards, board, pot, call, opponents, sims
        );
        
        // Display results
        displayResults(result);
        
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

std::vector<Card> MainWindow::parseHoleCards() {
    std::vector<Card> cards;
    
    // Card 1
    std::string card1Str = 
        holeCard1Rank->currentText().toStdString() + 
        holeCard1Suit->currentText().toStdString();
    cards.push_back(Card::fromString(card1Str));
    
    // Card 2
    std::string card2Str = 
        holeCard2Rank->currentText().toStdString() + 
        holeCard2Suit->currentText().toStdString();
    cards.push_back(Card::fromString(card2Str));
    
    return cards;
}

void MainWindow::displayResults(const DecisionResult& result) {
    equityLabel->setText(
        QString("Equity: %1%").arg(result.equity, 0, 'f', 2)
    );
    
    potOddsLabel->setText(
        QString("Pot Odds: %1%").arg(result.potOdds, 0, 'f', 2)
    );
    
    evLabel->setText(
        QString("EV: %1 chips").arg(result.expectedValue, 0, 'f', 2)
    );
    
    QString decision = result.shouldCall ? 
        "CALL ✓ (Profitable)" : "FOLD ✗ (Not profitable)";
    
    decisionLabel->setText("Decision: " + decision);
    decisionLabel->setStyleSheet(
        result.shouldCall ? 
        "color: green; font-weight: bold;" : 
        "color: red; font-weight: bold;"
    );
    
    reasoningText->setText(QString::fromStdString(result.reasoning));
    progressBar->setVisible(false);
}
```

### **Phase 4: Visual Enhancements (1-2 days)**

#### 4.1 Card Images
```cpp
// Use PNG images for cards
// Download from: https://github.com/htdebeer/SVG-cards
// Or create custom QPixmaps

class CardWidget : public QWidget {
public:
    CardWidget(const Card& card, QWidget* parent = nullptr) {
        QPixmap cardImage = loadCardImage(card);
        QLabel* label = new QLabel(this);
        label->setPixmap(cardImage.scaled(100, 150, 
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

private:
    QPixmap loadCardImage(const Card& card) {
        QString filename = QString(":/images/cards/%1%2.png")
            .arg(rankToChar(card.getRank()))
            .arg(suitToChar(card.getSuit()));
        return QPixmap(filename);
    }
};
```

#### 4.2 Animated Progress
```cpp
// Show live simulation progress
void MainWindow::onCalculateAsync() {
    // Create worker thread
    QThread* thread = new QThread;
    SolverWorker* worker = new SolverWorker(
        holeCards, board, pot, call, opponents, sims
    );
    worker->moveToThread(thread);
    
    // Connect signals
    connect(thread, &QThread::started, worker, &SolverWorker::process);
    connect(worker, &SolverWorker::progress, progressBar, &QProgressBar::setValue);
    connect(worker, &SolverWorker::finished, this, &MainWindow::displayResults);
    connect(worker, &SolverWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    
    thread->start();
}
```

### **Phase 5: Additional Features (2-3 days)**

#### 5.1 Hand History
```cpp
// Save/Load analyzed hands
class HandHistory : public QWidget {
private:
    QListWidget* historyList;
    QPushButton* saveBtn;
    QPushButton* loadBtn;
    
public slots:
    void addHand(const AnalyzedHand& hand);
    void saveToFile();
    void loadFromFile();
};
```

#### 5.2 Range Analysis
```cpp
// Visual range selector
class RangeSelector : public QWidget {
    // 13x13 grid for all possible starting hands
    // AA, AKs, AQs, ..., 72o
    QButtonGroup* handGrid[13][13];
    
public:
    std::vector<std::pair<Card, Card>> getSelectedRange();
};
```

#### 5.3 Equity Chart
```cpp
// Use QtCharts for visualization
#include <QtCharts>

class EquityChart : public QChartView {
public:
    void displayEquityDistribution(const EquityResult& result) {
        QPieSeries* series = new QPieSeries();
        series->append("Win", result.winRate);
        series->append("Tie", result.tieRate);
        series->append("Loss", result.loseRate);
        
        QChart* chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Equity Distribution");
        setChart(chart);
    }
};
```

---

## 🚀 Alternative: Dear ImGui (Faster Prototype)

### Why ImGui?
- **Immediate mode** - Simpler than Qt
- **Lightweight** - ~100 KB
- **Fast to implement** - 1-2 days for basic UI
- **Great for prototyping**

### Basic ImGui Implementation
```cpp
// main.cpp with ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "PokerSolver.h"

int main() {
    // Initialize DirectX/OpenGL
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);
    
    // State
    static char holeCard1[3] = "Ah";
    static char holeCard2[3] = "Kd";
    static char board1[3] = "";
    static int potSize = 100;
    static int callAmount = 30;
    static int opponents = 1;
    
    PokerSolver solver(10000);
    DecisionResult result;
    bool calculated = false;
    
    // Main loop
    while (running) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Poker Solver");
        
        ImGui::Text("Your Cards:");
        ImGui::InputText("Card 1", holeCard1, 3);
        ImGui::InputText("Card 2", holeCard2, 3);
        
        ImGui::Separator();
        ImGui::Text("Board:");
        ImGui::InputText("Flop 1", board1, 3);
        // ... more cards
        
        ImGui::Separator();
        ImGui::InputInt("Pot Size", &potSize);
        ImGui::InputInt("Call Amount", &callAmount);
        ImGui::InputInt("Opponents", &opponents);
        
        if (ImGui::Button("Calculate")) {
            try {
                std::vector<Card> hole = {
                    Card::fromString(holeCard1),
                    Card::fromString(holeCard2)
                };
                std::vector<Card> board = parseBoardCards();
                
                result = solver.analyzeDecision(
                    hole, board, potSize, callAmount, opponents
                );
                calculated = true;
            } catch (std::exception& e) {
                ImGui::OpenPopup("Error");
            }
        }
        
        if (calculated) {
            ImGui::Separator();
            ImGui::Text("Results:");
            ImGui::Text("Equity: %.2f%%", result.equity);
            ImGui::Text("Pot Odds: %.2f%%", result.potOdds);
            ImGui::Text("EV: %.2f chips", result.expectedValue);
            
            ImVec4 color = result.shouldCall ? 
                ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            ImGui::TextColored(color, result.shouldCall ? 
                "CALL (Profitable)" : "FOLD (Not profitable)");
        }
        
        ImGui::End();
        
        // Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapChain->Present(1, 0);
    }
    
    return 0;
}
```

---

## 📊 Comparison: Qt vs ImGui

| Feature | Qt | ImGui |
|---------|----|----|
| **Learning Curve** | Medium | Easy |
| **Development Time** | 1-2 weeks | 2-3 days |
| **Visual Quality** | Professional | Good |
| **Customization** | High | Medium |
| **Cross-platform** | Excellent | Good |
| **File Size** | 20-50 MB | <1 MB |
| **Best For** | Production app | Prototype |

---

## 🎯 Recommended Path

### For Production:
1. Use **Qt Framework**
2. Follow Phase 1-5 above
3. ~2 weeks development time
4. Professional, polished result

### For Quick Prototype:
1. Use **Dear ImGui**
2. Basic UI in 2-3 days
3. Test concepts quickly
4. Migrate to Qt later if needed

---

## 📦 Required Assets

### Card Images
```
cards/
  ├── Ah.png, Kd.png, 2s.png, ...  (52 cards)
  └── back.png  (card back)
```

Download free card images:
- **SVG Cards**: https://github.com/htdebeer/SVG-cards
- **Playing Cards**: https://www.me.uk/cards/
- **Custom**: Use GIMP/Photoshop to create

### Icons
```
icons/
  ├── calculate.png
  ├── clear.png
  ├── save.png
  ├── load.png
  └── settings.png
```

---

## 🔧 Project Structure with GUI

```
PokerSolver/
├── src/
│   ├── core/
│   │   ├── Card.cpp/h
│   │   ├── Deck.cpp/h
│   │   ├── HandEvaluator.cpp/h
│   │   ├── Player.cpp/h
│   │   ├── PotManager.cpp/h
│   │   ├── PokerGame.cpp/h
│   │   └── PokerSolver.cpp/h
│   └── gui/
│       ├── MainWindow.cpp/h
│       ├── CardWidget.cpp/h
│       ├── ResultsWidget.cpp/h
│       └── HandHistoryWidget.cpp/h
├── resources/
│   ├── images/
│   │   └── cards/
│   └── icons/
├── main.cpp
├── PokerSolver.pro  (Qt project file)
└── README.md
```

---

## 🚀 Next Steps

1. **Choose framework** (Qt recommended)
2. **Install Qt Creator**
3. **Create new Qt Widgets Application**
4. **Integrate existing code** (keep .h/.cpp files as-is)
5. **Design UI in Qt Designer**
6. **Connect signals/slots**
7. **Add card images**
8. **Test and polish**

The existing C++ core is **100% compatible** with Qt - no changes needed to the solver logic!

---

Would you like me to:
1. Generate the complete Qt project files?
2. Create a simple ImGui prototype?
3. Design the UI mockups in more detail?
