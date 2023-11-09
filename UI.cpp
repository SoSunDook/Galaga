//
// Created by SoSunDook on 06.11.2023.
//

#include "UI.h"

UI::UI(std::shared_ptr<std::filesystem::path> & dirPath,
       std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
       std::shared_ptr<sf::Time> & timer,
       std::shared_ptr<sf::RenderWindow> & window,
       std::shared_ptr<sf::Font> & font,
       std::shared_ptr<Highscore> & highScoreObj,
       std::shared_ptr<int> & currentHealth,
       std::shared_ptr<int> & currentScore,
       std::shared_ptr<int> & currentStage) {
    this->dir_path = dirPath;
    this->textureManager = textures;
    this->deltaTime = timer;
    this->window = window;
    this->font = font;
    this->currentHealth = currentHealth;
    this->currentScore = currentScore;
    this->currentStage = currentStage;
    this->highScoreObj = highScoreObj;
    this->initConstants();
    this->initLabels();
    this->initSprites();
}

void UI::initConstants() {
    this->ordinarySize = 25;

    this->galagaScale = 0.75f;
}

void UI::initLabels() {
    this->high = std::make_unique<Label>(this->font,
                                        "HIGH",
                                        sf::Color::Red,
                                        this->ordinarySize,
                                        sf::Vector2<float>(770.f, 40.f),
                                        sf::Vector2<float>(770.f, 40.f));
    this->score = std::make_unique<Label>(this->font,
                                        "SCORE",
                                        sf::Color::Red,
                                        this->ordinarySize,
                                        sf::Vector2<float>(820.f, 67.f),
                                        sf::Vector2<float>(820.f, 67.f));
    this->highScoreInt = std::make_unique<Label>(this->font,
                                        std::to_string(this->highScoreObj->read()),
                                        sf::Color::White,
                                        this->ordinarySize,
                                        sf::Vector2<float>(810.f, 94.f),
                                        sf::Vector2<float>(810.f, 94.f));
    this->player = std::make_unique<Label>(this->font,
                                        "1UP",
                                        sf::Color::Red,
                                        this->ordinarySize,
                                        sf::Vector2<float>(757.5f, 169.f),
                                        sf::Vector2<float>(757.5f, 169.f));
    this->playerScoreInt = std::make_unique<Label>(this->font,
                                        std::to_string(*this->currentScore),
                                        sf::Color::White,
                                        this->ordinarySize,
                                        sf::Vector2<float>(810.f, 196.f),
                                        sf::Vector2<float>(810.f, 196.f));
    this->stage = std::make_unique<Label>(this->font,
                                        "STAGE",
                                        sf::Color::Red,
                                        this->ordinarySize,
                                        sf::Vector2<float>(785.f, 560.f),
                                        sf::Vector2<float>(785.f, 560.f));
    this->stageInt = std::make_unique<Label>(this->font,
                                        std::to_string(*this->currentStage),
                                        sf::Color::White,
                                        this->ordinarySize,
                                        sf::Vector2<float>(810.f, 587.f),
                                        sf::Vector2<float>(810.f, 587.f));
}

void UI::initSprites() {
    this->galagaHealth.resize(*(this->currentHealth));

    for (int i = 0; i < *(this->currentHealth); ++i) {
        this->galagaHealth[i].setTexture(*this->textureManager->operator[]("galaga"));
        this->galagaHealth[i].setScale(this->galagaScale, this->galagaScale);
        this->galagaHealth[i].setOrigin(this->galagaHealth[i].getLocalBounds().getSize() / 2.f);
        this->galagaHealth[i].setPosition(720 + this->galagaHealth[i].getOrigin().x * this->galagaScale + this->galagaHealth[i].getOrigin().x * this->galagaScale * 2 * i, 400);
    }

    this->background.setTexture(*this->textureManager->operator[]("black"));
    this->background.setOrigin(this->background.getLocalBounds().getSize() / 2.f);
    this->background.setPosition(810, 360);
}

void UI::reset() {
    this->initLabels();
    this->initSprites();
}

void UI::update() {
    if (std::to_string(*(this->currentStage)) != this->stageInt->getText()) {
        this->stageInt->update(std::to_string(*(this->currentStage)));
    }
    if (std::to_string(*(this->currentScore)) != this->playerScoreInt->getText()) {
        this->playerScoreInt->update(std::to_string(*(this->currentScore)));
    }
    if (*(this->currentScore) > this->highScoreObj->read()) {
        this->highScoreObj->write(*(this->currentScore));
        this->highScoreInt->update(std::to_string(this->highScoreObj->read()));
    }
}

void UI::render() {
    this->window->draw(this->background);

    this->high->render(*this->window);
    this->score->render(*this->window);
    this->highScoreInt->render(*this->window);
    this->player->render(*this->window);
    this->playerScoreInt->render(*this->window);
    this->stage->render(*this->window);
    this->stageInt->render(*this->window);

    for (int i = 0; i < *(this->currentHealth); ++i) {
        this->window->draw(this->galagaHealth[i]);
    }
}
