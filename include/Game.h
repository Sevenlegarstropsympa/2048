#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "Grid.h"
#include "mecanics.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Grid* gridDisplay;
    Grid2048 gameLogic;
    
    bool running;
    bool gameOver;
    int score;
    
public:
    Game();
    ~Game();
    
    void run();
    void handleEvents();
    void update();
    void render();
    
    void resetGame();
    bool moveTiles(int dx, int dy);
    
    void drawUI();
    void drawText(const std::string& text, int x, int y, int size, SDL_Color color);
    
    std::vector<std::vector<int>> getGridData() const;
    
private:
    void initSDL();
    void cleanup();
    void updateScore();
    void debugPrintGrid() const;
    void forceRenderAfterMove();
};

#endif