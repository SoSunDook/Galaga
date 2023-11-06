//
// Created by SoSunDook on 06.11.2023.
//

#ifndef GALAGA_BACKGROUND_H
#define GALAGA_BACKGROUND_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

class Background {
private:
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;
    std::shared_ptr<sf::Time> deltaTime;

    sf::Sprite frontBackgroundSprite;
    sf::Sprite backBackgroundSprite;

    std::shared_ptr<sf::Texture> frontBackgroundTexture;
    std::shared_ptr<sf::Texture> backBackgroundTexture;

    sf::Vector2<float> frontPoint;
    sf::Vector2<float> frontVector;

    sf::Vector2<float> backPoint;
    sf::Vector2<float> backVector;

    float frontBackgroundTimer;
    float backBackgroundTimer;

    float frontBackgroundFlashDelay;
    float frontBackgroundPlayDelay;

    float backBackgroundFlashDelay;
    float backBackgroundPlayDelay;

    float frontBackgroundVelocity;
    float backBackgroundVelocity;

    bool frontShow;
    bool backShow;

    float backgroundScale;

    void initConstants();
    void initRectangles(const sf::Vector2<float> & pos);
public:
    Background(std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
               std::shared_ptr<sf::Time> & timer,
               const sf::Vector2<float> & pos,
               const float & bgScale);
    ~Background() = default;

    void update();

    void render(sf::RenderTarget & target);
};


#endif //GALAGA_BACKGROUND_H
