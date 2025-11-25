#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

/**
 * @brief Énumération pour les rangs des cartes (2 à As)
 */
enum class Rank {
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE
};

/**
 * @brief Énumération pour les couleurs (suits)
 */
enum class Suit {
    HEARTS,   // Coeur (h)
    DIAMONDS, // Carreau (d)
    CLUBS,    // Trèfle (c)
    SPADES    // Pique (s)
};

/**
 * @brief Classe représentant une carte de poker
 */
class Card {
private:
    Rank rank;
    Suit suit;

public:
    /**
     * @brief Constructeur
     * @param r Rang de la carte
     * @param s Couleur de la carte
     */
    Card(Rank r, Suit s);

    /**
     * @brief Obtenir le rang de la carte
     */
    Rank getRank() const;

    /**
     * @brief Obtenir la couleur de la carte
     */
    Suit getSuit() const;

    /**
     * @brief Obtenir la valeur numérique du rang (2-14)
     */
    int getRankValue() const;

    /**
     * @brief Représentation textuelle de la carte (ex: "Ah", "Kd")
     */
    std::string toString() const;

    /**
     * @brief Créer une carte depuis une chaîne (ex: "Ah", "2d")
     */
    static Card fromString(const std::string& str);

    /**
     * @brief Opérateur de comparaison pour le tri
     */
    bool operator<(const Card& other) const;
    bool operator==(const Card& other) const;
};

#endif // CARD_H
