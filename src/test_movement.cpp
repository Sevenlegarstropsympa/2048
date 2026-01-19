#include "../include/mecanics.hpp"
#include <iostream>
#include <cstdio>
#include <termios.h>
#include <unistd.h>

// Fonction pour lire une touche sans attendre Enter
char getKeyInput() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    char ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void displayGrid(const Grid2048& game) {
    system("clear");
    std::cout << "=== 2048 Test Movement ===" << std::endl;
    std::cout << "Utilisez les flèches pour bouger" << std::endl;
    std::cout << "Tapez 'q' pour quitter" << std::endl;
    std::cout << std::endl;
    
    for (int r = 0; r < 4; r++) {
        std::cout << "| ";
        for (int c = 0; c < 4; c++) {
            int val = game.get(r, c);
            if (val == 0) {
                std::cout << "    ";
            } else {
                printf("%4d", val);
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    if (game.isGameOver()) {
        std::cout << "GAME OVER!" << std::endl;
    }
}

int main() {
    Grid2048 game;
    char input;
    bool running = true;
    
    while (running) {
        displayGrid(game);
        
        input = getKeyInput();
        
        // Gère les séquences d'échappement pour les flèches
        if (input == '\033') {
            getKeyInput();  // Consomme '['
            input = getKeyInput();  // Récupère la direction
            
            switch (input) {
                case 'A':  // Flèche haut
                    game.moveUp();
                    break;
                case 'B':  // Flèche bas
                    game.moveDown();
                    break;
                case 'C':  // Flèche droite
                    game.moveRight();
                    break;
                case 'D':  // Flèche gauche
                    game.moveLeft();
                    break;
            }
        } else if (input == 'q' || input == 'Q') {
            running = false;
        }
    }
    
    std::cout << "Au revoir!" << std::endl;
    return 0;
}