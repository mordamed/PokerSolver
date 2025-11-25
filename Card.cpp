#include "Card.h"
#include <stdexcept>
#include <cctype>

Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

Rank Card::getRank() const {
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}

int Card::getRankValue() const {
    return static_cast<int>(rank);
}

std::string Card::toString() const {
    std::string result;
    
    // Rang
    switch(rank) {
        case Rank::TWO:   result += "2"; break;
        case Rank::THREE: result += "3"; break;
        case Rank::FOUR:  result += "4"; break;
        case Rank::FIVE:  result += "5"; break;
        case Rank::SIX:   result += "6"; break;
        case Rank::SEVEN: result += "7"; break;
        case Rank::EIGHT: result += "8"; break;
        case Rank::NINE:  result += "9"; break;
        case Rank::TEN:   result += "T"; break;
        case Rank::JACK:  result += "J"; break;
        case Rank::QUEEN: result += "Q"; break;
        case Rank::KING:  result += "K"; break;
        case Rank::ACE:   result += "A"; break;
    }
    
    // Couleur
    switch(suit) {
        case Suit::HEARTS:   result += "h"; break;
        case Suit::DIAMONDS: result += "d"; break;
        case Suit::CLUBS:    result += "c"; break;
        case Suit::SPADES:   result += "s"; break;
    }
    
    return result;
}

Card Card::fromString(const std::string& str) {
    if (str.length() != 2) {
        throw std::invalid_argument("Format de carte invalide: " + str);
    }
    
    // Parser le rang
    Rank r;
    char rankChar = std::toupper(str[0]);
    switch(rankChar) {
        case '2': r = Rank::TWO; break;
        case '3': r = Rank::THREE; break;
        case '4': r = Rank::FOUR; break;
        case '5': r = Rank::FIVE; break;
        case '6': r = Rank::SIX; break;
        case '7': r = Rank::SEVEN; break;
        case '8': r = Rank::EIGHT; break;
        case '9': r = Rank::NINE; break;
        case 'T': r = Rank::TEN; break;
        case 'J': r = Rank::JACK; break;
        case 'Q': r = Rank::QUEEN; break;
        case 'K': r = Rank::KING; break;
        case 'A': r = Rank::ACE; break;
        default: throw std::invalid_argument("Rang invalide: " + std::string(1, rankChar));
    }
    
    // Parser la couleur
    Suit s;
    char suitChar = std::tolower(str[1]);
    switch(suitChar) {
        case 'h': s = Suit::HEARTS; break;
        case 'd': s = Suit::DIAMONDS; break;
        case 'c': s = Suit::CLUBS; break;
        case 's': s = Suit::SPADES; break;
        default: throw std::invalid_argument("Couleur invalide: " + std::string(1, suitChar));
    }
    
    return Card(r, s);
}

bool Card::operator<(const Card& other) const {
    if (rank != other.rank) {
        return rank < other.rank;
    }
    return suit < other.suit;
}

bool Card::operator==(const Card& other) const {
    return rank == other.rank && suit == other.suit;
}
