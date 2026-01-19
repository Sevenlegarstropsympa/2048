#ifndef MECANICS_H
#define MECANICS_H

#include <vector>

class Grid2048 {
private:
    int grid[4][4];
    
    bool processLine(int line[4], bool reverse);
    void spawnNewTile();

public:
    Grid2048();
    
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    
    int get(int row, int col) const;
    bool isGameOver() const;
};

#endif