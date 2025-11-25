#include "PokerSolver.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>

std::string EquityResult::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "=== Résultat Monte Carlo (" << simulations << " simulations) ===" << std::endl;
    oss << "Victoires: " << winRate << "%" << std::endl;
    oss << "Égalités:  " << tieRate << "%" << std::endl;
    oss << "Défaites:  " << loseRate << "%" << std::endl;
    return oss.str();
}

std::string DecisionResult::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "=== Analyse de Décision ===" << std::endl;
    oss << "Équité:     " << equity << "%" << std::endl;
    oss << "Pot Odds:   " << potOdds << "%" << std::endl;
    oss << "EV:         " << expectedValue << " jetons" << std::endl;
    oss << "Décision:   " << (shouldCall ? "CALL (Rentable)" : "FOLD (Non rentable)") << std::endl;
    oss << "Raison:     " << reasoning << std::endl;
    return oss.str();
}

PokerSolver::PokerSolver(int simCount) : defaultSimulations(simCount) {}

EquityResult PokerSolver::calculateEquity(
    const std::vector<Card>& holeCards,
    const std::vector<Card>& board,
    int numOpponents,
    int simulations
) {
    if (simulations <= 0) {
        simulations = defaultSimulations;
    }
    
    if (holeCards.size() != 2) {
        throw std::invalid_argument("Il faut exactement 2 cartes privées");
    }
    
    if (board.size() > 5) {
        throw std::invalid_argument("Maximum 5 cartes au board");
    }
    
    EquityResult result;
    result.simulations = simulations;
    
    int wins = 0;
    int ties = 0;
    int losses = 0;
    
    // Cartes mortes (connues)
    std::vector<Card> deadCards = holeCards;
    deadCards.insert(deadCards.end(), board.begin(), board.end());
    
    // Effectuer les simulations
    for (int i = 0; i < simulations; i++) {
        int outcome = simulateHand(holeCards, board, numOpponents, deadCards);
        
        if (outcome > 0) {
            wins++;
        } else if (outcome == 0) {
            ties++;
        } else {
            losses++;
        }
    }
    
    result.winRate = (wins * 100.0) / simulations;
    result.tieRate = (ties * 100.0) / simulations;
    result.loseRate = (losses * 100.0) / simulations;
    
    return result;
}

EquityResult PokerSolver::calculateEquityVsHands(
    const std::vector<Card>& holeCards,
    const std::vector<Card>& board,
    const std::vector<std::vector<Card>>& opponentHands,
    int simulations
) {
    if (simulations <= 0) {
        simulations = defaultSimulations;
    }
    
    EquityResult result;
    result.simulations = simulations;
    
    int wins = 0;
    int ties = 0;
    int losses = 0;
    
    // Cartes mortes
    std::vector<Card> deadCards = holeCards;
    deadCards.insert(deadCards.end(), board.begin(), board.end());
    for (const auto& oppHand : opponentHands) {
        deadCards.insert(deadCards.end(), oppHand.begin(), oppHand.end());
    }
    
    for (int i = 0; i < simulations; i++) {
        // Créer un deck sans les cartes mortes
        Deck deck;
        deck.removeCards(deadCards);
        
        // Compléter le board si nécessaire
        std::vector<Card> fullBoard = board;
        while (fullBoard.size() < 5) {
            fullBoard.push_back(deck.draw());
        }
        
        // Évaluer toutes les mains
        std::vector<Card> heroFullHand = holeCards;
        heroFullHand.insert(heroFullHand.end(), fullBoard.begin(), fullBoard.end());
        HandValue heroHand = HandEvaluator::evaluate(heroFullHand);
        
        HandValue bestOpponentHand;
        bool firstOpp = true;
        
        for (const auto& oppCards : opponentHands) {
            std::vector<Card> oppFullHand = oppCards;
            oppFullHand.insert(oppFullHand.end(), fullBoard.begin(), fullBoard.end());
            HandValue oppHand = HandEvaluator::evaluate(oppFullHand);
            
            if (firstOpp || oppHand > bestOpponentHand) {
                bestOpponentHand = oppHand;
                firstOpp = false;
            }
        }
        
        // Comparer
        if (heroHand > bestOpponentHand) {
            wins++;
        } else if (heroHand == bestOpponentHand) {
            ties++;
        } else {
            losses++;
        }
    }
    
    result.winRate = (wins * 100.0) / simulations;
    result.tieRate = (ties * 100.0) / simulations;
    result.loseRate = (losses * 100.0) / simulations;
    
    return result;
}

double PokerSolver::calculatePotOdds(int potSize, int callAmount) const {
    if (potSize <= 0 || callAmount <= 0) {
        return 0.0;
    }
    
    int totalPot = potSize + callAmount;
    return (callAmount * 100.0) / totalPot;
}

DecisionResult PokerSolver::analyzeDecision(
    const std::vector<Card>& holeCards,
    const std::vector<Card>& board,
    int potSize,
    int callAmount,
    int numOpponents,
    int simulations
) {
    DecisionResult result;
    
    // Calculer l'équité
    EquityResult equity = calculateEquity(holeCards, board, numOpponents, simulations);
    result.equity = equity.winRate + (equity.tieRate / 2.0); // Split les ties
    
    // Calculer les pot odds
    result.potOdds = calculatePotOdds(potSize, callAmount);
    
    // Calculer l'EV
    int totalPotAfterCall = potSize + callAmount;
    result.expectedValue = calculateEV(result.equity / 100.0, totalPotAfterCall, callAmount);
    
    // Décision
    result.shouldCall = result.equity > result.potOdds;
    
    // Raisonnement
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    if (result.shouldCall) {
        oss << "Votre équité (" << result.equity << "%) est supérieure aux pot odds (" 
            << result.potOdds << "%). ";
        oss << "C'est un call rentable à long terme (+EV = " << result.expectedValue << " jetons).";
    } else {
        oss << "Votre équité (" << result.equity << "%) est inférieure aux pot odds (" 
            << result.potOdds << "%). ";
        oss << "C'est un fold rentable à long terme (EV du call = " << result.expectedValue << " jetons).";
    }
    
    result.reasoning = oss.str();
    
    return result;
}

double PokerSolver::calculateEV(double equity, int potSize, int callAmount) const {
    // EV = (Probabilité de gagner × Gain) - (Probabilité de perdre × Perte)
    double winAmount = potSize;
    double loseAmount = callAmount;
    
    return (equity * winAmount) - ((1.0 - equity) * loseAmount);
}

int PokerSolver::simulateHand(
    const std::vector<Card>& heroCards,
    std::vector<Card> currentBoard,
    int numOpponents,
    const std::vector<Card>& deadCards
) {
    // Créer un deck sans les cartes mortes
    Deck deck;
    deck.removeCards(deadCards);
    
    // Compléter le board
    while (currentBoard.size() < 5) {
        currentBoard.push_back(deck.draw());
    }
    
    // Évaluer la main du héros
    std::vector<Card> heroFullHand = heroCards;
    heroFullHand.insert(heroFullHand.end(), currentBoard.begin(), currentBoard.end());
    HandValue heroHand = HandEvaluator::evaluate(heroFullHand);
    
    // Générer des mains aléatoires pour les adversaires
    HandValue bestOpponentHand;
    bool firstOpponent = true;
    
    for (int i = 0; i < numOpponents; i++) {
        std::vector<Card> opponentCards;
        opponentCards.push_back(deck.draw());
        opponentCards.push_back(deck.draw());
        
        std::vector<Card> opponentFullHand = opponentCards;
        opponentFullHand.insert(opponentFullHand.end(), currentBoard.begin(), currentBoard.end());
        
        HandValue opponentHand = HandEvaluator::evaluate(opponentFullHand);
        
        if (firstOpponent || opponentHand > bestOpponentHand) {
            bestOpponentHand = opponentHand;
            firstOpponent = false;
        }
    }
    
    // Comparer
    if (heroHand > bestOpponentHand) {
        return 1; // Victoire
    } else if (heroHand == bestOpponentHand) {
        return 0; // Égalité
    } else {
        return -1; // Défaite
    }
}
