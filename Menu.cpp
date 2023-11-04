//
// Created by SoSunDook on 04.11.2023.
//

#include "Menu.h"

Menu::Menu(std::shared_ptr<std::filesystem::path> & dirPath,
           std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
           std::shared_ptr<sf::Time> & timer,
           std::shared_ptr<sf::RenderWindow> & window,
           std::shared_ptr<sf::Font> & font) {
    this->dir_path = dirPath;
    this->textureManager = textures;
    this->deltaTime = timer;
    this->window = window;
    this->font = font;
    this->initConstants();
    this->initLabels();
    this->initSprites();
    this->initBackground();
}

void Menu::initConstants() {
    this->ordinarySize = 25;
    this->namcoSize = 31;

    this->galagaScale = 1.f;
    this->copyrightScale = 0.2f;

    this->frontBackgroundFlashTimer = {};
    this->frontBackgroundFlashDelay = 0.3f;

    this->backBackgroundFlashTimer = {};
    this->backBackgroundFlashDelay = 0.2f;

    this->frontBackgroundVelocity = 0.9f;
    this->backBackgroundVelocity = 0.65f;

    this->backgroundScale = 1.25f;
}

void Menu::initLabels() {
    this->highScore = std::make_unique<Label>(this->font,
                                              "HI-SCORE",
                                              sf::Color::Red,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 70.f),
                                              sf::Vector2<float>(450.f, 70.f));
    this->highScoreInt = std::make_unique<Label>(this->font,
                                              "30000",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 96.f),
                                              sf::Vector2<float>(450.f, 96.f));
    this->firstPlayer = std::make_unique<Label>(this->font,
                                              "1 PLAYER",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 384.f),
                                              sf::Vector2<float>(450.f, 384.f));
    this->secondPlayer = std::make_unique<Label>(this->font,
                                              "2 PLAYERS",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 420.f),
                                              sf::Vector2<float>(450.f, 420.f));
    this->chooseArrow = std::make_unique<Label>(this->font,
                                              ">",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(318.f, 384.f),
                                              sf::Vector2<float>(318.f, 384.f));
    this->firstRedLine = std::make_unique<Label>(this->font,
                                              "MPC",
                                              sf::Color::Red,
                                              this->namcoSize,
                                              sf::Vector2<float>(450.f, 554.f),
                                              sf::Vector2<float>(450.f, 554.f));
    this->firstWhiteLine = std::make_unique<Label>(this->font,
                                              "52 / 812 / 2023",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 602.f),
                                              sf::Vector2<float>(450.f, 602.f));
    this->secondWhiteLine = std::make_unique<Label>(this->font,
                                              "ON THE WAY",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 650.f),
                                              sf::Vector2<float>(450.f, 650.f));
}

void Menu::initSprites() {
    this->galagaSprite.setTexture(*this->textureManager->operator[]("galagaLogo"));
    this->galagaSprite.setScale(this->galagaScale, this->galagaScale);
    this->galagaSprite.setOrigin(this->galagaSprite.getLocalBounds().getSize() / 2.f);
    this->galagaSprite.setPosition(450, 240);

    this->copyrightSprite.setTexture(*this->textureManager->operator[]("obla"));
    this->copyrightSprite.setScale(this->copyrightScale, this->copyrightScale);
    this->copyrightSprite.setOrigin(this->copyrightSprite.getLocalBounds().getSize() / 2.f);
    this->copyrightSprite.setPosition(200, 602);
}

void Menu::initBackground() {
    this->frontBackgroundTexture = this->textureManager->operator[]("bg_front");
    this->frontBackgroundSprite.setTexture(*this->frontBackgroundTexture);
    this->frontBackgroundSprite.setScale(this->backgroundScale, this->backgroundScale);

    auto size = this->frontBackgroundTexture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x), 720);
    const sf::Rect<int> rectangle(point, vector);
    this->frontBackgroundSprite.setTextureRect(rectangle);

    this->frontBackgroundSprite.setOrigin(this->frontBackgroundSprite.getLocalBounds().getSize() / 2.f);
    this->frontBackgroundSprite.setPosition(450, 360);


    this->backBackgroundTexture = this->textureManager->operator[]("bg_back");
    this->backBackgroundSprite.setTexture(*this->backBackgroundTexture);
    this->backBackgroundSprite.setScale(this->backgroundScale, this->backgroundScale);

    size = this->backBackgroundTexture->getSize();
    point = {0, 0};
    vector = {static_cast<int>(size.x), 720};
    const sf::Rect<int> rect(point, vector);
    this->backBackgroundSprite.setTextureRect(rect);

    this->backBackgroundSprite.setOrigin(this->backBackgroundSprite.getLocalBounds().getSize() / 2.f);
    this->backBackgroundSprite.setPosition(450, 360);
}

void Menu::update() {

}

void Menu::render() {
    this->window->draw(this->backBackgroundSprite);
    this->window->draw(this->frontBackgroundSprite);

    this->highScore->render(*this->window);
    this->highScoreInt->render(*this->window);
    this->firstPlayer->render(*this->window);
    this->secondPlayer->render(*this->window);
    this->chooseArrow->render(*this->window);
    this->firstRedLine->render(*this->window);
    this->firstWhiteLine->render(*this->window);
    this->secondWhiteLine->render(*this->window);

    this->window->draw(this->galagaSprite);
    this->window->draw(this->copyrightSprite);
}


