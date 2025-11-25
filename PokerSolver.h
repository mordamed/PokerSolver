#ifndef POKER_SOLVER_H
#define POKER_SOLVER_H

#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "Player.h"
#include <vector>
#include <string>

/**
 * @brief Résultat d'une simulation Monte Carlo
 */
struct EquityResult {
    double winRate;        // Pourcentage de victoires
    double tieRate;        // Pourcentage d'égalités
    double loseRate;       // Pourcentage de défaites
    int simulations;       // Nombre de simulations effectuées
    
    std::string toString() const;
};

/**
 * @brief Résultat d'analyse de décision
 */
struct DecisionResult {
    double equity;         // Équité (% de gagner)
    double potOdds;        // Cote du pot (% à investir)
    double expectedValue;  // EV de la décision
    bool shouldCall;       // Recommandation: Call ou Fold
    std::string reasoning; // Explication de la décision
    
    std::string toString() const;
};

/**
 * @brief Solver/Assistant de poker - Calcul d'équité et d'EV
 */
class PokerSolver {
private:
    int defaultSimulations;

public:
    /**
     * @brief Constructeur
     * @param simCount Nombre de simulations Monte Carlo par défaut
     */
    PokerSolver(int simCount = 10000);

    /**
     * @brief Calcule l'équité d'une main via simulation Monte Carlo
     * @param holeCards Vos 2 cartes privées
     * @param board Cartes communes (0 à 5 cartes)
     * @param numOpponents Nombre d'adversaires actifs
     * @param simulations Nombre de simulations (0 = utiliser défaut)
     * @return Résultat de l'équité
     */
    EquityResult calculateEquity(
        const std::vector<Card>& holeCards,
        const std::vector<Card>& board,
        int numOpponents,
        int simulations = 0
    );

    /**
     * @brief Calcule l'équité contre des ranges d'adversaires spécifiques
     * @param holeCards Vos cartes
     * @param board Cartes communes
     * @param opponentHands Mains des adversaires (si connues)
     * @param simulations Nombre de simulations
     */
    EquityResult calculateEquityVsHands(
        const std::vector<Card>& holeCards,
        const std::vector<Card>& board,
        const std::vector<std::vector<Card>>& opponentHands,
        int simulations = 0
    );

    /**
     * @brief Calcule les pot odds
     * @param potSize Taille actuelle du pot
     * @param callAmount Montant à payer
     * @return Pourcentage de pot odds (ex: 25.0 pour 25%)
     */
    double calculatePotOdds(int potSize, int callAmount) const;

    /**
     * @brief Analyse une décision de call/fold
     * @param holeCards Vos cartes
     * @param board Cartes communes
     * @param potSize Taille du pot
     * @param callAmount Montant à payer
     * @param numOpponents Nombre d'adversaires
     * @param simulations Nombre de simulations
     * @return Analyse complète de la décision
     */
    DecisionResult analyzeDecision(
        const std::vector<Card>& holeCards,
        const std::vector<Card>& board,
        int potSize,
        int callAmount,
        int numOpponents,
        int simulations = 0
    );

    /**
     * @brief Calcule l'Expected Value (EV) d'un call
     * @param equity Équité (0.0 à 1.0)
     * @param potSize Taille du pot après votre call
     * @param callAmount Montant à payer
     * @return EV en jetons
     */
    double calculateEV(double equity, int potSize, int callAmount) const;

private:
    /**
     * @brief Simule une main complète et retourne le gagnant
     */
    int simulateHand(
        const std::vector<Card>& heroCards,
        std::vector<Card> currentBoard,
        int numOpponents,
        const std::vector<Card>& deadCards
    );
};

#endif // POKER_SOLVER_H
