#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <random>

/**
 * @brief Classe représentant un paquet de 52 cartes
 */
class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;
    size_t currentIndex;

public:
    /**
     * @brief Constructeur - crée et mélange un paquet complet
     */
    Deck();

    /**
     * @brief Mélange le paquet
     */
    void shuffle();

    /**
     * @brief Tire une carte du dessus du paquet
     */
    Card draw();

    /**
     * @brief Réinitialise le paquet (remet toutes les cartes)
     */
    void reset();

    /**
     * @brief Nombre de cartes restantes dans le paquet
     */
    size_t cardsRemaining() const;

    /**
     * @brief Retirer des cartes spécifiques du paquet (pour simulation)
     */
    void removeCards(const std::vector<Card>& cardsToRemove);

    /**
     * @brief Obtenir toutes les cartes restantes
     */
    std::vector<Card> getRemainingCards() const;
};

#endif // DECK_H
