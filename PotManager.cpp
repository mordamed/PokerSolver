#include "PotManager.h"
#include "HandEvaluator.h"
#include <algorithm>
#include <sstream>

PotManager::PotManager() : currentBet(0) {
    pots.push_back(Pot()); // Pot principal
}

void PotManager::addToPot(Player* player, int amount) {
    pots[0].amount += amount;
    if (!pots[0].eligiblePlayers.empty()) {
        auto it = std::find(pots[0].eligiblePlayers.begin(), 
                           pots[0].eligiblePlayers.end(), player);
        if (it == pots[0].eligiblePlayers.end()) {
            pots[0].eligiblePlayers.push_back(player);
        }
    }
}

void PotManager::calculatePots(std::vector<Player*>& players) {
    pots.clear();
    
    // Créer une liste de (joueur, montant misé)
    std::vector<std::pair<Player*, int>> bets;
    for (auto* player : players) {
        if (player->getTotalBetInHand() > 0 && player->isActive()) {
            bets.push_back({player, player->getTotalBetInHand()});
        }
    }
    
    if (bets.empty()) {
        pots.push_back(Pot());
        return;
    }
    
    // Trier par montant misé
    std::sort(bets.begin(), bets.end(), 
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    int previousLevel = 0;
    
    for (size_t i = 0; i < bets.size(); i++) {
        int currentLevel = bets[i].second;
        
        if (currentLevel > previousLevel) {
            Pot pot;
            int betPerPlayer = currentLevel - previousLevel;
            
            // Tous les joueurs à partir de i contribuent à ce pot
            for (size_t j = i; j < bets.size(); j++) {
                pot.amount += betPerPlayer;
                pot.eligiblePlayers.push_back(bets[j].first);
            }
            
            pots.push_back(pot);
            previousLevel = currentLevel;
        }
    }
    
    if (pots.empty()) {
        pots.push_back(Pot());
    }
}

void PotManager::distributePots(const std::vector<std::pair<Player*, HandValue>>& playerHands) {
    for (auto& pot : pots) {
        if (pot.amount == 0) continue;
        
        // Trouver le(s) gagnant(s) parmi les joueurs éligibles
        std::vector<std::pair<Player*, HandValue>> eligibleHands;
        
        for (const auto& ph : playerHands) {
            auto it = std::find(pot.eligiblePlayers.begin(), 
                               pot.eligiblePlayers.end(), ph.first);
            if (it != pot.eligiblePlayers.end()) {
                eligibleHands.push_back(ph);
            }
        }
        
        if (eligibleHands.empty()) continue;
        
        // Trouver la meilleure main
        HandValue bestHand = eligibleHands[0].second;
        for (const auto& eh : eligibleHands) {
            if (eh.second > bestHand) {
                bestHand = eh.second;
            }
        }
        
        // Compter les gagnants (peut y avoir des splits)
        std::vector<Player*> winners;
        for (const auto& eh : eligibleHands) {
            if (eh.second == bestHand) {
                winners.push_back(eh.first);
            }
        }
        
        // Distribuer le pot
        int amountPerWinner = pot.amount / winners.size();
        int remainder = pot.amount % winners.size();
        
        for (size_t i = 0; i < winners.size(); i++) {
            int winAmount = amountPerWinner;
            if (i < static_cast<size_t>(remainder)) {
                winAmount++; // Distribuer le reste aux premiers gagnants
            }
            winners[i]->addChips(winAmount);
        }
    }
}

int PotManager::getTotalPot() const {
    int total = 0;
    for (const auto& pot : pots) {
        total += pot.amount;
    }
    return total;
}

int PotManager::getMainPot() const {
    if (pots.empty()) return 0;
    return pots[0].amount;
}

void PotManager::reset() {
    pots.clear();
    pots.push_back(Pot());
    currentBet = 0;
}

std::string PotManager::toString() const {
    std::ostringstream oss;
    oss << "Pot Total: " << getTotalPot() << " jetons";
    
    if (pots.size() > 1) {
        oss << " (Pot Principal: " << pots[0].amount;
        for (size_t i = 1; i < pots.size(); i++) {
            oss << ", Side Pot " << i << ": " << pots[i].amount;
        }
        oss << ")";
    }
    
    return oss.str();
}
