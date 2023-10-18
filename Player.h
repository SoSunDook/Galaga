//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_PLAYER_H
#define GALAGA_PLAYER_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class Player {
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    float spriteScale;

    std::shared_ptr<sf::Time> deltaTime;

    sf::Time playerShootTimer;
    sf::Time playerShootCooldown;

    float velocity;

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
    void setStartPos();
public:
    explicit Player(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedTexture, float & velocity, sf::Time & playerShootCooldown, float & spriteScale);
    ~Player() = default;

    void update();

    void render(sf::RenderTarget & target);

    void move(const float x, const float y);

    bool canAttack();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
    sf::Vector2<float> getOrigin();
};


#endif //GALAGA_PLAYER_H
