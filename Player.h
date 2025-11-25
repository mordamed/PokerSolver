#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <string>
#include <vector>

/**
 * @brief Statut d'un joueur dans la main
 */
enum class PlayerStatus {
    ACTIVE,    // Joueur actif dans la main
    FOLDED,    // Joueur a couché ses cartes
    ALL_IN     // Joueur est all-in
};

/**
 * @brief Classe représentant un joueur de poker
 */
class Player {
private:
    std::string name;
    int stack;              // Jetons disponibles
    int currentBet;         // Mise actuelle dans le tour d'enchères
    int totalBetInHand;     // Mise totale dans la main
    std::vector<Card> holeCards; // Cartes privées (2 cartes)
    PlayerStatus status;
    int position;           // Position à la table (0 = dealer)

public:
    /**
     * @brief Constructeur
     * @param playerName Nom du joueur
     * @param initialStack Stack initial
     * @param pos Position à la table
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
     * @brief Actions du joueur
     */
    bool bet(int amount);           // Miser
    bool raise(int raiseAmount);    // Relancer
    bool call(int amountToCall);    // Suivre
    void check();                   // Checker
    void fold();                    // Se coucher
    bool allIn();                   // All-in

    /**
     * @brief Gestion des jetons
     */
    void addChips(int amount);
    void resetBet();                // Reset la mise du tour actuel
    void resetForNewHand();         // Reset pour une nouvelle main

    /**
     * @brief Vérifications
     */
    bool canAct() const;            // Peut encore agir
    bool isActive() const;          // Est actif dans la main
    bool hasCards() const;          // A des cartes
};

#endif // PLAYER_H
