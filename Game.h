//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include "PlayerBullet.h"
#include "Zako.h"
#include "Goei.h"
#include "Boss.h"
#include "pugixml.hpp"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Game {
private:
    std::filesystem::path dir_path;
    pugi::xml_document spawningPatterns;
    std::map<std::string, std::shared_ptr<sf::Texture>> textureManager;
    std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> pathManager;

    std::unique_ptr<sf::RenderWindow> window;

    std::unique_ptr<Player> player;
    std::vector<std::shared_ptr<PlayerBullet>> playerBullets;

    std::vector<std::shared_ptr<Enemy>> enemies;
//    std::vector<std::shared_ptr<EnemyBullet>> enemyBullets;

    std::shared_ptr<Formation> formation;

    float playerVelocity;
    float bulletsVelocity;
    float enemyVelocity;
    float enemyRotationVelocity;

    sf::Time playerShootCooldown;
    sf::Time enemyShootCooldown;

    float bulletsScale;
    float enemiesScale;

    int maxCountZako;
    int maxCountGoei;
    int maxCountBoss;

    int currentCountZako;
    int currentCountGoei;
    int currentCountBoss;

    int currentFlyInPriority;
    int currentFlyInIndex;

    bool spawningFinished;
    sf::Time spawningDelay;
    sf::Time spawningTimer;
    sf::Clock spawnClock;


    void initConstants();
    void initWindow();
    void initTextures();
    void initFormation();
    void initPaths();
    void initSpawningPatterns();
    void initPlayer();
    std::shared_ptr<PlayerBullet> initNewPlBullet();

    void handleSpawning();
    void handleFormation();

    void updateInput();
    void updateBullets();
    void updatePlayers();
    void updateEnemies();
    void updateCombat();
    void update();

    void render();
public:
    Game();
    ~Game() = default;

    void run();
};

#endif //GALAGA_GAME_H
