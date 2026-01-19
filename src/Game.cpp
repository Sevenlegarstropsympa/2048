#include "../include/Game.h"
#include <iostream>
#include <random>
#include <algorithm>

// Constructeur : initialise toutes les variables membres
// nullptr pour les pointeurs, true pour running, false pour gameOver, 0 pour score
Game::Game() : window(nullptr), renderer(nullptr), font(nullptr),
               gridDisplay(nullptr), running(true), gameOver(false), score(0) {
    
    initSDL();  // Initialise SDL2 et crée fenêtre/renderer
    
    gridDisplay = new Grid(renderer, font);  // Crée l'objet qui affiche la grille
    
    resetGame();  // Initialise le jeu (crée nouvelle grille avec 2 tuiles)
    
    render();  // Affiche immédiatement l'écran de jeu
}

// Destructeur : libère toute la mémoire allouée
Game::~Game() {
    cleanup();
}

// Initialise SDL2 : crée fenêtre, renderer, charge polices
void Game::initSDL() {
    // 1. Initialise SDL pour la vidéo
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init échoué" << std::endl;
        throw std::runtime_error("SDL initialization failed");
    }
    
    // 2. Initialise SDL_ttf pour les textes
    if (!TTF_Init()) {
        std::cerr << "TTF_Init échoué" << std::endl;
        SDL_Quit();  // Nettoie SDL si TTF échoue
        throw std::runtime_error("TTF initialization failed");
    }
    
    // 3. Crée la fenêtre 800x600 redimensionnable
    window = SDL_CreateWindow("2048", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Fenêtre non créée" << std::endl;
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("Window creation failed");
    }
    
    // 4. Crée le renderer pour dessiner dans la fenêtre
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer non créé" << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("Renderer creation failed");
    }
    
    // 5. Charge la police Arial pour afficher texte et chiffres
    font = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);  // Essaie chemin Windows
        if (!font) {
            std::cerr << "Impossible de charger la police" << std::endl;
            // Continue sans police (texte ne s'affichera pas)
        }
    }
}

// Nettoie tout : supprime fenêtre, renderer, police, arrête SDL
void Game::cleanup() {
    if (gridDisplay) {
        delete gridDisplay;  // Supprime l'affichage grille
        gridDisplay = nullptr;
    }
    
    if (font) {
        TTF_CloseFont(font);  // Ferme la police
        font = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);  // Détruit le renderer
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);  // Détruit la fenêtre
        window = nullptr;
    }
    
    TTF_Quit();  // Arrête SDL_ttf
    SDL_Quit();  // Arrête SDL
}

// Boucle principale du jeu : tourne tant que running = true
void Game::run() {
    while (running) {
        handleEvents();  // Gère clavier/souris
        update();        // Met à jour logique jeu
        SDL_Delay(16);   // Attend ~16ms pour ~60 FPS
    }
}

// Gestion des événements (clavier)
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  // Traite tous les événements en attente
        if (event.type == SDL_EVENT_QUIT) {
            running = false;  // Quitte si croix fenêtre cliquée
        }
        else if (event.type == SDL_EVENT_KEY_DOWN) {  // Touche pressée
            bool moved = false;  // Si un mouvement a eu lieu
            
            switch (event.key.key) {
                case SDLK_UP:
                case SDLK_W:
                    moved = gameLogic.moveUp();  // Déplace vers le haut
                    break;
                    
                case SDLK_DOWN:
                case SDLK_S:
                    moved = gameLogic.moveDown();  // Déplace vers le bas
                    break;
                    
                case SDLK_LEFT:
                case SDLK_A:
                    moved = gameLogic.moveLeft();  // Déplace vers la gauche
                    break;
                    
                case SDLK_RIGHT:
                case SDLK_D:
                    moved = gameLogic.moveRight();  // Déplace vers la droite
                    break;
                    
                case 'r':
                case 'R':
                    resetGame();  // Recommence partie
                    render();     // Affiche nouvelle grille
                    return;       // Sort pour éviter double affichage
                    
                case SDLK_ESCAPE:
                    running = false;  // Quitte jeu
                    break;
                    
                case SDLK_SPACE:
                    // Mode debug : affiche grille dans console
                    std::cout << "\n=== DEBUG CONSOLE ===" << std::endl;
                    std::cout << "Grille actuelle:" << std::endl;
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            int val = gameLogic.get(r, c);
                            // Affiche "." pour 0, sinon le chiffre
                            std::cout << (val == 0 ? "." : std::to_string(val)) << "\t";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << "Score: " << score << std::endl;
                    std::cout << "Game Over: " << (gameOver ? "OUI" : "NON") << std::endl;
                    std::cout << "====================\n" << std::endl;
                    break;
            }
            
            // Si un mouvement a eu lieu
            if (moved) {
                updateScore();           // Recalcule score
                gameOver = gameLogic.isGameOver();  // Vérifie si partie finie
                render();                // Affiche nouvelle grille
                SDL_Delay(50);           // Petit délai pour pas aller trop vite
            }
        }
    }
}

// Mise à jour logique du jeu (vide car géré dans handleEvents)
void Game::update() {
    // Rien à faire ici, tout est géré quand touche pressée
}

// Fonction qui dessine tout à l'écran
void Game::render() {
    // 1. Efface écran avec couleur fond beige
    SDL_SetRenderDrawColor(renderer, 250, 248, 239, 255);
    SDL_RenderClear(renderer);
    
    // 2. Récupère taille fenêtre pour centrer grille
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    
    // 3. Dessine grille et tuiles
    if (gridDisplay) {
        gridDisplay->setPosition(width, height);            // Centre grille
        gridDisplay->drawEmptyGrid();                       // Dessine cases vides
        gridDisplay->drawAllTiles(getGridData());           // Dessine tuiles avec chiffres
    }
    
    // 4. Dessine interface (score, titre, instructions)
    drawUI();
    
    // 5. Affiche tout à l'écran
    SDL_RenderPresent(renderer);
}

// Dessine l'interface utilisateur
void Game::drawUI() {
    // Titre "2048" en haut centre
    drawText("2048", 400, 60, 72, SDL_Color{119, 110, 101, 255});
    
    // Score actuel (à droite)
    std::string scoreText = "SCORE\n" + std::to_string(score);
    drawText(scoreText, 600, 100, 24, SDL_Color{119, 110, 101, 255});
    
    // Meilleur score (pour l'instant = score actuel)
    drawText("BEST\n" + std::to_string(score), 700, 100, 24, SDL_Color{119, 110, 101, 255});
    
    // Instructions en bas
    drawText("Use arrow keys to move", 400, 550, 20, SDL_Color{150, 150, 150, 255});
    drawText("Press R to restart, ESC to quit", 400, 580, 20, SDL_Color{150, 150, 150, 255});
    drawText("SPACE: debug console", 400, 510, 16, SDL_Color{180, 180, 180, 255});
    
    // Message "GAME OVER!" si partie terminée
    if (gameOver) {
        drawText("GAME OVER!", 400, 300, 48, SDL_Color{255, 100, 100, 255});
        drawText("Press R to restart", 400, 350, 24, SDL_Color{150, 150, 150, 255});
    }
}

// Fonction pour dessiner du texte à l'écran
void Game::drawText(const std::string& text, int x, int y, int size, SDL_Color color) {
    if (!font) return;  // Si police pas chargée, on ne fait rien
    
    // Charge police avec bonne taille
    TTF_Font* sizedFont = TTF_OpenFont("assets/fonts/arial.ttf", size);
    if (!sizedFont) {
        sizedFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", size);
        if (!sizedFont) return;
    }
    
    // Crée surface texte avec couleur
    SDL_Surface* surface = TTF_RenderText_Solid(sizedFont, text.c_str(), text.length(), color);
    if (!surface) {
        TTF_CloseFont(sizedFont);
        return;
    }
    
    // Convertit surface en texture pour dessiner plus vite
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        TTF_CloseFont(sizedFont);
        return;
    }
    
    // Récupère taille texte pour centrer
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    // Positionne texte centré sur (x,y)
    SDL_FRect dest = {static_cast<float>(x - w/2), static_cast<float>(y - h/2), w, h};
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    
    // Nettoie
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(sizedFont);
}

// Récupère données grille depuis logique jeu pour affichage
std::vector<std::vector<int>> Game::getGridData() const {
    std::vector<std::vector<int>> gridData(4, std::vector<int>(4, 0));
    
    // Copie chaque case de gameLogic vers gridData
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            gridData[r][c] = gameLogic.get(r, c);
        }
    }
    
    return gridData;
}

// Recommence partie à zéro
void Game::resetGame() {
    gameLogic = Grid2048();  // Crée nouvelle grille (avec 2 tuiles aléatoires)
    score = 0;               // Remet score à 0
    gameOver = false;        // Plus game over
    std::cout << "Nouvelle partie!" << std::endl;  // Message console
}

// Ancienne méthode de déplacement (plus utilisée)
bool Game::moveTiles(int dx, int dy) {
    return false;
}

// Calcule score : somme de toutes les tuiles
void Game::updateScore() {
    score = 0;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int value = gameLogic.get(r, c);
            score += value;  // Ajoute valeur tuile au score
        }
    }
}