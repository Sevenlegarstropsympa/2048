CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Fichiers sources
SRC_DIR = src
TEST_DIR = tests
CATCH_DIR = include/catch2

# Fichiers objets
SRCS = $(SRC_DIR)/mecanics.cpp
TEST_SRCS = $(TEST_DIR)/test_mecanics.cpp
CATCH_SRC = $(CATCH_DIR)/catch_amalgamated.cpp

# Exécutable de test
TEST_EXEC = test_runner

# Règle principale
all: $(TEST_EXEC)

# Compilation de l'exécutable de test
$(TEST_EXEC): $(SRCS) $(TEST_SRCS) $(CATCH_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Exécuter les tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Exécuter avec verbose
test-verbose: $(TEST_EXEC)
	./$(TEST_EXEC) -s

# Exécuter seulement certains tests
test-moves: $(TEST_EXEC)
	./$(TEST_EXEC) "[moves]"

test-gameover: $(TEST_EXEC)
	./$(TEST_EXEC) "[gameOver]"

# Nettoyage
clean:
	rm -f $(TEST_EXEC)

.PHONY: all test test-verbose test-moves test-gameover clean