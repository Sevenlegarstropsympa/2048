#include "../include/mecanics.h"
#include <cstdlib>   // Pour rand() et srand()
#include <ctime>     // Pour time()
#include <iostream>  // Pour std::cout (debug)

// Traite une ligne de 4 cases (ligne ou colonne)
// reverse = true pour déplacement vers la droite/bas
bool Grid2048::processLine(int line[4], bool reverse) {
    // Si reverse, on inverse la ligne pour traiter comme vers la gauche
    if (reverse) {
        // Échange les éléments pour inverser la ligne
        for (int i = 0; i < 2; i++) {
            int temp = line[i];
            line[i] = line[3 - i];   // Premier avec dernier
            line[3 - i] = temp;      // Deuxième avec avant-dernier
        }
    }
    
    bool moved = false;   // Indique si quelque chose a bougé
    int writeIndex = 0;   // Position où écrire la prochaine tuile
    
    // ÉTAPE 1 : COMPACTER vers la gauche
    // Déplace toutes les tuiles non nulles vers la gauche
    for (int i = 0; i < 4; i++) {
        if (line[i] != 0) {  // Si case non vide
            if (i != writeIndex) {  // Si pas déjà à la bonne position
                line[writeIndex] = line[i];  // Déplace la tuile
                line[i] = 0;                 // Vide l'ancienne position
                moved = true;                // Un mouvement a eu lieu
            }
            writeIndex++;  // Avance la position d'écriture
        }
    }
    
    // ÉTAPE 2 : FUSIONNER les cases adjacentes identiques
    // Parcours les 3 premières cases (i+1 existe pour i=3)
    for (int i = 0; i < 3; i++) {
        if (line[i] != 0 && line[i] == line[i + 1]) {
            // Fusion : double la valeur
            line[i] *= 2;
            // Vide la case fusionnée
            line[i + 1] = 0;
            moved = true;  // Une fusion est un mouvement
            
            // Décaler les cases restantes vers la gauche
            // Pour combler le trou créé par la fusion
            for (int j = i + 1; j < 3; j++) {
                line[j] = line[j + 1];  // Déplace chaque case d'un cran
            }
            line[3] = 0;  // Dernière case devient vide
        }
    }
    
    // Si on avait inversé au début, on ré-inverse pour remettre dans l'ordre
    if (reverse) {
        for (int i = 0; i < 2; i++) {
            int temp = line[i];
            line[i] = line[3 - i];
            line[3 - i] = temp;
        }
    }
    
    return moved;  // Retourne true si au moins un déplacement/fusion
}

// Ajoute une nouvelle tuile aléatoire dans une case vide
void Grid2048::spawnNewTile() {
    int emptyCount = 0;          // Nombre de cases vides
    int emptyPos[16][2];         // Tableau des positions vides
    
    // Parcours toute la grille pour trouver les cases vides
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) {  // Case vide
                // Stocke la position (ligne, colonne)
                emptyPos[emptyCount][0] = r;
                emptyPos[emptyCount][1] = c;
                emptyCount++;  // Incrémente le compteur
            }
        }
    }
    
    // Si aucune case vide, on ne peut pas ajouter de tuile
    if (emptyCount == 0) return;
    
    // Choisit une position vide aléatoire
    int chosen = rand() % emptyCount;  // Nombre entre 0 et emptyCount-1
    int r = emptyPos[chosen][0];      // Ligne choisie
    int c = emptyPos[chosen][1];      // Colonne choisie
    
    // 90% de chance d'un 2, 10% de chance d'un 4
    grid[r][c] = (rand() % 10 < 9) ? 2 : 4;
}

// Constructeur : initialise la grille et ajoute 2 tuiles
Grid2048::Grid2048() {
    // Initialise le générateur aléatoire avec l'heure actuelle
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Initialise toutes les cases à 0 (vide)
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            grid[r][c] = 0;
        }
    }
    
    // Ajoute 2 tuiles pour démarrer le jeu
    spawnNewTile();
    spawnNewTile();
}

// Déplacement vers la GAUCHE
bool Grid2048::moveLeft() {
    bool moved = false;  // Aucun mouvement au départ
    
    // Traite chaque ligne (4 lignes)
    for (int r = 0; r < 4; r++) {
        // Crée un tableau avec la ligne actuelle
        int line[4] = {grid[r][0], grid[r][1], grid[r][2], grid[r][3]};
        
        // Traite la ligne (reverse = false pour gauche)
        if (processLine(line, false)) {
            moved = true;  // La ligne a changé
            
            // Met à jour la grille avec la nouvelle ligne
            grid[r][0] = line[0];
            grid[r][1] = line[1];
            grid[r][2] = line[2];
            grid[r][3] = line[3];
        }
    }
    
    // Si au moins une tuile a bougé, ajoute une nouvelle tuile
    if (moved) {
        spawnNewTile();
    }
    
    return moved;  // Retourne si le mouvement a eu un effet
}

// Déplacement vers la DROITE
bool Grid2048::moveRight() {
    bool moved = false;
    
    for (int r = 0; r < 4; r++) {
        int line[4] = {grid[r][0], grid[r][1], grid[r][2], grid[r][3]};
        if (processLine(line, true)) {  // reverse = true pour droite
            moved = true;
            grid[r][0] = line[0];
            grid[r][1] = line[1];
            grid[r][2] = line[2];
            grid[r][3] = line[3];
        }
    }
    
    if (moved) {
        spawnNewTile();
    }
    
    return moved;
}

// Déplacement vers le HAUT
bool Grid2048::moveUp() {
    bool moved = false;
    
    // Traite chaque colonne (4 colonnes)
    for (int c = 0; c < 4; c++) {
        // Crée un tableau avec la colonne actuelle
        int line[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        
        if (processLine(line, false)) {  // reverse = false pour haut
            moved = true;
            // Met à jour la colonne dans la grille
            grid[0][c] = line[0];
            grid[1][c] = line[1];
            grid[2][c] = line[2];
            grid[3][c] = line[3];
        }
    }
    
    if (moved) {
        spawnNewTile();
    }
    
    return moved;
}

// Déplacement vers le BAS
bool Grid2048::moveDown() {
    bool moved = false;
    
    for (int c = 0; c < 4; c++) {
        int line[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        if (processLine(line, true)) {  // reverse = true pour bas
            moved = true;
            grid[0][c] = line[0];
            grid[1][c] = line[1];
            grid[2][c] = line[2];
            grid[3][c] = line[3];
        }
    }
    
    if (moved) {
        spawnNewTile();
    }
    
    return moved;
}

// Retourne la valeur d'une case (getter)
int Grid2048::get(int row, int col) const {
    return grid[row][col];  // Retourne la valeur
}

// Vérifie si le jeu est terminé (plus de mouvements possibles)
bool Grid2048::isGameOver() const {
    // ÉTAPE 1 : Vérifier s'il reste des cases vides
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) {
                return false;  // Case vide = jeu pas terminé
            }
        }
    }
    
    // ÉTAPE 2 : Vérifier s'il reste des fusions possibles
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            // Vérifier si fusion possible avec la case de DROITE
            if (c < 3 && grid[r][c] == grid[r][c + 1]) {
                return false;  // Fusion possible horizontalement
            }
            // Vérifier si fusion possible avec la case du BAS
            if (r < 3 && grid[r][c] == grid[r + 1][c]) {
                return false;  // Fusion possible verticalement
            }
        }
    }
    
    // Aucune case vide ET aucune fusion possible = GAME OVER
    return true;
}