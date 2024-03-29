//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_PLAYER_H
#define GALAGA_PLAYER_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
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
    sf::Sprite spriteDoubled;

    sf::Sound deathSound;

    float spriteScale;

    std::shared_ptr<sf::Texture> deathTexture;

    std::shared_ptr<sf::Time> deltaTime;

    sf::Vector2<float> startPos;

    bool doubledPlayer;

    sf::Time playerShootTimer;
    sf::Time playerShootCooldown;

    float velocity;

    std::shared_ptr<int> healthPoints;

    STATES currentState;
    STATES currentDoubledSate;

    int deathSpriteDivisor = 4;
    float deathAnimationTimer;
    float deathAnimationDelay;
    int currentDeathAnimationFrame;

    bool deathAnimationDone;

    float deathAnimationDoubledTimer;
    int currentDeathAnimationDoubledFrame;

    bool deathAnimationDoubledDone;

    void die(bool instant = false);
    void dieDouble();

    void runDeathAnimation();
    void runDeathDoubledAnimation();

    void handleHitState();
    void handleDeadState();
    void handleDeadDoubledState();
    void handleStates();

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
    void setStartPos();
    void initDeathSound(std::shared_ptr<sf::SoundBuffer> & managedDeathSound, float & volume);
public:
    explicit Player(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedTexture, std::shared_ptr<sf::SoundBuffer> & managedDeathSound,
                    float & velocity, sf::Time & playerShootCooldown, float & spriteScale, float & volume);
    ~Player() = default;

    void toGetHit(bool side = false);
    void toGetCaptured();

    void respawn(bool normal = true);

    void toDouble();

    void move(const float x, const float y);

    bool canAttack();

    void reset();

    void update();

    void render(sf::RenderTarget & target);

    bool & getDoubledPlayer();
    std::shared_ptr<int> & getHealth();
    STATES & getCurrentState();
    STATES & getCurrentDoubledState();
    sf::FloatRect getGlobalBoundsMain();
    sf::FloatRect getGlobalBoundsDoubled();
    sf::FloatRect getLocalBounds();
    sf::Vector2<float> getOrigin();
    sf::Vector2<float> & getStartPos();
};


#endif //GALAGA_PLAYER_H
