// Improved Card Selector Widget with colored suit buttons
// Add this to MainWindow.h if you want to use it

#ifndef CARDSELECTOR_H
#define CARDSELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class CardSelector : public QWidget {
    Q_OBJECT
public:
    explicit CardSelector(const QString& label, QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setSpacing(5);
        
        // Label
        QLabel* titleLabel = new QLabel(label, this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
        layout->addWidget(titleLabel);
        
        // Rank selector
        rankCombo = new QComboBox(this);
        rankCombo->addItems({"--", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"});
        rankCombo->setStyleSheet("QComboBox { font-size: 16px; font-weight: bold; padding: 5px; }");
        layout->addWidget(rankCombo);
        
        // Suit buttons
        QHBoxLayout* suitLayout = new QHBoxLayout();
        suitLayout->setSpacing(5);
        
        suitGroup = new QButtonGroup(this);
        suitGroup->setExclusive(true);
        
        // Hearts button (red)
        QPushButton* heartsBtn = new QPushButton("♥", this);
        heartsBtn->setCheckable(true);
        heartsBtn->setFixedSize(40, 35);
        heartsBtn->setStyleSheet(
            "QPushButton { background-color: white; color: #DC143C; font-size: 20px; border: 2px solid #ddd; border-radius: 5px; }"
            "QPushButton:checked { background-color: #DC143C; color: white; border: 2px solid #DC143C; }"
            "QPushButton:hover { border: 2px solid #DC143C; }"
        );
        suitGroup->addButton(heartsBtn, 1);
        suitLayout->addWidget(heartsBtn);
        
        // Diamonds button (red)
        QPushButton* diamondsBtn = new QPushButton("♦", this);
        diamondsBtn->setCheckable(true);
        diamondsBtn->setFixedSize(40, 35);
        diamondsBtn->setStyleSheet(
            "QPushButton { background-color: white; color: #DC143C; font-size: 20px; border: 2px solid #ddd; border-radius: 5px; }"
            "QPushButton:checked { background-color: #DC143C; color: white; border: 2px solid #DC143C; }"
            "QPushButton:hover { border: 2px solid #DC143C; }"
        );
        suitGroup->addButton(diamondsBtn, 2);
        suitLayout->addWidget(diamondsBtn);
        
        // Clubs button (black)
        QPushButton* clubsBtn = new QPushButton("♣", this);
        clubsBtn->setCheckable(true);
        clubsBtn->setFixedSize(40, 35);
        clubsBtn->setStyleSheet(
            "QPushButton { background-color: white; color: #1a1a1a; font-size: 20px; border: 2px solid #ddd; border-radius: 5px; }"
            "QPushButton:checked { background-color: #1a1a1a; color: white; border: 2px solid #1a1a1a; }"
            "QPushButton:hover { border: 2px solid #1a1a1a; }"
        );
        suitGroup->addButton(clubsBtn, 3);
        suitLayout->addWidget(clubsBtn);
        
        // Spades button (black)
        QPushButton* spadesBtn = new QPushButton("♠", this);
        spadesBtn->setCheckable(true);
        spadesBtn->setFixedSize(40, 35);
        spadesBtn->setStyleSheet(
            "QPushButton { background-color: white; color: #1a1a1a; font-size: 20px; border: 2px solid #ddd; border-radius: 5px; }"
            "QPushButton:checked { background-color: #1a1a1a; color: white; border: 2px solid #1a1a1a; }"
            "QPushButton:hover { border: 2px solid #1a1a1a; }"
        );
        suitGroup->addButton(spadesBtn, 4);
        suitLayout->addWidget(spadesBtn);
        
        layout->addLayout(suitLayout);
        
        connect(rankCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CardSelector::cardChanged);
        connect(suitGroup, &QButtonGroup::idClicked, this, &CardSelector::cardChanged);
    }
    
    QString getRank() const {
        return rankCombo->currentText();
    }
    
    QString getSuit() const {
        int suitId = suitGroup->checkedId();
        if (suitId == 1) return "h";
        if (suitId == 2) return "d";
        if (suitId == 3) return "c";
        if (suitId == 4) return "s";
        return "None";
    }
    
    void clear() {
        rankCombo->setCurrentIndex(0);
        if (suitGroup->checkedButton()) {
            suitGroup->setExclusive(false);
            suitGroup->checkedButton()->setChecked(false);
            suitGroup->setExclusive(true);
        }
    }
    
signals:
    void cardChanged();
    
private:
    QComboBox* rankCombo;
    QButtonGroup* suitGroup;
};

#endif // CARDSELECTOR_H
