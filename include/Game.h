#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "Grid.h"  // INCLURE Grid.h ici, pas de forward declaration

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Grid* grid;
    
    bool running;
    bool gameOver;
    int score;
    
    std::vector<std::vector<int>> gridData;
    
public:
    Game();
    ~Game();
    
    void run();
    void handleEvents();
    void update();
    void render();
    
    void resetGame();
    void addRandomTile();
    bool moveTiles(int dx, int dy);
    bool canMove() const;
    
    void drawUI();
    void drawText(const std::string& text, int x, int y, int size, SDL_Color color);
    
    const std::vector<std::vector<int>>& getGrid() const { return gridData; }
    
private:
    void initSDL();
    void cleanup();
};

#endif