//
// Created by SoSunDook on 06.11.2023.
//

#ifndef GALAGA_UI_H
#define GALAGA_UI_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include "Label.h"
#include "Highscore.h"

class UI {
private:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Time> deltaTime;
    std::shared_ptr<std::filesystem::path> dir_path;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;
    std::shared_ptr<Highscore> highScoreObj;

    std::shared_ptr<int> currentHealth;
    std::shared_ptr<int> currentScore;
    std::shared_ptr<int> currentStage;
    std::shared_ptr<bool> currentPlayer;

    std::unique_ptr<Label> high;
    std::unique_ptr<Label> score;
    std::unique_ptr<Label> highScoreInt;
    std::unique_ptr<Label> player;
    std::unique_ptr<Label> playerScoreInt;
    std::unique_ptr<Label> stage;
    std::unique_ptr<Label> stageInt;

    std::vector<sf::Sprite> galagaHealth;

    sf::Sprite background;

    unsigned int ordinarySize;

    float galagaScale;

    void initConstants();
    void initLabels();
    void initSprites();
public:
    explicit UI(std::shared_ptr<std::filesystem::path> & dirPath,
                std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
                std::shared_ptr<sf::Time> & timer,
                std::shared_ptr<sf::RenderWindow> & window,
                std::shared_ptr<sf::Font> & font,
                std::shared_ptr<Highscore> & highScoreObj,
                std::shared_ptr<int> & currentHealth,
                std::shared_ptr<int> & currentScore,
                std::shared_ptr<int> & currentStage,
                std::shared_ptr<bool> & currentPlayer);
    ~UI() = default;

    void reset();

    void update();

    void render();
};


#endif //GALAGA_UI_H
