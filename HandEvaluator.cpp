#include "HandEvaluator.h"
#include <algorithm>
#include <map>
#include <sstream>

std::string HandValue::toString() const {
    std::string result;
    
    switch(rank) {
        case HandRank::HIGH_CARD:        result = "High Card"; break;
        case HandRank::PAIR:             result = "Pair"; break;
        case HandRank::TWO_PAIR:         result = "Two Pair"; break;
        case HandRank::THREE_OF_A_KIND:  result = "Three of a Kind"; break;
        case HandRank::STRAIGHT:         result = "Straight"; break;
        case HandRank::FLUSH:            result = "Flush"; break;
        case HandRank::FULL_HOUSE:       result = "Full House"; break;
        case HandRank::FOUR_OF_A_KIND:   result = "Four of a Kind"; break;
        case HandRank::STRAIGHT_FLUSH:   result = "Straight Flush"; break;
        case HandRank::ROYAL_FLUSH:      result = "Royal Flush"; break;
    }
    
    return result;
}

HandValue HandEvaluator::evaluate(std::vector<Card> cards) {
    if (cards.size() < 5) {
        throw std::invalid_argument("At least 5 cards required to evaluate a hand");
    }
    
    // Si on a exactement 5 cartes, évaluation directe
    if (cards.size() == 5) {
        return evaluateFiveCards(cards);
    }
    
    // Sinon, tester toutes les combinaisons de 5 cartes parmi les 7
    HandValue bestHand;
    int n = cards.size();
    
    // Générer toutes les combinaisons de 5 cartes
    std::vector<int> indices(5);
    for (int i = 0; i < 5; i++) indices[i] = i;
    
    do {
        std::vector<Card> fiveCards;
        for (int idx : indices) {
            fiveCards.push_back(cards[idx]);
        }
        
        HandValue current = evaluateFiveCards(fiveCards);
        if (current > bestHand) {
            bestHand = current;
        }
        
    } while (std::next_permutation(indices.begin(), indices.end()) && indices[4] < n);
    
    // Méthode plus efficace: itération sur toutes les combinaisons
    bestHand = HandValue();
    for (int i = 0; i < n - 4; i++) {
        for (int j = i + 1; j < n - 3; j++) {
            for (int k = j + 1; k < n - 2; k++) {
                for (int l = k + 1; l < n - 1; l++) {
                    for (int m = l + 1; m < n; m++) {
                        std::vector<Card> fiveCards = {cards[i], cards[j], cards[k], cards[l], cards[m]};
                        HandValue current = evaluateFiveCards(fiveCards);
                        if (current > bestHand) {
                            bestHand = current;
                        }
                    }
                }
            }
        }
    }
    
    return bestHand;
}

HandValue HandEvaluator::evaluateFiveCards(const std::vector<Card>& cards) {
    if (cards.size() != 5) {
        throw std::invalid_argument("Exactly 5 cards required");
    }
    
    HandValue result;
    
    // Trier les cartes par rang décroissant
    std::vector<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), 
              [](const Card& a, const Card& b) { return a.getRankValue() > b.getRankValue(); });
    
    // Vérifier Flush
    bool isFlushHand = isFlush(sortedCards);
    
    // Vérifier Straight
    int straightHigh = 0;
    bool isStraightHand = isStraight(sortedCards, straightHigh);
    
    // Quinte Flush Royale
    if (isFlushHand && isStraightHand && straightHigh == 14) {
        result.rank = HandRank::ROYAL_FLUSH;
        result.kickers = {14};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Quinte Flush
    if (isFlushHand && isStraightHand) {
        result.rank = HandRank::STRAIGHT_FLUSH;
        result.kickers = {straightHigh};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Compter les occurrences de chaque rang
    std::map<int, int> rankCount;
    for (const auto& card : sortedCards) {
        rankCount[card.getRankValue()]++;
    }
    
    // Organiser les comptages
    std::vector<std::pair<int, int>> counts; // (count, rank)
    for (const auto& pair : rankCount) {
        counts.push_back({pair.second, pair.first});
    }
    std::sort(counts.begin(), counts.end(), 
              [](const auto& a, const auto& b) {
                  if (a.first != b.first) return a.first > b.first;
                  return a.second > b.second;
              });
    
    // Carré
    if (counts[0].first == 4) {
        result.rank = HandRank::FOUR_OF_A_KIND;
        result.kickers = {counts[0].second, counts[1].second};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Full
    if (counts[0].first == 3 && counts[1].first == 2) {
        result.rank = HandRank::FULL_HOUSE;
        result.kickers = {counts[0].second, counts[1].second};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Couleur
    if (isFlushHand) {
        result.rank = HandRank::FLUSH;
        for (const auto& card : sortedCards) {
            result.kickers.push_back(card.getRankValue());
        }
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Quinte
    if (isStraightHand) {
        result.rank = HandRank::STRAIGHT;
        result.kickers = {straightHigh};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Brelan
    if (counts[0].first == 3) {
        result.rank = HandRank::THREE_OF_A_KIND;
        result.kickers = {counts[0].second, counts[1].second, counts[2].second};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Double Paire
    if (counts[0].first == 2 && counts[1].first == 2) {
        result.rank = HandRank::TWO_PAIR;
        int high = std::max(counts[0].second, counts[1].second);
        int low = std::min(counts[0].second, counts[1].second);
        result.kickers = {high, low, counts[2].second};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Paire
    if (counts[0].first == 2) {
        result.rank = HandRank::PAIR;
        result.kickers = {counts[0].second, counts[1].second, counts[2].second, counts[3].second};
        result.score = calculateScore(result.rank, result.kickers);
        return result;
    }
    
    // Carte Haute
    result.rank = HandRank::HIGH_CARD;
    for (const auto& card : sortedCards) {
        result.kickers.push_back(card.getRankValue());
    }
    result.score = calculateScore(result.rank, result.kickers);
    
    return result;
}

int HandEvaluator::compareHands(const HandValue& hand1, const HandValue& hand2) {
    if (hand1.score > hand2.score) return 1;
    if (hand1.score < hand2.score) return -1;
    return 0;
}

bool HandEvaluator::isFlush(const std::vector<Card>& cards) {
    Suit firstSuit = cards[0].getSuit();
    for (size_t i = 1; i < cards.size(); i++) {
        if (cards[i].getSuit() != firstSuit) {
            return false;
        }
    }
    return true;
}

bool HandEvaluator::isStraight(const std::vector<Card>& cards, int& highCard) {
    std::vector<int> ranks;
    for (const auto& card : cards) {
        ranks.push_back(card.getRankValue());
    }
    std::sort(ranks.begin(), ranks.end(), std::greater<int>());
    
    // Vérifier suite normale
    bool isStraightNormal = true;
    for (size_t i = 0; i < ranks.size() - 1; i++) {
        if (ranks[i] - ranks[i + 1] != 1) {
            isStraightNormal = false;
            break;
        }
    }
    
    if (isStraightNormal) {
        highCard = ranks[0];
        return true;
    }
    
    // Vérifier suite à l'As (A-2-3-4-5)
    if (ranks[0] == 14 && ranks[1] == 5 && ranks[2] == 4 && ranks[3] == 3 && ranks[4] == 2) {
        highCard = 5; // Dans cette suite, le 5 est la carte haute
        return true;
    }
    
    return false;
}

int HandEvaluator::calculateScore(HandRank rank, const std::vector<int>& kickers) {
    int score = static_cast<int>(rank) * 100000000; // Base sur le rang de la main
    
    // Ajouter les kickers avec poids décroissants
    int multiplier = 1000000;
    for (int kicker : kickers) {
        score += kicker * multiplier;
        multiplier /= 100;
    }
    
    return score;
}
