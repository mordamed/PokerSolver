#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "../include/PokerSolver.h"
#include "PokerGame.h"

/**
 * @brief Fonction utilitaire pour parser les cartes depuis une chaîne
 */
std::vector<Card> parseCards(const std::string& input) {
    std::vector<Card> cards;
    std::istringstream iss(input);
    std::string cardStr;
    
    while (iss >> cardStr) {
        try {
            cards.push_back(Card::fromString(cardStr));
        } catch (const std::exception& e) {
            std::cerr << "Erreur de parsing: " << cardStr << " - " << e.what() << std::endl;
        }
    }
    
    return cards;
}

/**
 * @brief Affiche un menu interactif
 */
void displayMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  POKER SOLVER - Menu Principal" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Mode Scenario - Analyse d'une situation" << std::endl;
    std::cout << "2. Test de l'evaluateur de mains" << std::endl;
    std::cout << "3. Simulation Monte Carlo simple" << std::endl;
    std::cout << "4. Exemples de scenarios pre-configures" << std::endl;
    std::cout << "5. Quitter" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Choix: ";
}

/**
 * @brief Mode Scénario - Analyse interactive
 */
void scenarioMode() {
    std::cout << "\n=== MODE SCENARIO - ANALYSE DE SITUATION ===" << std::endl;
    std::cout << "Format des cartes: Rang + Couleur (ex: Ah Kd 2s)" << std::endl;
    std::cout << "Rangs: 2-9, T(10), J, Q, K, A" << std::endl;
    std::cout << "Couleurs: h(coeur), d(carreau), c(trefle), s(pique)" << std::endl;
    
    std::string input;
    
    // Vos cartes
    std::cout << "\nEntrez vos 2 cartes (ex: Ah Kd): ";
    std::cin.ignore();
    std::getline(std::cin, input);
    std::vector<Card> holeCards = parseCards(input);
    
    if (holeCards.size() != 2) {
        std::cout << "Erreur: Vous devez entrer exactement 2 cartes." << std::endl;
        return;
    }
    
    // Board
    std::cout << "Entrez les cartes du board (0 a 5 cartes, ex: 2h 3d 4s): ";
    std::getline(std::cin, input);
    std::vector<Card> board = parseCards(input);
    
    if (board.size() > 5) {
        std::cout << "Erreur: Maximum 5 cartes au board." << std::endl;
        return;
    }
    
    // Pot et mise
    int potSize, callAmount;
    std::cout << "Taille du pot actuel: ";
    std::cin >> potSize;
    
    std::cout << "Montant a payer pour caller: ";
    std::cin >> callAmount;
    
    // Nombre d'adversaires
    int numOpponents;
    std::cout << "Nombre d'adversaires actifs: ";
    std::cin >> numOpponents;
    
    // Analyse
    std::cout << "\n=== ANALYSE EN COURS ===" << std::endl;
    std::cout << "Vos cartes: ";
    for (const auto& card : holeCards) {
        std::cout << "[" << card.toString() << "] ";
    }
    std::cout << "\nBoard: ";
    if (board.empty()) {
        std::cout << "(vide)";
    } else {
        for (const auto& card : board) {
            std::cout << "[" << card.toString() << "] ";
        }
    }
    std::cout << "\nPot: " << potSize << " jetons" << std::endl;
    std::cout << "A payer: " << callAmount << " jetons" << std::endl;
    std::cout << "Adversaires: " << numOpponents << std::endl;
    
    // Calculer avec le solver
    PokerSolver solver(10000);
    DecisionResult result = solver.analyzeDecision(holeCards, board, potSize, callAmount, numOpponents);
    
    std::cout << "\n" << result.toString() << std::endl;
}

/**
 * @brief Test de l'évaluateur de mains
 */
void testHandEvaluator() {
    std::cout << "\n=== TEST DE L'EVALUATEUR DE MAINS ===" << std::endl;
    
    // Exemple 1: Quinte Flush
    std::cout << "\nExemple 1: Quinte Flush" << std::endl;
    std::vector<Card> cards1 = {
        Card(Rank::NINE, Suit::HEARTS),
        Card(Rank::EIGHT, Suit::HEARTS),
        Card(Rank::SEVEN, Suit::HEARTS),
        Card(Rank::SIX, Suit::HEARTS),
        Card(Rank::FIVE, Suit::HEARTS)
    };
    HandValue hand1 = HandEvaluator::evaluateFiveCards(cards1);
    std::cout << "Main: ";
    for (const auto& card : cards1) std::cout << "[" << card.toString() << "] ";
    std::cout << "\nResultat: " << hand1.toString() << std::endl;
    
    // Exemple 2: Full
    std::cout << "\nExemple 2: Full" << std::endl;
    std::vector<Card> cards2 = {
        Card(Rank::ACE, Suit::HEARTS),
        Card(Rank::ACE, Suit::DIAMONDS),
        Card(Rank::ACE, Suit::CLUBS),
        Card(Rank::KING, Suit::HEARTS),
        Card(Rank::KING, Suit::DIAMONDS)
    };
    HandValue hand2 = HandEvaluator::evaluateFiveCards(cards2);
    std::cout << "Main: ";
    for (const auto& card : cards2) std::cout << "[" << card.toString() << "] ";
    std::cout << "\nResultat: " << hand2.toString() << std::endl;
    
    // Exemple 3: Paire
    std::cout << "\nExemple 3: Simple Paire" << std::endl;
    std::vector<Card> cards3 = {
        Card(Rank::QUEEN, Suit::HEARTS),
        Card(Rank::QUEEN, Suit::DIAMONDS),
        Card(Rank::ACE, Suit::CLUBS),
        Card(Rank::JACK, Suit::HEARTS),
        Card(Rank::NINE, Suit::SPADES)
    };
    HandValue hand3 = HandEvaluator::evaluateFiveCards(cards3);
    std::cout << "Main: ";
    for (const auto& card : cards3) std::cout << "[" << card.toString() << "] ";
    std::cout << "\nResultat: " << hand3.toString() << std::endl;
    
    // Comparaison
    std::cout << "\n=== COMPARAISONS ===" << std::endl;
    std::cout << "Quinte Flush vs Full: " 
              << (hand1 > hand2 ? "Quinte Flush gagne" : "Full gagne") << std::endl;
    std::cout << "Full vs Paire: " 
              << (hand2 > hand3 ? "Full gagne" : "Paire gagne") << std::endl;
}

/**
 * @brief Simulation Monte Carlo simple
 */
void monteCarloDemo() {
    std::cout << "\n=== SIMULATION MONTE CARLO ===" << std::endl;
    
    // Situation: Vous avez AA, un adversaire
    std::vector<Card> holeCards = {
        Card(Rank::ACE, Suit::HEARTS),
        Card(Rank::ACE, Suit::DIAMONDS)
    };
    
    std::vector<Card> board = {
        Card(Rank::KING, Suit::CLUBS),
        Card(Rank::QUEEN, Suit::HEARTS),
        Card(Rank::JACK, Suit::DIAMONDS)
    };
    
    std::cout << "Vos cartes: [Ah] [Ad]" << std::endl;
    std::cout << "Board: [Kc] [Qh] [Jd]" << std::endl;
    std::cout << "Adversaires: 1" << std::endl;
    
    PokerSolver solver(50000);
    std::cout << "\nSimulation de 50,000 mains..." << std::endl;
    
    EquityResult result = solver.calculateEquity(holeCards, board, 1, 50000);
    std::cout << "\n" << result.toString() << std::endl;
}

/**
 * @brief Exemples de scénarios pré-configurés
 */
void preConfiguredScenarios() {
    std::cout << "\n=== SCENARIOS PRE-CONFIGURES ===" << std::endl;
    std::cout << "1. Draw de couleur au turn" << std::endl;
    std::cout << "2. Paire d'As pre-flop contre 3 adversaires" << std::endl;
    std::cout << "3. Open-ended straight draw a la river" << std::endl;
    std::cout << "\nChoix (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    PokerSolver solver(20000);
    
    switch(choice) {
        case 1: {
            // Draw de couleur au turn
            std::cout << "\n=== SCENARIO 1: DRAW DE COULEUR ===" << std::endl;
            std::vector<Card> hole = {Card(Rank::ACE, Suit::HEARTS), Card(Rank::KING, Suit::HEARTS)};
            std::vector<Card> brd = {
                Card(Rank::NINE, Suit::HEARTS),
                Card(Rank::FIVE, Suit::HEARTS),
                Card(Rank::TWO, Suit::CLUBS),
                Card(Rank::SEVEN, Suit::SPADES)
            };
            
            std::cout << "Vos cartes: [Ah] [Kh]" << std::endl;
            std::cout << "Board: [9h] [5h] [2c] [7s]" << std::endl;
            std::cout << "Pot: 100 jetons, A payer: 30 jetons" << std::endl;
            
            DecisionResult result = solver.analyzeDecision(hole, brd, 100, 30, 1);
            std::cout << "\n" << result.toString() << std::endl;
            break;
        }
        
        case 2: {
            // AA pre-flop
            std::cout << "\n=== SCENARIO 2: PAIRE D'AS PRE-FLOP ===" << std::endl;
            std::vector<Card> hole = {Card(Rank::ACE, Suit::SPADES), Card(Rank::ACE, Suit::CLUBS)};
            std::vector<Card> brd = {};
            
            std::cout << "Vos cartes: [As] [Ac]" << std::endl;
            std::cout << "Board: (Pre-flop)" << std::endl;
            std::cout << "Pot: 50 jetons, A payer: 20 jetons" << std::endl;
            std::cout << "3 adversaires" << std::endl;
            
            EquityResult equity = solver.calculateEquity(hole, brd, 3);
            std::cout << "\n" << equity.toString() << std::endl;
            break;
        }
        
        case 3: {
            // Open-ended straight draw
            std::cout << "\n=== SCENARIO 3: OPEN-ENDED STRAIGHT DRAW ===" << std::endl;
            std::vector<Card> hole = {Card(Rank::JACK, Suit::DIAMONDS), Card(Rank::TEN, Suit::CLUBS)};
            std::vector<Card> brd = {
                Card(Rank::QUEEN, Suit::HEARTS),
                Card(Rank::NINE, Suit::SPADES),
                Card(Rank::TWO, Suit::DIAMONDS),
                Card(Rank::FIVE, Suit::CLUBS)
            };
            
            std::cout << "Vos cartes: [Jd] [Tc]" << std::endl;
            std::cout << "Board: [Qh] [9s] [2d] [5c]" << std::endl;
            std::cout << "Pot: 150 jetons, A payer: 50 jetons" << std::endl;
            
            DecisionResult result = solver.analyzeDecision(hole, brd, 150, 50, 2);
            std::cout << "\n" << result.toString() << std::endl;
            break;
        }
        
        default:
            std::cout << "Choix invalide." << std::endl;
    }
}

/**
 * @brief Programme principal
 */
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  BIENVENUE DANS LE POKER SOLVER" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Un moteur de poker complet avec calculateur" << std::endl;
    std::cout << "de probabilites base sur Monte Carlo" << std::endl;
    
    bool running = true;
    
    while (running) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                scenarioMode();
                break;
            
            case 2:
                testHandEvaluator();
                break;
            
            case 3:
                monteCarloDemo();
                break;
            
            case 4:
                preConfiguredScenarios();
                break;
            
            case 5:
                std::cout << "\nMerci d'avoir utilise le Poker Solver!" << std::endl;
                running = false;
                break;
            
            default:
                std::cout << "\nChoix invalide. Reessayez." << std::endl;
        }
        
        if (running) {
            std::cout << "\nAppuyez sur Entree pour continuer...";
            std::cin.ignore();
            std::cin.get();
        }
    }
    
    return 0;
}
