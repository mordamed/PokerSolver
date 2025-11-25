#include "Deck.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>

Deck::Deck() : currentIndex(0) {
    // Initialize random generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
    
    reset();
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
    currentIndex = 0;
}

Card Deck::draw() {
    if (currentIndex >= cards.size()) {
        throw std::runtime_error("No more cards in the deck!");
    }
    return cards[currentIndex++];
}

void Deck::reset() {
    cards.clear();
    currentIndex = 0;
    
    // Create the 52 cards
    for (int s = 0; s < 4; s++) {
        Suit suit = static_cast<Suit>(s);
        for (int r = 2; r <= 14; r++) {
            Rank rank = static_cast<Rank>(r);
            cards.emplace_back(rank, suit);
        }
    }
    
    shuffle();
}

size_t Deck::cardsRemaining() const {
    return cards.size() - currentIndex;
}

void Deck::removeCards(const std::vector<Card>& cardsToRemove) {
    for (const auto& cardToRemove : cardsToRemove) {
        auto it = std::find(cards.begin(), cards.end(), cardToRemove);
        if (it != cards.end()) {
            cards.erase(it);
        }
    }
    currentIndex = 0;
}

std::vector<Card> Deck::getRemainingCards() const {
    return std::vector<Card>(cards.begin() + currentIndex, cards.end());
}
