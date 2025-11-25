#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "Card.h"
#include <vector>
#include <string>

/**
 * @brief Types de combinaisons de poker (du plus faible au plus fort)
 */
enum class HandRank {
    HIGH_CARD = 1,
    PAIR = 2,
    TWO_PAIR = 3,
    THREE_OF_A_KIND = 4,
    STRAIGHT = 5,
    FLUSH = 6,
    FULL_HOUSE = 7,
    FOUR_OF_A_KIND = 8,
    STRAIGHT_FLUSH = 9,
    ROYAL_FLUSH = 10
};

/**
 * @brief Résultat de l'évaluation d'une main
 */
struct HandValue {
    HandRank rank;
    std::vector<int> kickers; // Pour départager les égalités
    int score; // Score numérique unique pour comparaison rapide
    
    HandValue() : rank(HandRank::HIGH_CARD), score(0) {}
    
    bool operator>(const HandValue& other) const {
        return score > other.score;
    }
    
    bool operator<(const HandValue& other) const {
        return score < other.score;
    }
    
    bool operator==(const HandValue& other) const {
        return score == other.score;
    }
    
    std::string toString() const;
};

/**
 * @brief Évaluateur de mains de poker (meilleure main de 5 cartes sur 7)
 */
class HandEvaluator {
public:
    /**
     * @brief Évalue la meilleure main de 5 cartes parmi 7 cartes
     * @param cards 7 cartes (2 cartes privées + 5 cartes du board)
     * @return Valeur de la main
     */
    static HandValue evaluate(std::vector<Card> cards);

    /**
     * @brief Évalue une main de 5 cartes exactement
     */
    static HandValue evaluateFiveCards(const std::vector<Card>& cards);

    /**
     * @brief Compare deux mains et retourne 1 si main1 gagne, -1 si main2 gagne, 0 si égalité
     */
    static int compareHands(const HandValue& hand1, const HandValue& hand2);

private:
    static bool isFlush(const std::vector<Card>& cards);
    static bool isStraight(const std::vector<Card>& cards, int& highCard);
    static std::vector<int> getRankCounts(const std::vector<Card>& cards);
    static HandValue evaluateRankCounts(const std::vector<int>& counts, const std::vector<Card>& cards);
    static int calculateScore(HandRank rank, const std::vector<int>& kickers);
};

#endif // HAND_EVALUATOR_H
