//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_ENEMY_H
#define GALAGA_ENEMY_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class Enemy {
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    float velocity{};

    int healthPoints{};

    int worthPoints{};

    float enemyShootCooldown{};
    float enemyShootCooldownMax{};

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
public:
    Enemy() = default;
    ~Enemy() = default;

    void updateAttack();
    void update();

    void render(sf::RenderTarget & target);

    void move();

    bool canAttack();

    void setPosition(float & x, float & y);

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_ENEMY_H
