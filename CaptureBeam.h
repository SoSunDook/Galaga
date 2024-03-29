//
// Created by SoSunDook on 16.10.2023.
//

#ifndef GALAGA_CAPTUREBEAM_H
#define GALAGA_CAPTUREBEAM_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class CaptureBeam {
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    int spriteDivisor = 3;
    float spriteScale{};

    bool animationDone{};

    float totalCaptureTime{};
    float captureTimer{};

    float animationDelay{};
    float animationTimer{};
    int currentAnimationFrame{};
//    Rolling
    float rollingDelay{};
    float rollingTimer{};
    int currentRolling{};

    std::shared_ptr<sf::Time> deltaTime{};

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();
    void fixSprite();

    void runAnimation();
public:
    CaptureBeam() = default;
    explicit CaptureBeam(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale);
    ~CaptureBeam() = default;

    void setPosition(float & pos_x, float & pos_y);

    void resetAnimation();

    bool & finishedAnimation();

    void endAnimation();

    sf::Vector2<float> getOrigin();

    void update();

    void render(sf::RenderTarget & target);

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
};


#endif //GALAGA_CAPTUREBEAM_H
