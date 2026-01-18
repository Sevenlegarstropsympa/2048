#include "../include/mecanics.hpp"
#include <cstdlib>
#include <ctime>

bool Grid2048::processLine(int line[4], bool reverse) {
    bool moved = false;
    
    if (reverse) {
        // Inverse temporairement pour réutiliser la même logique
        int temp[4] = {line[3], line[2], line[1], line[0]};
        moved = processLine(temp, false);
        line[0] = temp[3]; line[1] = temp[2]; 
        line[2] = temp[1]; line[3] = temp[0];
        return moved;
    }
    
    int next = 0;
    bool hasJustMerged = false;
    
    for (int i = 0; i < 4; i++) {
        if (line[i] == 0) continue;  // Skip cases vides
        
        if (line[next] == 0) {  // Cas A1 : next est vide
            line[next] = line[i];
            if (i != next) {
                line[i] = 0;
                moved = true;
            }
            hasJustMerged = false;
            next++;
        }
        else if (line[next] == line[i] && !hasJustMerged) {  // Cas B : fusion
            line[next] *= 2;
            line[i] = 0;
            hasJustMerged = true;
            moved = true;
            next++;
        }
        else {  // Cas A2 : next occupé avec valeur différente
            next++;
            line[next] = line[i];
            if (i != next) {
                line[i] = 0;
                moved = true;
            }
            hasJustMerged = false;
            next++;
        }
    }
    
    return moved;
}

void Grid2048::spawnNewTile() {
    // Trouve toutes les cases vides
    int emptyCount = 0;
    int emptyPos[16][2];  // Max 16 cases
    
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) {
                emptyPos[emptyCount][0] = r;
                emptyPos[emptyCount][1] = c;
                emptyCount++;
            }
        }
    }
    
    if (emptyCount == 0) return;  // Grille pleine
    
    // Choisit une position aléatoire parmi les vides
    int chosen = rand() % emptyCount;
    int r = emptyPos[chosen][0];
    int c = emptyPos[chosen][1];
    
    // 70% de chance pour 2, 30% pour 4
    grid[r][c] = (rand() % 10 < 7) ? 2 : 4;
}

Grid2048::Grid2048() {
    // Init seed random
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Init grille vide
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            grid[r][c] = 0;
        }
    }
    
    // Spawn 2 tuiles de départ
    spawnNewTile();
    spawnNewTile();
}

bool Grid2048::moveLeft() {
    bool moved = false;
    for (int r = 0; r < 4; r++) {
        if (processLine(grid[r], false)) {
            moved = true;
        }
    }
    if (moved) spawnNewTile();
    return moved;
}

bool Grid2048::moveRight() {
    bool moved = false;
    for (int r = 0; r < 4; r++) {
        if (processLine(grid[r], true)) {
            moved = true;
        }
    }
    if (moved) spawnNewTile();
    return moved;
}

bool Grid2048::moveUp() {
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        // Extrait la colonne
        int col[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        if (processLine(col, false)) {
            moved = true;
            // Réécrit la colonne
            grid[0][c] = col[0]; grid[1][c] = col[1];
            grid[2][c] = col[2]; grid[3][c] = col[3];
        }
    }
    if (moved) spawnNewTile();
    return moved;
}

bool Grid2048::moveDown() {
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        // Extrait la colonne
        int col[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        if (processLine(col, true)) {
            moved = true;
            // Réécrit la colonne
            grid[0][c] = col[0]; grid[1][c] = col[1];
            grid[2][c] = col[2]; grid[3][c] = col[3];
        }
    }
    if (moved) spawnNewTile();
    return moved;
}

int Grid2048::get(int row, int col) const {
    return grid[row][col];
}

bool Grid2048::isGameOver() const {
    // Si une case vide existe, jeu pas fini
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) return false;
        }
    }
    
    // Vérifie si une fusion est possible (horizontalement ou verticalement)
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (c < 3 && grid[r][c] == grid[r][c+1]) return false;
            if (r < 3 && grid[r][c] == grid[r+1][c]) return false;
        }
    }
    
    return true;  // Aucun mouvement possible
}