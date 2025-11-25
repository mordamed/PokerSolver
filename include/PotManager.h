#ifndef POT_MANAGER_H
#define POT_MANAGER_H

#include "Player.h"
#include "HandEvaluator.h"
#include <vector>
#include <map>

/**
 * @brief Represents a pot (main or side pot)
 */
struct Pot {
    int amount;
    std::vector<Player*> eligiblePlayers; // Players eligible to win this pot
    
    Pot() : amount(0) {}
};

/**
 * @brief Manages the main pot and side pots
 */
class PotManager {
private:
    std::vector<Pot> pots; // pots[0] est le pot principal
    int currentBet;        // Mise la plus élevée du tour actuel

public:
    PotManager();

    /**
     * @brief Add a bet to the pot
     */
    void addToPot(Player* player, int amount);

    /**
     * @brief Calculate pots (main and side pots) after a betting round
     */
    void calculatePots(std::vector<Player*>& players);

    /**
     * @brief Distribute pots to winners
     */
    void distributePots(const std::vector<std::pair<Player*, HandValue>>& playerHands);

    /**
     * @brief Get the total amount of all pots
     */
    int getTotalPot() const;

    /**
     * @brief Get the main pot
     */
    int getMainPot() const;

    /**
     * @brief Reset for a new hand
     */
    void reset();

    /**
     * @brief Get the current bet to call
     */
    int getCurrentBet() const { return currentBet; }
    void setCurrentBet(int bet) { currentBet = bet; }

    /**
     * @brief Display the pots
     */
    std::string toString() const;
};

#endif // POT_MANAGER_H
