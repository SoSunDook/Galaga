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
#include "CapturedPlayer.h"
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

    sf::Clock clock;
    std::shared_ptr<sf::Time> deltaTime;

    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<PlayerBullet>> playerBullets;

    std::vector<std::shared_ptr<Zako>> formationZakos;
    std::vector<std::shared_ptr<Goei>> formationGoeis;
    std::vector<std::shared_ptr<Boss>> formationBosses;
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
    float playersScale;

    int maxCountZako;
    int maxCountGoei;
    int maxCountBoss;

    int currentCountZako;
    int currentCountGoei;
    int currentCountBoss;

    int aliveCountZako;
    int aliveCountGoei;
    int aliveCountBoss;

    int currentFlyInPriority;
    int currentFlyInIndex;

    bool spawningFinished;
    float spawningDelay;
    float spawningTimer;

    float respawningDelay;
    float respawningTimer;

    std::shared_ptr<Goei> divingGoei;
    bool skipFirstGoei;
    float goeiDiveDelay;
    float goeiDiveTimer;

    std::shared_ptr<Zako> firstDivingZako;
    std::shared_ptr<Zako> secondDivingZako;
    float zakoDiveDelay;
    float zakoDiveTimer;

    std::shared_ptr<Boss> divingBoss;
    bool captureDive;
    bool skipFirstBoss;
    float bossDiveDelay;
    float bossDiveTimer;

    std::shared_ptr<CapturedPlayer> capturedPlayer;
    bool savedCapturedPlayer;
    int savedCapturedPlayerIndex;

    void initDeltaTime();
    void initConstants();
    void initWindow();
    void initTextures();
    void initFormationVectors();
    void initFormation();
    void initPaths();
    void initSpawningPatterns();
    void initPlayer();
    std::shared_ptr<PlayerBullet> initNewPlBullet();

    bool enemyFlyingIn();
    void handleSpawning();
    void handleFormation();
    void handleDiving();

    void handlePVE();
    void handleEVP();

    void updateDeltaTime();
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
