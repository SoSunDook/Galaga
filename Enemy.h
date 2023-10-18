//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_ENEMY_H
#define GALAGA_ENEMY_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <valarray>
#include "BezierPath.h"
#include "DynamicBezierPath.h"
#include "Formation.h"

class Enemy {
public:
    enum STATES {
        flyIn,
        formation,
        dive,
        dead
    };
    enum TYPES {
        zako,
        goei,
        boss
    };
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    std::shared_ptr<Formation> formationPtr;

    std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> paths;

    TYPES type{};

    std::shared_ptr<sf::Time> deltaTime{};

    int spriteDivisor = 2;
    float spriteScale{};

    std::shared_ptr<BezierPath> currentPath;
    unsigned currentPoint{};

    float velocity{};
    float rotationVelocity{};

    sf::Time enemyShootTimer{};
    sf::Time enemyShootCooldown{};

    int healthPoints{};
    int worthPoints{};

    STATES currentState{};

    float wantedRotation{};

    int index{};

    sf::Vector2<float> diveStartPosition{};

    void initSpawnPath(std::shared_ptr<BezierPath> & spawningPath);
    void initFormation(std::shared_ptr<Formation> & formationP);
    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initPaths(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths);
    void initSprite();
    void initOrigin();
    void initRotation();
    void initSpawnPosition();
protected:
    virtual sf::Vector2<float> localFormationPosition() = 0;
    sf::Vector2<float> globalFormationPosition();

    void joinFormation();

    void flyInComplete();

    void handleFlyInState();
    void handleFormationState();
    virtual void handleDiveState() = 0;
    virtual void handleDeadState() = 0;
    void handleStates();

    void updateRotation();
    void updateAttack();
public:
    Enemy() = default;
    ~Enemy() = default;

    void update();

    virtual void render(sf::RenderTarget & target);

    virtual void toDive(bool tp = false);

    bool canAttack();

    void setRotation(float & angle);

    void setWantedRotation(float & x, float & y);
    void setWantedRotation(float & angle);

    void setPosition(float & x, float & y);

    void setPath(std::shared_ptr<BezierPath> & path);

    STATES & getCurrentState();

    TYPES & getType();

    int & getIndex();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_ENEMY_H
