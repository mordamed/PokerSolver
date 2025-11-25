# Guide de Démarrage Rapide - Poker Solver

## 🚀 Compilation Rapide

### Windows (avec MinGW/g++)
```bash
cd c:\Users\tajan\Documents\Codes\PokerSolver
g++ -std=c++17 -O2 -o PokerSolver.exe Card.cpp Deck.cpp HandEvaluator.cpp Player.cpp PotManager.cpp PokerGame.cpp PokerSolver.cpp main.cpp
```

### Avec le Makefile (recommandé)
```bash
make
```

## ▶️ Exécution

```bash
# Programme principal avec menu interactif
./PokerSolver.exe

# Programme de test rapide
./test.exe
```

## 📚 Exemples d'Utilisation

### Exemple 1: Analyse d'une Main Rapide (Mode Scénario)

**Situation**: Vous avez [As][Ac], le board est [Kh][Qh][Jh]

1. Lancez le programme: `./PokerSolver.exe`
2. Choisissez l'option `1` (Mode Scenario)
3. Entrez vos cartes: `As Ac`
4. Entrez le board: `Kh Qh Jh`
5. Taille du pot: `100`
6. Montant à payer: `30`
7. Nombre d'adversaires: `1`

**Résultat attendu**:
```
Équité:     ~67%
Pot Odds:   23.08%
Décision:   CALL (Rentable)
```

### Exemple 2: Test de l'Évaluateur

**Option 2** du menu principal vous montre:
- Quinte Flush vs Full vs Paire
- Comparaisons automatiques
- Affichage des combinaisons

### Exemple 3: Simulation Monte Carlo

**Option 3** simule automatiquement:
- Paire d'As contre 1 adversaire
- Board: [Kc][Qh][Jd]
- 50,000 simulations
- Résultat: ~67% d'équité

### Exemple 4: Scénarios Pré-configurés

**Option 4** offre 3 scénarios classiques:

1. **Draw de Couleur au Turn**
   - Cartes: [Ah][Kh]
   - Board: [9h][5h][2c][7s]
   - Situation typique de flush draw

2. **AA Pre-flop vs 3 Adversaires**
   - Cartes: [As][Ac]
   - 3 adversaires actifs
   - Calcul d'équité pre-flop

3. **Open-Ended Straight Draw**
   - Cartes: [Jd][Tc]
   - Board: [Qh][9s][2d][5c]
   - 8 outs pour la quinte

## 🎯 Cas d'Usage Typiques

### 1. Vérifier si un Call est Rentable

```
Vos cartes: 8h 7h (draw de couleur)
Board: 2h 5h Kc
Pot: 80 jetons
À payer: 20 jetons
```

→ Le solver calcule automatiquement si l'équité > pot odds

### 2. Comparer Deux Mains

```cpp
// Dans le code ou via le mode test
HandValue main1 = HandEvaluator::evaluateFiveCards(cartes1);
HandValue main2 = HandEvaluator::evaluateFiveCards(cartes2);

if (main1 > main2) {
    std::cout << "Main 1 gagne!" << std::endl;
}
```

### 3. Calculer l'Équité Pre-flop

```
Cartes: As Ac
Adversaires: 3
Board: (vide)
```

→ Simulation de toutes les fins possibles

## 📊 Interprétation des Résultats

### Équité (Win Rate)
- **85%+**: Main très forte (ex: AA pre-flop)
- **60-85%**: Main forte (ex: grosse paire avec board favorable)
- **40-60%**: Main moyenne (coin flip)
- **20-40%**: Draw (couleur, quinte)
- **<20%**: Main faible, fold recommandé

### Pot Odds
- **Formule**: (Mise à payer) / (Pot total après call) × 100
- **Exemple**: 20 jetons dans un pot de 100
  - Pot total après call: 120
  - Pot Odds: 20/120 = 16.67%

### Décision (Call/Fold)
```
Si Équité > Pot Odds  →  CALL (+EV)
Si Équité < Pot Odds  →  FOLD (-EV)
```

**Exemple**:
- Équité: 35%
- Pot Odds: 20%
- Décision: **CALL** (35% > 20%)
- C'est rentable à long terme!

## 🔧 Personnalisation

### Changer le Nombre de Simulations

Par défaut: 10,000 simulations

```cpp
// Pour plus de précision (plus lent)
PokerSolver solver(50000);

// Pour des tests rapides
PokerSolver solver(1000);
```

### Ajouter Vos Propres Scénarios

Modifiez `main.cpp`, fonction `preConfiguredScenarios()`:

```cpp
case 4: {
    std::cout << "=== MON SCENARIO ===" << std::endl;
    std::vector<Card> hole = {
        Card(Rank::JACK, Suit::HEARTS), 
        Card(Rank::TEN, Suit::HEARTS)
    };
    std::vector<Card> board = {
        Card(Rank::QUEEN, Suit::DIAMONDS),
        Card(Rank::KING, Suit::CLUBS),
        Card(Rank::ACE, Suit::SPADES)
    };
    
    DecisionResult result = solver.analyzeDecision(hole, board, 200, 50, 2);
    std::cout << result.toString() << std::endl;
    break;
}
```

## 🐛 Dépannage

### "Plus de cartes dans le paquet"
- Vérifiez que vous n'avez pas de cartes dupliquées
- Exemple invalide: `Ah Kd Ah` (As de cœur en double)

### Compilation échoue
```bash
# Vérifier la version de g++
g++ --version

# Doit être >= C++11, recommandé C++17
```

### Résultats Incohérents
- Augmentez le nombre de simulations
- Monte Carlo est probabiliste, variance normale ±2%

## 📈 Optimisations

### Pour des Résultats Plus Rapides
1. Réduire les simulations: `solver(5000)`
2. Compiler en mode Release: `-O3`
3. Utiliser plusieurs threads (avancé)

### Pour Plus de Précision
1. Augmenter les simulations: `solver(100000)`
2. Faire plusieurs runs et moyenner
3. Utiliser des ranges d'adversaires plus précis

## 💡 Conseils d'Utilisation

1. **Pre-flop**: 5,000-10,000 simulations suffisent
2. **Flop/Turn**: 10,000-20,000 recommandées
3. **River**: 20,000+ pour précision maximale

4. **Comprenez les Pot Odds**: C'est la clé du poker mathématique
5. **Long terme**: Une décision +EV gagnera de l'argent sur 1000+ mains

## 🎓 Ressources Supplémentaires

- **Théorie**: "The Mathematics of Poker" par Bill Chen
- **Stratégie**: "Harrington on Hold'em" par Dan Harrington
- **GTO**: "Applications of No-Limit Hold'em" par Matthew Janda

---

**Bon jeu! 🎲**
