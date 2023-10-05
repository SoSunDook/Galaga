//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Zako.h"
#include "BezierPath.h"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Game {
private:
    std::filesystem::path dir_path;
    std::map<std::string, std::shared_ptr<sf::Texture>> textureManager;
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::Clock> clock;

    std::unique_ptr<Player> player;
    std::vector<std::shared_ptr<PlayerBullet>> playerBullets;

    std::vector<std::shared_ptr<Enemy>> enemies;
//    std::vector<std::shared_ptr<EnemyBullet>> enemyBullets;
    std::map<std::string, std::shared_ptr<BezierPath>> pathManager;

    float playerVelocity;
    float bulletsVelocity;
    float enemyVelocity;

    sf::Time playerShootCooldown;
    sf::Time enemyShootCooldown;

    float bulletsScale;
    float enemiesScale;

//  unsigned numberOfEnemies;
    void initConstants();
    void initWindow();
    void initTextures();
    void initPaths();
    void initPlayer();
    std::shared_ptr<PlayerBullet> initNewPlBullet();
    void initEnemies();
public:
    Game();
    ~Game() = default;

    void updateInput();
    void updateBullets();
    void updatePlayers();
    void updateEnemies();
    void updateCombat();
    void update();

    void render();

    void run();
};

#endif //GALAGA_GAME_H
