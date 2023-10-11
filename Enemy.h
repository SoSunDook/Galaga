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
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    std::shared_ptr<Formation> formationPtr;

    sf::Clock moveClock{};
    sf::Clock shootClock{};
    sf::Clock rotationClock{};

    int spriteDivisor = 2;
    float spriteScale{};

    std::shared_ptr<BezierPath> currentPath;
    unsigned currentPoint{};

    std::shared_ptr<DynamicBezierPath> dynamicPath;

    float velocity{};
    float rotationVelocity{};

    sf::Time enemyShootCooldown{};

    int healthPoints{};
    int worthPoints{};

    STATES currentState{};

    float wantedRotation{};

    int index{};

    void initFormation(std::shared_ptr<Formation> & formationP);
    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
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
public:
    Enemy() = default;
    ~Enemy() = default;

    void updateRotation();
    void updateAttack();
    void update();

    void render(sf::RenderTarget & target);

    bool canAttack();

    void setRotation(float & angle);

    void setWantedRotation(float & x, float & y);
    void setWantedRotation(float & angle);

    void setPosition(float & x, float & y);

    void setPath(std::shared_ptr<BezierPath> & path);

    STATES & getCurrentState();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_ENEMY_H
