#include "../include/Grid.h"
#include <iostream>
#include <string>

// Constructeur : initialise les variables membres
// renderer et font viennent de Game.cpp
// gridSize = 4 (grille 4x4), cellSize = 100px, margin = 10px entre les cases
Grid::Grid(SDL_Renderer* renderer, TTF_Font* font) 
    : renderer(renderer), font(font), gridSize(4), cellSize(100), margin(10) {
    std::cout << "Grid créé" << std::endl;  // Message debug
}

// Destructeur : vide car pas d'allocation mémoire dans Grid
// Le renderer et font sont gérés par Game
Grid::~Grid() {
}

// Calcule la position de la grille pour la centrer dans la fenêtre
void Grid::setPosition(int windowWidth, int windowHeight) {
    // Largeur totale de la grille = (4 cases * 100px) + (3 marges * 10px)
    int gridWidth = gridSize * cellSize + (gridSize - 1) * margin;
    int gridHeight = gridSize * cellSize + (gridSize - 1) * margin;
    
    // Centre horizontalement, décalé de 100px vers le bas pour laisser place au titre
    startX = (windowWidth - gridWidth) / 2;
    startY = (windowHeight - gridHeight) / 2 + 100;
}

// Dessine la grille vide (les 16 cases sans tuiles)
void Grid::drawEmptyGrid() {
    // Rectangle de fond de la grille (avec marges extérieures)
    SDL_FRect background = {
        static_cast<float>(startX - margin),  // x avec marge gauche
        static_cast<float>(startY - margin),  // y avec marge haute
        static_cast<float>(gridSize * cellSize + (gridSize + 1) * margin),  // largeur totale
        static_cast<float>(gridSize * cellSize + (gridSize + 1) * margin)   // hauteur totale
    };
    
    // Couleur de fond : brun clair
    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderFillRect(renderer, &background);
    
    // Dessine chaque case vide
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            // Rectangle d'une case
            SDL_FRect cell = {
                static_cast<float>(startX + x * (cellSize + margin)),  // x de la case
                static_cast<float>(startY + y * (cellSize + margin)),  // y de la case
                static_cast<float>(cellSize),                         // largeur case
                static_cast<float>(cellSize)                          // hauteur case
            };
            
            // Couleur case vide : beige
            SDL_SetRenderDrawColor(renderer, 205, 193, 180, 255);
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

// Dessine une tuile avec sa valeur
void Grid::drawTile(int gridX, int gridY, int value) {
    if (value == 0) return;  // Case vide, on ne dessine rien
    
    // Position de la tuile dans la fenêtre (en pixels)
    float x = startX + gridX * (cellSize + margin);
    float y = startY + gridY * (cellSize + margin);
    
    // Rectangle de la tuile (légèrement plus petit que la case avec 3px de marge)
    SDL_FRect tileRect = {
        x + 3.0f, y + 3.0f,                    // position avec marge interne
        static_cast<float>(cellSize - 6),      // largeur moins marges
        static_cast<float>(cellSize - 6)       // hauteur moins marges
    };
    
    // Obtient la couleur selon la valeur (2, 4, 8, etc.)
    SDL_Color tileColor = getTileColor(value);
    // Applique la couleur et dessine le rectangle
    SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);
    SDL_RenderFillRect(renderer, &tileRect);
    
    // Convertit la valeur en texte
    std::string text = std::to_string(value);
    // Obtient la couleur du texte (noir pour petites valeurs, blanc pour grandes)
    SDL_Color textColor = getTextColor(value);
    
    // Taille de police adaptée à la valeur
    int fontSize = 36;            // valeurs 2-9
    if (value >= 1000) fontSize = 24;    // 4 chiffres -> police plus petite
    else if (value >= 100) fontSize = 30;  // 3 chiffres
    else if (value >= 10) fontSize = 36;   // 2 chiffres
    else fontSize = 42;                    // 1 chiffre -> police plus grande
    
    // Dessine le texte au centre de la tuile
    drawText(text, x + cellSize/2.0f, y + cellSize/2.0f, fontSize, textColor);
}

// Dessine toutes les tuiles de la grille
void Grid::drawAllTiles(const std::vector<std::vector<int>>& grid) {
    // Compte les tuiles pour debug
    int tileCount = 0;
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            if (grid[y][x] != 0) {
                tileCount++;
            }
        }
    }
    
    // Messages debug
    if (tileCount == 0) {
        std::cout << "DEBUG Grid: Grille vide reçue" << std::endl;
    } else {
        std::cout << "DEBUG Grid: " << tileCount << " tuiles à afficher" << std::endl;
    }
    
    // Dessine chaque tuile
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            drawTile(x, y, grid[y][x]);
        }
    }
}

// Retourne la couleur d'une tuile selon sa valeur
SDL_Color Grid::getTileColor(int value) {
    // Tableau de couleurs pour chaque puissance de 2
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
    
    // Calcule l'index dans le tableau
    int index = 0;
    int temp = value;
    while (temp > 2 && index < 10) {
        temp /= 2;      // Divise par 2 jusqu'à obtenir 2
        index++;        // Incrémente l'index
    }
    
    // Retourne la couleur si dans le tableau
    if (index < 11) return colors[index];
    return {60, 58, 50, 255};  // Noir pour valeurs > 2048
}

// Retourne la couleur du texte selon la valeur de la tuile
SDL_Color Grid::getTextColor(int value) {
    // Texte noir pour petites valeurs (2, 4), blanc pour grandes valeurs
    return (value <= 4) ? SDL_Color{119, 110, 101, 255} : SDL_Color{249, 246, 242, 255};
}

// Dessine du texte à l'écran
void Grid::drawText(const std::string& text, float x, float y, int size, SDL_Color color) {
    if (!font) {
        std::cout << "DEBUG Grid: Pas de police chargée" << std::endl;
        return;
    }
    
    // Charge la police avec la bonne taille
    TTF_Font* sizedFont = TTF_OpenFont("assets/fonts/arial.ttf", size);
    if (!sizedFont) {
        sizedFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", size);
        if (!sizedFont) {
            std::cout << "DEBUG Grid: Impossible de charger police taille " << size << std::endl;
            return;
        }
    }
    
    // Crée une surface SDL avec le texte
    SDL_Surface* surface = TTF_RenderText_Solid(sizedFont, text.c_str(), text.length(), color);
    if (!surface) {
        TTF_CloseFont(sizedFont);
        std::cout << "DEBUG Grid: Échec création surface texte" << std::endl;
        return;
    }
    
    // Convertit la surface en texture (plus rapide à dessiner)
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        TTF_CloseFont(sizedFont);
        std::cout << "DEBUG Grid: Échec création texture texte" << std::endl;
        return;
    }
    
    // Récupère les dimensions de la texture
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    // Positionne la texture centrée sur (x, y)
    SDL_FRect dest = {x - w/2.0f, y - h/2.0f, w, h};
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    
    // Nettoie la mémoire
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(sizedFont);
}