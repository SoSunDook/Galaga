//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_PLAYER_H
#define GALAGA_PLAYER_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class Player {
public:
    enum STATES {
        alive,
        hit,
        captured,
        dead
    };
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    float spriteScale;

    std::shared_ptr<sf::Texture> deathTexture;

    std::shared_ptr<sf::Time> deltaTime;

    sf::Vector2<float> startPos;

    bool doubledPlayer;

    sf::Time playerShootTimer;
    sf::Time playerShootCooldown;

    float velocity;

    int healthPoints;

    STATES currentState;

    int deathSpriteDivisor = 4;
    float deathAnimationTimer;
    float deathAnimationDelay;
    int currentDeathAnimationFrame;

    bool deathAnimationDone;

    void die(bool tp = false);

    void runDeathAnimation();

    void handleHitState();
    void handleDeadState();
    void handleStates();

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
    void setStartPos();
public:
    explicit Player(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedTexture,
                    float & velocity, sf::Time & playerShootCooldown, float & spriteScale);
    ~Player() = default;

    void toGetHit();
    void toGetCaptured();

    void respawn();

    void move(const float x, const float y);

    bool canAttack();

    void update();

    void render(sf::RenderTarget & target);

    int & getHealth();
    STATES & getCurrentState();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
    sf::Vector2<float> getOrigin();
    sf::Vector2<float> & getStartPos();
};


#endif //GALAGA_PLAYER_H
