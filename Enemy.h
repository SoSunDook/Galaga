//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_ENEMY_H
#define GALAGA_ENEMY_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <valarray>
#include "BezierPath.h"
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
        boss,
        capturedPlayer
    };
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    sf::Sound hitSound;
    sf::Sound diveSound;

    std::shared_ptr<sf::Texture> deathTexture;

    std::shared_ptr<Formation> formationPtr;

    TYPES type{};

    std::shared_ptr<sf::Time> deltaTime{};

    int spriteDivisor = 2;
    float spriteScale{};

    int deathSpriteDivisor = 4;

    std::shared_ptr<BezierPath> currentPath;
    unsigned currentPoint{};

    float velocity{};
    float rotationVelocity{};

    sf::Time enemyShootTimer{};
    sf::Time enemyShootCooldown{};

    float deathAnimationTimer{};
    float deathAnimationDelay = 0.1f;
    int currentDeathAnimationFrame{};

    bool deathAnimationDone{};

    int healthPoints{};
    int worthPoints{};

    STATES currentState{};

    float wantedRotation{};

    int index{};

    sf::Vector2<float> diveStartPosition{};

    void initSpawnPath(std::shared_ptr<BezierPath> & spawningPath);
    void initFormation(std::shared_ptr<Formation> & formationP);
    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    virtual void initSprite();
    void initOrigin();
    void initRotation();
    void initSpawnPosition();
    void initHitSound(std::shared_ptr<sf::SoundBuffer> & managedDeathSound, float & volume);
    void initDiveSound(std::shared_ptr<sf::SoundBuffer> & managedDiveSound, float & volume);
protected:
    virtual sf::Vector2<float> localFormationPosition() = 0;
    sf::Vector2<float> globalFormationPosition();

    void joinFormation();

    void flyInComplete();

    void runDeathAnimation();

    void handleFlyInState();
    void handleFormationState();
    virtual void handleDiveState() = 0;
    virtual void handleDeadState();
    virtual void handleStates();

    void handleFlyInDiveAnimation();
    void handleFormationAnimation();
    void handleAnimation();

    void updateRotation();
public:
    Enemy() = default;
    ~Enemy() = default;

    void update();

    virtual void render(sf::RenderTarget & target);

    virtual void toDive(bool tp = false);

    virtual void hit();

    virtual void die();

    bool canAttack();

    void setRotation(float & angle);

    void setWantedRotation(float & x, float & y);
    void setWantedRotation(float & angle);

    void setPosition(float & x, float & y);

    void setPath(std::shared_ptr<BezierPath> & path);

    STATES & getCurrentState();

    TYPES & getType();

    int & getIndex();

    int & getWorthPoints();

    sf::Vector2<float> getOrigin();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_ENEMY_H
