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
    sf::Clock clock;

    sf::Time playerShootCooldown;

    float velocity;

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite(sf::RenderTarget & target);
public:
    explicit Player(std::shared_ptr<sf::Texture> & managedTexture, sf::RenderTarget & target, float & velocity, sf::Time & playerShootCooldown);
    ~Player() = default;

    void update();

    void render(sf::RenderTarget & target);

    void move(const float x, const float y, sf::RenderTarget & target);

    bool canAttack();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_PLAYER_H
