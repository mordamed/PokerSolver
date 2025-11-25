#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <random>

/**
 * @brief Class representing a 52-card deck
 */
class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;
    size_t currentIndex;

public:
    /**
     * @brief Constructor - creates and shuffles a complete deck
     */
    Deck();

    /**
     * @brief Shuffle the deck
     */
    void shuffle();

    /**
     * @brief Draw a card from the top of the deck
     */
    Card draw();

    /**
     * @brief Reset the deck (restore all cards)
     */
    void reset();

    /**
     * @brief Number of cards remaining in the deck
     */
    size_t cardsRemaining() const;

    /**
     * @brief Remove specific cards from the deck (for simulation)
     */
    void removeCards(const std::vector<Card>& cardsToRemove);

    /**
     * @brief Get all remaining cards
     */
    std::vector<Card> getRemainingCards() const;
};

#endif // DECK_H
