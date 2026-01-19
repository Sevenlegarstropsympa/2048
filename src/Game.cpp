#include "../include/Game.h"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : window(nullptr), renderer(nullptr), font(nullptr), 
               grid(nullptr), running(true), gameOver(false), score(0) {
    
    initSDL();
    
    // Init 4x4
    gridData.resize(4, std::vector<int>(4, 0));
    
    // Créer l'objet Grid pour l'affichage
    grid = new Grid(renderer, font);
    
    // Init jeu
    resetGame();
}

Game::~Game() {
    cleanup();
}

void Game::initSDL() {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL initialization failed");
    }
    
    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed" << std::endl;
        SDL_Quit();
        throw std::runtime_error("TTF initialization failed");
    }
    
    // Créer la fenêtre
    window = SDL_CreateWindow("2048", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Fenêtre non créée: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("Window creation failed");
    }
    
    // Créer le renderer
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer non créé: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("Renderer creation failed");
    }
    
    // Charger la police
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        // Autres chemins
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
        if (!font) {
            std::cerr << "Impossible de charger la police" << std::endl;
            //Le texte ne s'affichera pas
        }
    }
}

void Game::cleanup() {
    // Clean Grid
    if (grid) {
        delete grid;
        grid = nullptr;
    }
    
    // Clean SDL
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    //Clean Renderer
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    //Clean Window
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
        // Gestion des touches
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.key) {
                case SDLK_UP:
                case SDLK_W:
                    moveTiles(0, -1);
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    moveTiles(0, 1);
                    break;
                case SDLK_LEFT:
                case SDLK_A:
                    moveTiles(-1, 0);
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    moveTiles(1, 0);
                    break;
                case SDLK_R:  // Touche R
                    resetGame();
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_SPACE:
                    addRandomTile(); 
                    break;
            }
        }
    }
}

void Game::update() {
    if (!canMove()) {
        gameOver = true;
    }
}

void Game::render() {
    // bg
    SDL_SetRenderDrawColor(renderer, 250, 248, 239, 255);
    SDL_RenderClear(renderer);
    
    // MAJ position de la grille
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    if (grid) {
        grid->setPosition(width, height);
        grid->drawEmptyGrid();
        grid->drawAllTiles(gridData);
    }
    
    // Dessine l'UI
    drawUI();
    
    SDL_RenderPresent(renderer);
}

void Game::drawUI() {
    // Logo
    drawText("2048", 400, 60, 72, SDL_Color{119, 110, 101, 255});
    
    // Score
    std::string scoreText = "SCORE\n" + std::to_string(score);
    drawText(scoreText, 550, 100, 24, SDL_Color{119, 110, 101, 255});
    
    // Best Score
    drawText("BEST\n2048", 750, 100, 24, SDL_Color{119, 110, 101, 255});
    
    // Guide
    drawText("Use arrow keys to move", 400, 550, 20, SDL_Color{150, 150, 150, 255});
    drawText("Press R to restart, ESC to quit", 400, 580, 20, SDL_Color{150, 150, 150, 255});
    
    // Game Over
    if (gameOver) {
        drawText("GAME OVER!", 400, 300, 48, SDL_Color{255, 100, 100, 255});
        drawText("Press R to restart", 400, 350, 24, SDL_Color{150, 150, 150, 255});
    }
}

void Game::drawText(const std::string& text, int x, int y, int size, SDL_Color color) {
    if (!font) return;
    
    TTF_Font* sizedFont = TTF_OpenFont("assets/fonts/arial.ttf", size);
    if (!sizedFont) {
        sizedFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", size);
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
    
    SDL_FRect dest = {static_cast<float>(x - w/2), static_cast<float>(y - h/2), w, h};
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(sizedFont);
}

void Game::resetGame() {
    // Reset la grille
    for (auto& row : gridData) {
        std::fill(row.begin(), row.end(), 0);
    }
    
    score = 0;
    gameOver = false;
    
    //tuiles initiales
    addRandomTile();
    addRandomTile();
    
    // Pour TESTER l'affichage, décommentez ces lignes :
    /*
    gridData[0][0] = 2;
    gridData[0][1] = 4;
    gridData[0][2] = 8;
    gridData[0][3] = 16;
    gridData[1][0] = 32;
    gridData[1][1] = 64;
    gridData[1][2] = 128;
    gridData[1][3] = 256;
    gridData[2][0] = 512;
    gridData[2][1] = 1024;
    gridData[2][2] = 2048;
    gridData[3][3] = 2;
    */
}

void Game::addRandomTile() {
    std::vector<std::pair<int, int>> emptyCells;
    
    // Trouver toutes les cases vides
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (gridData[y][x] == 0) {
                emptyCells.push_back({x, y});
            }
        }
    }
    
    if (emptyCells.empty()) return;
    
    // ChooseFreeTile
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, emptyCells.size() - 1);
    auto [x, y] = emptyCells[dist(gen)];
    
    //70% de 2, 30% de 4
    std::uniform_int_distribution<> valueDist(0, 9);
    gridData[y][x] = (valueDist(gen) < 7) ? 2 : 4;
}

bool Game::moveTiles(int dx, int dy) {
    bool moved = false;
    
    if (dx != 0 || dy != 0) {
        addRandomTile();
        moved = true;
    }
    
    return moved;
}

bool Game::canMove() const {
    // Check cases vides
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (gridData[y][x] == 0) {
                return true;
            }
        }
    }
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int current = gridData[y][x];
            
            // CheckDroit
            if (x < 3 && gridData[y][x+1] == current) return true;
            // CheckBas
            if (y < 3 && gridData[y+1][x] == current) return true;
        }
    }
    
    return false;
}