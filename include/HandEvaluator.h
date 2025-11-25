#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "Card.h"
#include <vector>
#include <string>

/**
 * @brief Poker hand types (from weakest to strongest)
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
 * @brief Result of a hand evaluation
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
 * @brief Poker hand evaluator (best 5-card hand from 7)
 */
class HandEvaluator {
public:
    /**
     * @brief Evaluates the best 5-card hand from 7 cards
     * @param cards 7 cards (2 hole cards + 5 board cards)
     * @return Hand value
     */
    static HandValue evaluate(std::vector<Card> cards);

    /**
     * @brief Evaluates exactly 5 cards
     */
    static HandValue evaluateFiveCards(const std::vector<Card>& cards);

    /**
     * @brief Compares two hands and returns 1 if hand1 wins, -1 if hand2 wins, 0 if tie
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
