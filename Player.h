#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <string>
#include <vector>

/**
 * @brief Player status in the hand
 */
enum class PlayerStatus {
    ACTIVE,    // Player active in the hand
    FOLDED,    // Player has folded
    ALL_IN     // Player is all-in
};

/**
 * @brief Class representing a poker player
 */
class Player {
private:
    std::string name;
    int stack;              // Available chips
    int currentBet;         // Current bet in the betting round
    int totalBetInHand;     // Total bet in the hand
    std::vector<Card> holeCards; // Hole cards (2 cards)
    PlayerStatus status;
    int position;           // Position at the table (0 = dealer)

public:
    /**
     * @brief Constructor
     * @param playerName Player name
     * @param initialStack Initial stack
     * @param pos Position at the table
     */
    Player(const std::string& playerName, int initialStack, int pos = 0);

    // Getters
    std::string getName() const { return name; }
    int getStack() const { return stack; }
    int getCurrentBet() const { return currentBet; }
    int getTotalBetInHand() const { return totalBetInHand; }
    std::vector<Card> getHoleCards() const { return holeCards; }
    PlayerStatus getStatus() const { return status; }
    int getPosition() const { return position; }

    // Setters
    void setPosition(int pos) { position = pos; }
    void setHoleCards(const std::vector<Card>& cards);

    /**
     * @brief Player actions
     */
    bool bet(int amount);           // Bet
    bool raise(int raiseAmount);    // Raise
    bool call(int amountToCall);    // Call
    void check();                   // Check
    void fold();                    // Fold
    bool allIn();                   // All-in

    /**
     * @brief Chip management
     */
    void addChips(int amount);
    void resetBet();                // Reset current round bet
    void resetForNewHand();         // Reset for new hand

    /**
     * @brief Checks
     */
    bool canAct() const;            // Can still act
    bool isActive() const;          // Is active in the hand
    bool hasCards() const;          // Has cards
};

#endif // PLAYER_H
