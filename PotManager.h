#ifndef POT_MANAGER_H
#define POT_MANAGER_H

#include "Player.h"
#include "HandEvaluator.h"
#include <vector>
#include <map>

/**
 * @brief Représente un pot (principal ou side pot)
 */
struct Pot {
    int amount;
    std::vector<Player*> eligiblePlayers; // Joueurs pouvant gagner ce pot
    
    Pot() : amount(0) {}
};

/**
 * @brief Gère le pot principal et les side pots
 */
class PotManager {
private:
    std::vector<Pot> pots; // pots[0] est le pot principal
    int currentBet;        // Mise la plus élevée du tour actuel

public:
    PotManager();

    /**
     * @brief Ajouter une mise au pot
     */
    void addToPot(Player* player, int amount);

    /**
     * @brief Calculer les pots (principal et side pots) après un tour d'enchères
     */
    void calculatePots(std::vector<Player*>& players);

    /**
     * @brief Distribuer les pots aux gagnants
     */
    void distributePots(const std::vector<std::pair<Player*, HandValue>>& playerHands);

    /**
     * @brief Obtenir le montant total de tous les pots
     */
    int getTotalPot() const;

    /**
     * @brief Obtenir le pot principal
     */
    int getMainPot() const;

    /**
     * @brief Reset pour une nouvelle main
     */
    void reset();

    /**
     * @brief Obtenir la mise actuelle à suivre
     */
    int getCurrentBet() const { return currentBet; }
    void setCurrentBet(int bet) { currentBet = bet; }

    /**
     * @brief Afficher les pots
     */
    std::string toString() const;
};

#endif // POT_MANAGER_H
