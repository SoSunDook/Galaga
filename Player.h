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

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
public:
    explicit Player(std::shared_ptr<sf::Texture> & managedTexture);
    ~Player() = default;

    void update();
    void render(sf::RenderTarget & target);
};


#endif //GALAGA_PLAYER_H
