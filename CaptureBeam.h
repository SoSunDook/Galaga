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
    float spriteScale;

    bool animationDone;

    float totalCaptureTime;
    float captureTimer;

    sf::Clock clock;
    sf::Time deltaTime;

    void initTexture(std::shared_ptr<sf::Texture> & managedTexture);
    void initSprite();
    void initOrigin();

    void updateDeltaTime();

    void runAnimation();
public:
    explicit CaptureBeam(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale);
    ~CaptureBeam() = default;

    void setPosition(float & pos_x, float & pos_y);

    void resetClock();

    void resetAnimation();

    bool & finishedAnimation();

    void update();

    void render(sf::RenderTarget & target);
};


#endif //GALAGA_CAPTUREBEAM_H
