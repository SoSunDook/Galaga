//
// Created by SoSunDook on 04.11.2023.
//

#ifndef GALAGA_MENU_H
#define GALAGA_MENU_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include "Label.h"

class Menu {
private:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Time> deltaTime;
    std::shared_ptr<std::filesystem::path> dir_path;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;

    std::unique_ptr<Label> highScore;
    std::unique_ptr<Label> highScoreInt;
    std::unique_ptr<Label> firstPlayer;
    std::unique_ptr<Label> secondPlayer;
    std::unique_ptr<Label> chooseArrow;
    std::unique_ptr<Label> firstRedLine;
    std::unique_ptr<Label> firstWhiteLine;
    std::unique_ptr<Label> secondWhiteLine;

    sf::Sprite galagaSprite;
    sf::Sprite copyrightSprite;

    unsigned int ordinarySize;
    unsigned int namcoSize;

    float galagaScale;
    float copyrightScale;

    sf::Sprite frontBackgroundSprite;
    sf::Sprite backBackgroundSprite;

    std::shared_ptr<sf::Texture> frontBackgroundTexture;
    std::shared_ptr<sf::Texture> backBackgroundTexture;

    float frontBackgroundFlashTimer;
    float frontBackgroundFlashDelay;

    float backBackgroundFlashTimer;
    float backBackgroundFlashDelay;

    float frontBackgroundVelocity;
    float backBackgroundVelocity;

    float backgroundScale;

    void initConstants();
    void initLabels();
    void initSprites();
    void initBackground();
public:
    Menu(std::shared_ptr<std::filesystem::path> & dirPath,
         std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
         std::shared_ptr<sf::Time> & timer,
         std::shared_ptr<sf::RenderWindow> & window,
         std::shared_ptr<sf::Font> & font);
    ~Menu() = default;

    void update();

    void render();
};


#endif //GALAGA_MENU_H
