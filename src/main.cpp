#include "../include/Game.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::cout << "=== JEU 2048 ===" << std::endl;
    std::cout << "Démarrage..." << std::endl;
    
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "ERREUR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Jeu terminé." << std::endl;
    return EXIT_SUCCESS;
}