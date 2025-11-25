#ifndef POKER_GAME_H
#define POKER_GAME_H

#include "Player.h"
#include "Deck.h"
#include "PotManager.h"
#include <vector>
#include <string>

/**
 * @brief Game phases
 */
enum class GamePhase {
    PRE_FLOP,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

/**
 * @brief Class managing a poker game
 */
class PokerGame {
private:
    std::vector<Player*> players;
    Deck deck;
    PotManager potManager;
    std::vector<Card> board;
    GamePhase currentPhase;
    int dealerPosition;
    int smallBlind;
    int bigBlind;
    int currentPlayerIndex;

public:
    /**
     * @brief Constructor
     * @param numPlayers Number of players (2 to 9)
     * @param sb Small blind
     * @param bb Big blind
     * @param startingStack Initial stack for each player
     */
    PokerGame(int numPlayers, int sb, int bb, int startingStack = 1000);
    
    ~PokerGame();

    /**
     * @brief Start a new hand
     */
    void startNewHand();

    /**
     * @brief Deal hole cards
     */
    void dealHoleCards();

    /**
     * @brief Deal the flop (3 cards)
     */
    void dealFlop();

    /**
     * @brief Deal the turn (1 card)
     */
    void dealTurn();

    /**
     * @brief Deal the river (1 card)
     */
    void dealRiver();

    /**
     * @brief Betting round
     */
    void bettingRound();

    /**
     * @brief Showdown - determine the winner
     */
    void showdown();

    /**
     * @brief Play a complete hand
     */
    void playHand();

    /**
     * @brief Display the current game state
     */
    void displayGameState() const;

    /**
     * @brief Getters
     */
    std::vector<Card> getBoard() const { return board; }
    int getPotSize() const { return potManager.getTotalPot(); }
    GamePhase getCurrentPhase() const { return currentPhase; }
    std::vector<Player*> getActivePlayers() const;

private:
    void postBlinds();
    void moveDealer();
    int getNextActivePlayer(int startFrom);
    bool isRoundComplete();
};

#endif // POKER_GAME_H
