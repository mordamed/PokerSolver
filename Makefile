# Compilateur
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

# Nom de l'exécutable
TARGET = PokerSolver

# Fichiers source
SOURCES = Card.cpp Deck.cpp HandEvaluator.cpp Player.cpp PotManager.cpp PokerGame.cpp PokerSolver.cpp main.cpp

# Fichiers objets (générés automatiquement)
OBJECTS = $(SOURCES:.cpp=.o)

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilation réussie! Lancez avec: ./$(TARGET)"

# Compilation des fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Fichiers nettoyés"

# Recompilation complète
rebuild: clean all

# Exécuter le programme
run: $(TARGET)
	./$(TARGET)

# Aide
help:
	@echo "Makefile pour Poker Solver"
	@echo "Commandes disponibles:"
	@echo "  make         - Compile le projet"
	@echo "  make clean   - Supprime les fichiers compilés"
	@echo "  make rebuild - Recompile tout depuis zéro"
	@echo "  make run     - Compile et exécute le programme"
	@echo "  make help    - Affiche cette aide"

.PHONY: all clean rebuild run help
