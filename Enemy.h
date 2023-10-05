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

class Enemy {
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    sf::Clock moveClock{};
    sf::Clock shootClock{};

    int spriteDivisor = 2;
    float spriteScale{};

    std::shared_ptr<BezierPath> currentPath;
    unsigned currentPoint;

    std::shared_ptr<DynamicBezierPath> dynamicPath;

    float velocity{};

    int healthPoints{};

    int worthPoints{};

    sf::Time enemyShootCooldown{};

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
public:
    Enemy() = default;
    ~Enemy() = default;

    void updateRotation(float & x, float & y);
    void updateAttack();
    void update();

    void render(sf::RenderTarget & target);

    void move();

    bool canAttack();

    void setPosition(float & x, float & y);

    void setPath(std::shared_ptr<BezierPath> & path);

    void initDynamicPath();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_ENEMY_H
