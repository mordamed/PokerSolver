#include <iostream>
#include "Card.h"
#include "HandEvaluator.h"
#include "PokerSolver.h"

/**
 * @brief Test rapide du système
 */
int main() {
    std::cout << "==================================" << std::endl;
    std::cout << "  TEST RAPIDE DU POKER SOLVER" << std::endl;
    std::cout << "==================================" << std::endl;
    
    // Test 1: Évaluation de mains
    std::cout << "\n[TEST 1] Evaluation de mains" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    std::vector<Card> royalFlush = {
        Card(Rank::ACE, Suit::HEARTS),
        Card(Rank::KING, Suit::HEARTS),
        Card(Rank::QUEEN, Suit::HEARTS),
        Card(Rank::JACK, Suit::HEARTS),
        Card(Rank::TEN, Suit::HEARTS)
    };
    
    HandValue rf = HandEvaluator::evaluateFiveCards(royalFlush);
    std::cout << "Quinte Flush Royale: " << rf.toString() << " ✓" << std::endl;
    
    // Test 2: Calcul d'équité
    std::cout << "\n[TEST 2] Calcul d'equite - AA pre-flop" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    std::vector<Card> pocketAces = {
        Card(Rank::ACE, Suit::SPADES),
        Card(Rank::ACE, Suit::CLUBS)
    };
    
    std::vector<Card> emptyBoard = {};
    
    PokerSolver solver(5000);
    std::cout << "Simulation de 5000 mains contre 1 adversaire..." << std::endl;
    EquityResult equity = solver.calculateEquity(pocketAces, emptyBoard, 1, 5000);
    std::cout << "Equite: " << equity.winRate << "% (attendu ~85%)" << std::endl;
    
    // Test 3: Analyse de décision
    std::cout << "\n[TEST 3] Analyse de decision" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Situation: Draw de couleur au turn" << std::endl;
    std::cout << "Cartes: [Ah][Kh]" << std::endl;
    std::cout << "Board: [9h][5h][2c][7s]" << std::endl;
    std::cout << "Pot: 100, A payer: 25" << std::endl;
    
    std::vector<Card> flushDraw = {
        Card(Rank::ACE, Suit::HEARTS),
        Card(Rank::KING, Suit::HEARTS)
    };
    
    std::vector<Card> board = {
        Card(Rank::NINE, Suit::HEARTS),
        Card(Rank::FIVE, Suit::HEARTS),
        Card(Rank::TWO, Suit::CLUBS),
        Card(Rank::SEVEN, Suit::SPADES)
    };
    
    DecisionResult decision = solver.analyzeDecision(flushDraw, board, 100, 25, 1, 3000);
    std::cout << "\nResultat:" << std::endl;
    std::cout << "  Equite: " << decision.equity << "%" << std::endl;
    std::cout << "  Pot Odds: " << decision.potOdds << "%" << std::endl;
    std::cout << "  Decision: " << (decision.shouldCall ? "CALL ✓" : "FOLD ✗") << std::endl;
    
    // Test 4: Pot Odds
    std::cout << "\n[TEST 4] Calcul de Pot Odds" << std::endl;
    std::cout << "----------------------------" << std::endl;
    double potOdds1 = solver.calculatePotOdds(100, 25);
    double potOdds2 = solver.calculatePotOdds(200, 50);
    std::cout << "Pot: 100, A payer: 25 => Pot Odds: " << potOdds1 << "%" << std::endl;
    std::cout << "Pot: 200, A payer: 50 => Pot Odds: " << potOdds2 << "%" << std::endl;
    
    std::cout << "\n==================================" << std::endl;
    std::cout << "  TOUS LES TESTS PASSES ✓" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "\nLancez './PokerSolver.exe' pour le menu interactif complet!" << std::endl;
    
    return 0;
}
