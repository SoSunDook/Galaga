//
// Created by SoSunDook on 25.09.2023.
//
#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Game {
private:
    std::filesystem::path dir_path;
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<Player> player;
    std::map<std::string, std::shared_ptr<sf::Texture>> textureManager;
//    unsigned numberOfEnemies;

    void initWindow();
    void initTextures();
public:
    Game();
    ~Game() = default;

    void update();
    void render();

    void run();
};

#endif //GALAGA_GAME_H
