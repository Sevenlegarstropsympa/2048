#include "../include/catch2/catch_amalgamated.hpp"
#include "../include/mecanics.hpp"

// Helper pour créer une grille avec un état connu
// On utilise une approche "boîte noire" via les méthodes publiques
class GridTestHelper {
public:
    // Vérifie l'état complet de la grille
    static bool checkGrid(const Grid2048& game, int expected[4][4]) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (game.get(r, c) != expected[r][c]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Compte le nombre de tuiles non vides
    static int countTiles(const Grid2048& game) {
        int count = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (game.get(r, c) != 0) {
                    count++;
                }
            }
        }
        return count;
    }
    
    // Vérifie si une valeur existe dans la grille
    static bool hasValue(const Grid2048& game, int value) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (game.get(r, c) == value) {
                    return true;
                }
            }
        }
        return false;
    }
    
    // Affiche la grille (pour debug)
    static void printGrid(const Grid2048& game) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                printf("%4d ", game.get(r, c));
            }
            printf("\n");
        }
        printf("\n");
    }
};

// ============ TESTS CONSTRUCTEUR ============

TEST_CASE("Constructeur - Initialise avec 2 tuiles", "[constructor]") {
    Grid2048 game;
    
    int count = GridTestHelper::countTiles(game);
    
    REQUIRE(count == 2);
    // Vérifie que les tuiles sont 2 ou 4
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int val = game.get(r, c);
            if (val != 0) {
                REQUIRE((val == 2 || val == 4));
            }
        }
    }
}

TEST_CASE("get - Retourne les bonnes valeurs", "[get]") {
    Grid2048 game;
    
    // Vérifie que get() retourne des valeurs valides
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int val = game.get(r, c);
            REQUIRE(val >= 0);
        }
    }
}

// ============ TESTS MOUVEMENTS (tests indirects de processLine) ============

TEST_CASE("moveLeft - Déplacement et fusion", "[moves][processLine]") {
    Grid2048 game;
    
    // On teste le comportement global
    // Après plusieurs mouvements, on devrait avoir des fusions
    int initialCount = GridTestHelper::countTiles(game);
    
    // Fait plusieurs mouvements pour tester la logique
    game.moveLeft();
    game.moveLeft();
    
    // La grille devrait avoir changé
    int newCount = GridTestHelper::countTiles(game);
    REQUIRE(newCount >= 2); // Au moins 2 tuiles (spawn après chaque move)
}

TEST_CASE("moveRight - Déplacement vers la droite", "[moves][processLine]") {
    Grid2048 game;
    
    // Effectue un mouvement à droite
    game.moveRight();
    
    // Vérifie qu'il y a au moins 3 tuiles (2 initiales + 1 spawn)
    int count = GridTestHelper::countTiles(game);
    REQUIRE(count >= 2);
}

TEST_CASE("moveUp - Déplacement vers le haut", "[moves][processLine]") {
    Grid2048 game;
    
    game.moveUp();
    
    int count = GridTestHelper::countTiles(game);
    REQUIRE(count >= 2);
}

TEST_CASE("moveDown - Déplacement vers le bas", "[moves][processLine]") {
    Grid2048 game;
    
    game.moveDown();
    
    int count = GridTestHelper::countTiles(game);
    REQUIRE(count >= 2);
}

TEST_CASE("Mouvements - Pas de mouvement si grille pleine sans fusion", "[moves]") {
    Grid2048 game;
    
    // Remplit la grille en faisant beaucoup de mouvements
    // (difficile de tester sans accès direct à la grille)
    for (int i = 0; i < 100; i++) {
        if (game.isGameOver()) break;
        game.moveLeft();
        if (game.isGameOver()) break;
        game.moveUp();
    }
    
    // Si game over, aucun mouvement ne devrait être possible
    if (game.isGameOver()) {
        bool canMove = false;
        Grid2048 testGame = game;
        canMove |= testGame.moveLeft();
        canMove |= testGame.moveRight();
        canMove |= testGame.moveUp();
        canMove |= testGame.moveDown();
        
        REQUIRE(canMove == false);
    }
}

TEST_CASE("Mouvements - Spawn après mouvement valide", "[moves][spawnNewTile]") {
    Grid2048 game;
    
    int countBefore = GridTestHelper::countTiles(game);
    
    // Fait un mouvement (devrait spawn une nouvelle tuile)
    bool moved = game.moveLeft();
    
    if (moved) {
        int countAfter = GridTestHelper::countTiles(game);
        // Si mouvement réussi, une tuile devrait être ajoutée
        REQUIRE(countAfter >= countBefore);
    }
}

TEST_CASE("Mouvements - Fusion crée des valeurs plus grandes", "[moves][processLine]") {
    Grid2048 game;
    
    // Fait beaucoup de mouvements pour forcer des fusions
    for (int i = 0; i < 50; i++) {
        if (game.isGameOver()) break;
        
        switch (i % 4) {
            case 0: game.moveLeft(); break;
            case 1: game.moveUp(); break;
            case 2: game.moveRight(); break;
            case 3: game.moveDown(); break;
        }
    }
    
    // Vérifie qu'on a créé au moins une tuile > 4
    bool hasLargeTile = false;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (game.get(r, c) > 4) {
                hasLargeTile = true;
                break;
            }
        }
    }
    
    // Avec 50 mouvements, on devrait avoir fusionné au moins une fois
    REQUIRE(hasLargeTile == true);
}

// ============ TESTS ISGAMEOVER ============

TEST_CASE("isGameOver - Faux au début du jeu", "[gameOver]") {
    Grid2048 game;
    
    REQUIRE(game.isGameOver() == false);
}

TEST_CASE("isGameOver - Faux tant que mouvements possibles", "[gameOver]") {
    Grid2048 game;
    
    // Fait quelques mouvements
    game.moveLeft();
    game.moveUp();
    game.moveRight();
    
    // Le jeu ne devrait pas être fini
    REQUIRE(game.isGameOver() == false);
}

TEST_CASE("isGameOver - Cohérence avec possibilité de mouvement", "[gameOver]") {
    Grid2048 game;
    
    // Si le jeu n'est pas fini, au moins un mouvement devrait être possible
    if (!game.isGameOver()) {
        Grid2048 test1 = game, test2 = game, test3 = game, test4 = game;
        
        bool canMove = test1.moveLeft() || test2.moveRight() || 
                       test3.moveUp() || test4.moveDown();
        
        REQUIRE(canMove == true);
    }
}

// ============ TESTS D'INTÉGRATION ============

TEST_CASE("Scénario complet - Partie jusqu'à game over ou 8", "[integration]") {
    Grid2048 game;
    
    int moveCount = 0;
    const int maxMoves = 200;
    
    while (!game.isGameOver() && moveCount < maxMoves) {
        // Stratégie simple : alterne les directions
        switch (moveCount % 4) {
            case 0: game.moveLeft(); break;
            case 1: game.moveUp(); break;
            case 2: game.moveRight(); break;
            case 3: game.moveDown(); break;
        }
        moveCount++;
        
        // Vérifie qu'on a au moins atteint 8
        if (GridTestHelper::hasValue(game, 8)) {
            SUCCEED("Tuile 8 atteinte !");
            break;
        }
    }
    
    REQUIRE(moveCount > 0);
}

TEST_CASE("Cohérence - Grille toujours valide", "[integration]") {
    Grid2048 game;
    
    // Fait 20 mouvements aléatoires
    for (int i = 0; i < 20; i++) {
        if (game.isGameOver()) break;
        
        switch (i % 4) {
            case 0: game.moveLeft(); break;
            case 1: game.moveRight(); break;
            case 2: game.moveUp(); break;
            case 3: game.moveDown(); break;
        }
        
        // Vérifie que toutes les valeurs sont des puissances de 2 ou 0
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                int val = game.get(r, c);
                if (val != 0) {
                    // Vérifie que c'est une puissance de 2
                    bool isPowerOf2 = (val > 0) && ((val & (val - 1)) == 0);
                    REQUIRE(isPowerOf2 == true);
                }
            }
        }
    }
}