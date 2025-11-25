#include "../include/Player.h"
#include <algorithm>
#include <stdexcept>

Player::Player(const std::string& playerName, int initialStack, int pos)
    : name(playerName), stack(initialStack), currentBet(0), 
      totalBetInHand(0), status(PlayerStatus::ACTIVE), position(pos) {
}

void Player::setHoleCards(const std::vector<Card>& cards) {
    if (cards.size() != 2) {
        throw std::invalid_argument("A player must have exactly 2 cards");
    }
    holeCards = cards;
}

bool Player::bet(int amount) {
    if (amount <= 0 || amount > stack) {
        return false;
    }
    
    stack -= amount;
    currentBet += amount;
    totalBetInHand += amount;
    
    if (stack == 0) {
        status = PlayerStatus::ALL_IN;
    }
    
    return true;
}

bool Player::raise(int raiseAmount) {
    return bet(raiseAmount);
}

bool Player::call(int amountToCall) {
    int toCall = amountToCall - currentBet;
    
    if (toCall <= 0) {
        return true; // Déjà égalisé
    }
    
    if (toCall >= stack) {
        // All-in
        return allIn();
    }
    
    return bet(toCall);
}

void Player::check() {
    // Ne rien faire, juste passer
}

void Player::fold() {
    status = PlayerStatus::FOLDED;
    holeCards.clear();
}

bool Player::allIn() {
    int amount = stack;
    if (amount <= 0) {
        return false;
    }
    
    currentBet += amount;
    totalBetInHand += amount;
    stack = 0;
    status = PlayerStatus::ALL_IN;
    
    return true;
}

void Player::addChips(int amount) {
    stack += amount;
}

void Player::resetBet() {
    currentBet = 0;
}

void Player::resetForNewHand() {
    currentBet = 0;
    totalBetInHand = 0;
    holeCards.clear();
    status = PlayerStatus::ACTIVE;
}

bool Player::canAct() const {
    return status == PlayerStatus::ACTIVE && stack > 0;
}

bool Player::isActive() const {
    return status != PlayerStatus::FOLDED;
}

bool Player::hasCards() const {
    return !holeCards.empty();
}
