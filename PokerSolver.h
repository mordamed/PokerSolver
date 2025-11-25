#ifndef POKER_SOLVER_H
#define POKER_SOLVER_H

#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "Player.h"
#include <vector>
#include <string>

/**
 * @brief Result of a Monte Carlo simulation
 */
struct EquityResult {
    double winRate;        // Win percentage
    double tieRate;        // Tie percentage
    double loseRate;       // Loss percentage
    int simulations;       // Number of simulations performed
    
    std::string toString() const;
};

/**
 * @brief Result of a decision analysis
 */
struct DecisionResult {
    double equity;         // Equity (% to win)
    double potOdds;        // Pot odds (% to invest)
    double expectedValue;  // EV of the decision
    bool shouldCall;       // Recommendation: Call or Fold
    std::string reasoning; // Explanation of the decision
    
    std::string toString() const;
};

/**
 * @brief Poker Solver/Assistant - Equity and EV calculation
 */
class PokerSolver {
private:
    int defaultSimulations;

public:
    /**
     * @brief Constructor
     * @param simCount Number of Monte Carlo simulations by default
     */
    PokerSolver(int simCount = 10000);

    /**
     * @brief Calculates hand equity via Monte Carlo simulation
     * @param holeCards Your 2 hole cards
     * @param board Community cards (0 to 5 cards)
     * @param numOpponents Number of active opponents
     * @param simulations Number of simulations (0 = use default)
     * @return Equity result
     */
    EquityResult calculateEquity(
        const std::vector<Card>& holeCards,
        const std::vector<Card>& board,
        int numOpponents,
        int simulations = 0
    );

    /**
     * @brief Calculates equity against specific opponent ranges
     * @param holeCards Your cards
     * @param board Community cards
     * @param opponentHands Opponent hands (if known)
     * @param simulations Number of simulations
     */
    EquityResult calculateEquityVsHands(
        const std::vector<Card>& holeCards,
        const std::vector<Card>& board,
        const std::vector<std::vector<Card>>& opponentHands,
        int simulations = 0
    );

    /**
     * @brief Calculates pot odds
     * @param potSize Current pot size
     * @param callAmount Amount to call
     * @return Pot odds percentage (e.g., 25.0 for 25%)
     */
    double calculatePotOdds(int potSize, int callAmount) const;

    /**
     * @brief Analyzes a call/fold decision
     * @param holeCards Your cards
     * @param board Community cards
     * @param potSize Pot size
     * @param callAmount Amount to call
     * @param numOpponents Number of opponents
     * @param simulations Number of simulations
     * @return Complete decision analysis
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
     * @brief Calculates the Expected Value (EV) of a call
     * @param equity Equity (0.0 to 1.0)
     * @param potSize Pot size after your call
     * @param callAmount Amount to call
     * @return EV in chips
     */
    double calculateEV(double equity, int potSize, int callAmount) const;

private:
    /**
     * @brief Simulates a complete hand and returns the winner
     */
    int simulateHand(
        const std::vector<Card>& heroCards,
        std::vector<Card> currentBoard,
        int numOpponents,
        const std::vector<Card>& deadCards
    );
};

#endif // POKER_SOLVER_H
