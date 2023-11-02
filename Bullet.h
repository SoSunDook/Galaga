//
// Created by SoSunDook on 27.09.2023.
//

#ifndef GALAGA_BULLET_H
#define GALAGA_BULLET_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class Bullet {
protected:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    float spriteScale{};

    float dir_x{};
    float dir_y{};

    float velocity{};

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
public:
    Bullet() = default;
    ~Bullet() = default;

    void setPosition(float & pos_x, float & pos_y);
    void setDirection(float & new_dir_x, float & new_dir_y);
    void setRotation(const float & rotation);

    void update();

    void render(sf::RenderTarget & target);

    void move();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
    sf::Vector2<float> getOrigin();
};


#endif //GALAGA_BULLET_H
