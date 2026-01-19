#include "../include/mecanics.hpp"
#include <cstdlib>
#include <ctime>

bool Grid2048::processLine(int line[4], bool reverse) {
    bool moved = false;
    
    if (reverse) {
        // Inverse le tableau en place
        int temp0 = line[0], temp1 = line[1];
        line[0] = line[3];
        line[1] = line[2];
        line[2] = temp1;
        line[3] = temp0;
        
        // Applique l'algo normal
        moved = processLine(line, false);
        
        // Re-inverse
        temp0 = line[0]; temp1 = line[1];
        line[0] = line[3];
        line[1] = line[2];
        line[2] = temp1;
        line[3] = temp0;
        
        return moved;
    }
    
    int next = 0;
    bool hasJustMerged = false;
    
    for (int i = 0; i < 4; i++) {
        if (line[i] == 0) continue;
        
        if (line[next] == 0) {
            line[next] = line[i];
            if (i != next) {
                line[i] = 0;
                moved = true;
            }
            hasJustMerged = false;
            next++;
        }
        else if (line[next] == line[i] && !hasJustMerged) {
            line[next] *= 2;
            line[i] = 0;
            hasJustMerged = true;
            moved = true;
            next++;
        }
        else {
            next++;
            if (next < 4) {  // SÉCURITÉ : évite overflow
                line[next] = line[i];
                if (i != next) {
                    line[i] = 0;
                    moved = true;
                }
                hasJustMerged = false;
                next++;
            }
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
        // Réécrit toujours la colonne
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
        // Réécrit toujours la colonne
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