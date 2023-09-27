//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include "PlayerBullet.h"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Game {
private:
    std::filesystem::path dir_path;
    std::map<std::string, std::shared_ptr<sf::Texture>> textureManager;
    std::unique_ptr<sf::RenderWindow> window;

    std::unique_ptr<Player> player;
    std::vector<std::shared_ptr<PlayerBullet>> playerBullets;

    float playerVelocity = 0.1f;
    float bulletsVelocity = 0.3f;

    float playerShootCooldownMax = 220.f;

    float bulletsScale = 3.f;

//  unsigned numberOfEnemies;

    void initWindow();
    void initTextures();
    void initPlayer();
public:
    Game();
    ~Game() = default;

    void updateInput();
    void updateBullets();
    void updatePlayers();
    void update();

    void render();

    void run();
};

#endif //GALAGA_GAME_H
