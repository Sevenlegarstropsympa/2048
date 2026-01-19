#include "../include/mecanics.hpp"
#include <cstdlib>
#include <ctime>

bool Grid2048::processLine(int line[4], bool reverse) {
    if (reverse) {
        // Inverse le tableau en place
        int temp0 = line[0], temp1 = line[1];
        line[0] = line[3];
        line[1] = line[2];
        line[2] = temp1;
        line[3] = temp0;
        
        // Applique l'algo normal
        bool moved = processLine(line, false);
        
        // Re-inverse
        temp0 = line[0]; temp1 = line[1];
        line[0] = line[3];
        line[1] = line[2];
        line[2] = temp1;
        line[3] = temp0;
        
        return moved;
    }
    
    // Compacte et fusionne en un seul passage
    int writePos = 0;
    bool moved = false;
    int lastMerged = -1;  // Position de la dernière fusion
    
    for (int i = 0; i < 4; i++) {
        if (line[i] == 0) continue;
        
        // Si la position d'écriture est vide, on place directement
        if (writePos == 0 || line[writePos - 1] == 0) {
            if (i != writePos) {
                line[writePos] = line[i];
                line[i] = 0;
                moved = true;
            }
            writePos++;
        }
        // Si on peut fusionner avec la case précédente
        else if (line[writePos - 1] == line[i] && lastMerged != writePos - 1) {
            line[writePos - 1] *= 2;
            line[i] = 0;
            lastMerged = writePos - 1;
            moved = true;
        }
        // Sinon on place à la position suivante
        else {
            if (i != writePos) {
                line[writePos] = line[i];
                line[i] = 0;
                moved = true;
            }
            writePos++;
        }
    }
    
    return moved;
}

void Grid2048::spawnNewTile() {
    int emptyCount = 0;
    int emptyPos[16][2];
    
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) {
                emptyPos[emptyCount][0] = r;
                emptyPos[emptyCount][1] = c;
                emptyCount++;
            }
        }
    }
    
    if (emptyCount == 0) return;
    
    int chosen = rand() % emptyCount;
    int r = emptyPos[chosen][0];
    int c = emptyPos[chosen][1];
    
    grid[r][c] = (rand() % 10 < 7) ? 2 : 4;
}

Grid2048::Grid2048() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            grid[r][c] = 0;
        }
    }
    
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
        int col[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        if (processLine(col, false)) {
            moved = true;
        }
        grid[0][c] = col[0]; 
        grid[1][c] = col[1];
        grid[2][c] = col[2]; 
        grid[3][c] = col[3];
    }
    if (moved) spawnNewTile();
    return moved;
}

bool Grid2048::moveDown() {
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        int col[4] = {grid[0][c], grid[1][c], grid[2][c], grid[3][c]};
        if (processLine(col, true)) {
            moved = true;
        }
        grid[0][c] = col[0]; 
        grid[1][c] = col[1];
        grid[2][c] = col[2]; 
        grid[3][c] = col[3];
    }
    if (moved) spawnNewTile();
    return moved;
}

int Grid2048::get(int row, int col) const {
    return grid[row][col];
}

bool Grid2048::isGameOver() const {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid[r][c] == 0) return false;
        }
    }
    
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (c < 3 && grid[r][c] == grid[r][c+1]) return false;
            if (r < 3 && grid[r][c] == grid[r+1][c]) return false;
        }
    }
    
    return true;
}