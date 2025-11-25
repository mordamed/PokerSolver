#ifndef POKER_GAME_H
#define POKER_GAME_H

#include "Player.h"
#include "Deck.h"
#include "PotManager.h"
#include <vector>
#include <string>

/**
 * @brief Phases du jeu
 */
enum class GamePhase {
    PRE_FLOP,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

/**
 * @brief Classe gérant une partie de poker
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
     * @brief Constructeur
     * @param numPlayers Nombre de joueurs (2 à 9)
     * @param sb Small blind
     * @param bb Big blind
     * @param startingStack Stack initial pour chaque joueur
     */
    PokerGame(int numPlayers, int sb, int bb, int startingStack = 1000);
    
    ~PokerGame();

    /**
     * @brief Démarrer une nouvelle main
     */
    void startNewHand();

    /**
     * @brief Distribuer les cartes privées
     */
    void dealHoleCards();

    /**
     * @brief Distribuer le flop (3 cartes)
     */
    void dealFlop();

    /**
     * @brief Distribuer le turn (1 carte)
     */
    void dealTurn();

    /**
     * @brief Distribuer la river (1 carte)
     */
    void dealRiver();

    /**
     * @brief Tour d'enchères
     */
    void bettingRound();

    /**
     * @brief Showdown - déterminer le gagnant
     */
    void showdown();

    /**
     * @brief Jouer une main complète
     */
    void playHand();

    /**
     * @brief Afficher l'état actuel du jeu
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
