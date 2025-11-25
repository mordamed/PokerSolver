#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

/**
 * @brief Enumeration for card ranks (2 to Ace)
 */
enum class Rank {
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE
};

/**
 * @brief Enumeration for card suits
 */
enum class Suit {
    HEARTS,   // Hearts (h)
    DIAMONDS, // Diamonds (d)
    CLUBS,    // Clubs (c)
    SPADES    // Spades (s)
};

/**
 * @brief Class representing a poker card
 */
class Card {
private:
    Rank rank;
    Suit suit;

public:
    /**
     * @brief Constructor
     * @param r Card rank
     * @param s Card suit
     */
    Card(Rank r, Suit s);

    /**
     * @brief Get the card's rank
     */
    Rank getRank() const;

    /**
     * @brief Get the card's suit
     */
    Suit getSuit() const;

    /**
     * @brief Get the numeric value of the rank (2-14)
     */
    int getRankValue() const;

    /**
     * @brief Text representation of the card (e.g., "Ah", "Kd")
     */
    std::string toString() const;

    /**
     * @brief Create a card from a string (e.g., "Ah", "2d")
     */
    static Card fromString(const std::string& str);

    /**
     * @brief Comparison operator for sorting
     */
    bool operator<(const Card& other) const;
    bool operator==(const Card& other) const;
};

#endif // CARD_H
