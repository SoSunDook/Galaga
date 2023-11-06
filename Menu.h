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
#include "Background.h"

class Menu {
public:
    enum STATES {
        flyIn,
        choosing,
        onePlayer,
        twoPlayers
    };
private:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Time> deltaTime;
    std::shared_ptr<std::filesystem::path> dir_path;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;

    std::unique_ptr<Background> background;

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

    sf::Vector2<float> galagaEndPos;
    sf::Vector2<float> copyrightEndPos;

    unsigned int ordinarySize;
    unsigned int namcoSize;

    float galagaScale;
    float copyrightScale;

    float flyInVelocity;

    STATES currentState;

    float selectionTimer;
    float selectionDelay;

    bool currentSelection;

    void initConstants();
    void initLabels();
    void initSprites();
    void initBackground();

    void finishFlyIn();

    void handleFlyInState();
    void handleChoosingState();
    void handleStates();

    void updateInput();
public:
    Menu(std::shared_ptr<std::filesystem::path> & dirPath,
         std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
         std::shared_ptr<sf::Time> & timer,
         std::shared_ptr<sf::RenderWindow> & window,
         std::shared_ptr<sf::Font> & font);
    ~Menu() = default;

    void update();

    void render();

    STATES & getCurrentState();
};


#endif //GALAGA_MENU_H
