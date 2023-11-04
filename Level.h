//
// Created by SoSunDook on 04.11.2023.
//

#ifndef GALAGA_LEVEL_H
#define GALAGA_LEVEL_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Zako.h"
#include "Goei.h"
#include "Boss.h"
#include "CapturedPlayer.h"
#include "pugixml.hpp"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Level {
private:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Time> deltaTime;
    std::shared_ptr<std::filesystem::path> dir_path;
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;
    std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> pathManager;

    pugi::xml_document spawningPatterns;

    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<PlayerBullet>> playerBullets;

    std::vector<std::shared_ptr<Zako>> formationZakos;
    std::vector<std::shared_ptr<Goei>> formationGoeis;
    std::vector<std::shared_ptr<Boss>> formationBosses;
    std::vector<std::shared_ptr<EnemyBullet>> enemyBullets;

    std::shared_ptr<Formation> formation;

    float playerVelocity;
    float playerBulletsVelocity;
    float enemyVelocity;
    float enemyBulletsVelocity;
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

    void initConstants();
    void initFormationVectors();
    void initFormation();
    void initSpawningPatterns();
    void initPlayer();
    std::shared_ptr<PlayerBullet> initNewPlBullet();
    std::shared_ptr<EnemyBullet> initNewEnBullet();

    bool enemyFlyingIn();
    void handleSpawning();
    void handleFormation();
    void handleDiving();

    void handlePVE();
    void handleEVP();

    void updateInput();
    void updateBullets();
    void updatePlayers();
    void updateEnemies();
    void updateCombat();
public:
    Level(std::shared_ptr<std::filesystem::path> & dirPath,
          std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
          std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & pathManager,
          std::shared_ptr<sf::Time> & timer,
          std::shared_ptr<sf::RenderWindow> & window);
    ~Level() = default;

    void update();

    void render();
};


#endif //GALAGA_LEVEL_H
