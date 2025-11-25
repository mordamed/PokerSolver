#include "PokerGame.h"
#include "HandEvaluator.h"
#include <iostream>
#include <algorithm>

PokerGame::PokerGame(int numPlayers, int sb, int bb, int startingStack)
    : smallBlind(sb), bigBlind(bb), dealerPosition(0), currentPhase(GamePhase::PRE_FLOP), currentPlayerIndex(0) {
    
    if (numPlayers < 2 || numPlayers > 9) {
        throw std::invalid_argument("Number of players must be between 2 and 9");
    }
    
    // Create players
    for (int i = 0; i < numPlayers; i++) {
        std::string name = "Player " + std::to_string(i + 1);
        players.push_back(new Player(name, startingStack, i));
    }
}

PokerGame::~PokerGame() {
    for (auto* player : players) {
        delete player;
    }
}

void PokerGame::startNewHand() {
    // Reset
    deck.reset();
    board.clear();
    potManager.reset();
    currentPhase = GamePhase::PRE_FLOP;
    
    // Reset les joueurs
    for (auto* player : players) {
        player->resetForNewHand();
    }
    
    // Poster les blinds
    postBlinds();
    
    // Distribuer les cartes
    dealHoleCards();
}

void PokerGame::dealHoleCards() {
    for (auto* player : players) {
        if (player->isActive() && player->getStack() > 0) {
            std::vector<Card> cards;
            cards.push_back(deck.draw());
            cards.push_back(deck.draw());
            player->setHoleCards(cards);
        }
    }
}

void PokerGame::dealFlop() {
    if (currentPhase != GamePhase::PRE_FLOP) {
        throw std::runtime_error("Invalid phase for flop");
    }
    
    deck.draw(); // Burn card
    
    for (int i = 0; i < 3; i++) {
        board.push_back(deck.draw());
    }
    
    currentPhase = GamePhase::FLOP;
}

void PokerGame::dealTurn() {
    if (currentPhase != GamePhase::FLOP) {
        throw std::runtime_error("Phase invalide pour le turn");
    }
    
    deck.draw(); // Burn card
    board.push_back(deck.draw());
    currentPhase = GamePhase::TURN;
}

void PokerGame::dealRiver() {
    if (currentPhase != GamePhase::RIVER) {
        throw std::runtime_error("Phase invalide pour la river");
    }
    
    deck.draw(); // Burn card
    board.push_back(deck.draw());
    currentPhase = GamePhase::RIVER;
}

void PokerGame::bettingRound() {
    // Simplification: cette méthode devrait gérer les actions des joueurs
    // Pour l'instant, c'est un placeholder
}

void PokerGame::showdown() {
    auto activePlayers = getActivePlayers();
    
    if (activePlayers.empty()) {
        return;
    }
    
    // Si un seul joueur actif, il gagne
    if (activePlayers.size() == 1) {
        int pot = potManager.getTotalPot();
        activePlayers[0]->addChips(pot);
        std::cout << activePlayers[0]->getName() << " remporte le pot de " << pot << " jetons!" << std::endl;
        return;
    }
    
    // Évaluer les mains
    std::vector<std::pair<Player*, HandValue>> playerHands;
    
    for (auto* player : activePlayers) {
        std::vector<Card> fullHand = player->getHoleCards();
        fullHand.insert(fullHand.end(), board.begin(), board.end());
        
        HandValue hand = HandEvaluator::evaluate(fullHand);
        playerHands.push_back({player, hand});
        
        std::cout << player->getName() << " a: " << hand.toString() << std::endl;
    }
    
    // Distribuer les pots
    potManager.distributePots(playerHands);
    
    currentPhase = GamePhase::SHOWDOWN;
}

void PokerGame::playHand() {
    startNewHand();
    
    std::cout << "=== Nouvelle Main ===" << std::endl;
    displayGameState();
    
    // Pre-flop
    bettingRound();
    
    // Flop
    dealFlop();
    std::cout << "\n=== FLOP ===" << std::endl;
    displayGameState();
    bettingRound();
    
    // Turn
    dealTurn();
    std::cout << "\n=== TURN ===" << std::endl;
    displayGameState();
    bettingRound();
    
    // River
    dealRiver();
    std::cout << "\n=== RIVER ===" << std::endl;
    displayGameState();
    bettingRound();
    
    // Showdown
    std::cout << "\n=== SHOWDOWN ===" << std::endl;
    showdown();
}

void PokerGame::displayGameState() const {
    std::cout << "\nBoard: ";
    if (board.empty()) {
        std::cout << "(vide)";
    } else {
        for (const auto& card : board) {
            std::cout << "[" << card.toString() << "] ";
        }
    }
    std::cout << std::endl;
    
    std::cout << potManager.toString() << std::endl;
    
    std::cout << "\nJoueurs:" << std::endl;
    for (const auto* player : players) {
        std::cout << "  " << player->getName() << " - Stack: " << player->getStack() 
                  << " jetons";
        
        if (player->hasCards()) {
            std::cout << " - Cartes: ";
            for (const auto& card : player->getHoleCards()) {
                std::cout << "[" << card.toString() << "] ";
            }
        }
        
        std::cout << std::endl;
    }
}

std::vector<Player*> PokerGame::getActivePlayers() const {
    std::vector<Player*> active;
    for (auto* player : players) {
        if (player->isActive()) {
            active.push_back(player);
        }
    }
    return active;
}

void PokerGame::postBlinds() {
    int sbPos = (dealerPosition + 1) % players.size();
    int bbPos = (dealerPosition + 2) % players.size();
    
    players[sbPos]->bet(smallBlind);
    players[bbPos]->bet(bigBlind);
    
    potManager.addToPot(players[sbPos], smallBlind);
    potManager.addToPot(players[bbPos], bigBlind);
    potManager.setCurrentBet(bigBlind);
}

void PokerGame::moveDealer() {
    dealerPosition = (dealerPosition + 1) % players.size();
}

int PokerGame::getNextActivePlayer(int startFrom) {
    for (int i = 1; i <= static_cast<int>(players.size()); i++) {
        int idx = (startFrom + i) % players.size();
        if (players[idx]->canAct()) {
            return idx;
        }
    }
    return -1;
}

bool PokerGame::isRoundComplete() {
    int activePlayers = 0;
    int currentBet = potManager.getCurrentBet();
    
    for (auto* player : players) {
        if (player->isActive()) {
            activePlayers++;
            if (player->canAct() && player->getCurrentBet() < currentBet) {
                return false;
            }
        }
    }
    
    return activePlayers > 0;
}
