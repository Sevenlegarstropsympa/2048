#ifndef GRID_H
#define GRID_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

class Grid {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    int gridSize;
    int cellSize;
    int margin;
    int startX, startY;
    
    struct Colors {
        SDL_Color background = {250, 248, 239, 255};
        SDL_Color gridBackground = {187, 173, 160, 255};
        SDL_Color emptyCell = {205, 193, 180, 255};
    } colors;
    
public:
    Grid(SDL_Renderer* renderer, TTF_Font* font);
    ~Grid();
    
    void setPosition(int windowWidth, int windowHeight);
    void drawEmptyGrid();
    void drawTile(int gridX, int gridY, int value);
    void drawAllTiles(const std::vector<std::vector<int>>& grid);
    
    static SDL_Color getTileColor(int value);
    static SDL_Color getTextColor(int value);
    
private:
    void drawText(const std::string& text, float x, float y, int size, SDL_Color color);
};

#endif