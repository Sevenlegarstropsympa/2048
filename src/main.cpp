#include "../include/Game.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::cout << "Démarrage du jeu 2048..." << std::endl;
    
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Erreur inconnue!" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Jeu terminé." << std::endl;
    return EXIT_SUCCESS;
}