#include "../include/Grid.h"
#include <iostream>
#include <string>

Grid::Grid(SDL_Renderer* renderer, TTF_Font* font) 
    : renderer(renderer), font(font), gridSize(4), cellSize(100), margin(10) {
}

Grid::~Grid() {
}

void Grid::setPosition(int windowWidth, int windowHeight) {
    int gridWidth = gridSize * cellSize + (gridSize - 1) * margin;
    int gridHeight = gridSize * cellSize + (gridSize - 1) * margin;
    
    startX = (windowWidth - gridWidth) / 2;
    startY = (windowHeight - gridHeight) / 2 + 100;
}

void Grid::drawEmptyGrid() {
    SDL_FRect background = {
        static_cast<float>(startX - margin),
        static_cast<float>(startY - margin),
        static_cast<float>(gridSize * cellSize + (gridSize + 1) * margin),
        static_cast<float>(gridSize * cellSize + (gridSize + 1) * margin)
    };
    
    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderFillRect(renderer, &background);
    
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            SDL_FRect cell = {
                static_cast<float>(startX + x * (cellSize + margin)),
                static_cast<float>(startY + y * (cellSize + margin)),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };
            
            SDL_SetRenderDrawColor(renderer, 205, 193, 180, 255);
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void Grid::drawTile(int gridX, int gridY, int value) {
    if (value == 0) return;
    
    float x = startX + gridX * (cellSize + margin);
    float y = startY + gridY * (cellSize + margin);
    
    SDL_FRect tileRect = {
        x + 3.0f, y + 3.0f, 
        static_cast<float>(cellSize - 6), 
        static_cast<float>(cellSize - 6)
    };
    
    SDL_Color tileColor = Grid::getTileColor(value);
    SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);
    SDL_RenderFillRect(renderer, &tileRect);
    
    std::string text = std::to_string(value);
    SDL_Color textColor = Grid::getTextColor(value);
    
    int fontSize = 36;
    if (value >= 1000) fontSize = 24;
    else if (value >= 100) fontSize = 30;
    else if (value >= 10) fontSize = 36;
    else fontSize = 42;
    
    drawText(text, x + cellSize/2.0f, y + cellSize/2.0f, fontSize, textColor);
}

void Grid::drawAllTiles(const std::vector<std::vector<int>>& grid) {
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            drawTile(x, y, grid[y][x]);
        }
    }
}

SDL_Color Grid::getTileColor(int value) {
    static const SDL_Color colors[] = {
        {238, 228, 218, 255},  // 2
        {237, 224, 200, 255},  // 4
        {242, 177, 121, 255},  // 8
        {245, 149, 99, 255},   // 16
        {246, 124, 95, 255},   // 32
        {246, 94, 59, 255},    // 64
        {237, 207, 114, 255},  // 128
        {237, 204, 97, 255},   // 256
        {237, 200, 80, 255},   // 512
        {237, 197, 63, 255},   // 1024
        {237, 194, 46, 255}    // 2048
    };
    
    int index = 0;
    int temp = value;
    while (temp > 2 && index < 10) {
        temp /= 2;
        index++;
    }
    
    if (index < 11) return colors[index];
    return {60, 58, 50, 255};
}

SDL_Color Grid::getTextColor(int value) {
    return (value <= 4) ? SDL_Color{119, 110, 101, 255} : SDL_Color{249, 246, 242, 255};
}

void Grid::drawText(const std::string& text, float x, float y, int size, SDL_Color color) {
    if (!font) return;
    
    TTF_Font* sizedFont = TTF_OpenFont("assets/fonts/arial.ttf", size);
    if (!sizedFont) {
        sizedFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", size);
        if (!sizedFont) return;
    }
    
    SDL_Surface* surface = TTF_RenderText_Solid(sizedFont, text.c_str(), text.length(), color);
    if (!surface) {
        TTF_CloseFont(sizedFont);
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        TTF_CloseFont(sizedFont);
        return;
    }
    
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    SDL_FRect dest = {x - w/2.0f, y - h/2.0f, w, h};
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(sizedFont);
}