//
// Created by SoSunDook on 04.11.2023.
//

#include <valarray>
#include "Menu.h"

Menu::Menu(std::shared_ptr<std::filesystem::path> & dirPath,
           std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
           std::shared_ptr<sf::Time> & timer,
           std::shared_ptr<sf::RenderWindow> & window,
           std::shared_ptr<sf::Font> & font,
           std::shared_ptr<Highscore> & highScoreObj) {
    this->dir_path = dirPath;
    this->textureManager = textures;
    this->deltaTime = timer;
    this->window = window;
    this->font = font;
    this->highScoreObj = highScoreObj;
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

    this->galagaEndPos = {};
    this->copyrightEndPos = {};

    this->flyInVelocity = 0.5f;

    this->currentState = STATES::flyIn;

    this->selectionTimer = {};
    this->selectionDelay = 0.15f;

    this->currentSelection = {};
}

void Menu::initLabels() {
    this->highScore = std::make_unique<Label>(this->font,
                                              "HI-SCORE",
                                              sf::Color::Red,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 70.f),
                                              sf::Vector2<float>(450.f, 70.f));
    this->highScoreInt = std::make_unique<Label>(this->font,
                                              std::to_string(this->highScoreObj->read()),
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 96.f),
                                              sf::Vector2<float>(450.f, 96.f));
    this->firstPlayer = std::make_unique<Label>(this->font,
                                              "1 PLAYER",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 384.f),
                                              sf::Vector2<float>(450.f, 384.f));
    this->secondPlayer = std::make_unique<Label>(this->font,
                                              "2 PLAYERS",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 420.f),
                                              sf::Vector2<float>(450.f, 420.f));
    this->chooseArrow = std::make_unique<Label>(this->font,
                                              ">",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(318.f, 720 + 384.f),
                                              sf::Vector2<float>(318.f, 384.f));
    this->firstRedLine = std::make_unique<Label>(this->font,
                                              "MPC",
                                              sf::Color::Red,
                                              this->namcoSize,
                                              sf::Vector2<float>(450.f, 720 + 554.f),
                                              sf::Vector2<float>(450.f, 554.f));
    this->firstWhiteLine = std::make_unique<Label>(this->font,
                                              "52 / 812 / 2023",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 602.f),
                                              sf::Vector2<float>(450.f, 602.f));
    this->secondWhiteLine = std::make_unique<Label>(this->font,
                                              "ON THE WAY",
                                              sf::Color::White,
                                              this->ordinarySize,
                                              sf::Vector2<float>(450.f, 720 + 650.f),
                                              sf::Vector2<float>(450.f, 650.f));
}

void Menu::initSprites() {
    this->galagaSprite.setTexture(*this->textureManager->operator[]("galagaLogo"));
    this->galagaSprite.setScale(this->galagaScale, this->galagaScale);
    this->galagaSprite.setOrigin(this->galagaSprite.getLocalBounds().getSize() / 2.f);
    this->galagaEndPos = {450, 240};
    this->galagaSprite.setPosition(450, 720 + 240);

    this->copyrightSprite.setTexture(*this->textureManager->operator[]("obla"));
    this->copyrightSprite.setScale(this->copyrightScale, this->copyrightScale);
    this->copyrightSprite.setOrigin(this->copyrightSprite.getLocalBounds().getSize() / 2.f);
    this->copyrightEndPos = {200, 602};
    this->copyrightSprite.setPosition(200, 720 + 602);
}

void Menu::initBackground() {
    this->background = std::make_unique<Background>(this->textureManager,
                                                    this->deltaTime,
                                                    sf::Vector2<float>(450.f, 360.f),
                                                    1.25f);
}

void Menu::finishFlyIn() {
    this->highScore->setEndPosition();
    this->highScoreInt->setEndPosition();
    this->firstPlayer->setEndPosition();
    this->secondPlayer->setEndPosition();
    this->chooseArrow->setEndPosition();
    this->firstRedLine->setEndPosition();
    this->firstWhiteLine->setEndPosition();
    this->secondWhiteLine->setEndPosition();

    this->galagaSprite.setPosition(this->galagaEndPos);
    this->copyrightSprite.setPosition(this->copyrightEndPos);

    this->currentState = STATES::choosing;
}

void Menu::handleFlyInState() {
    sf::Vector2f direction = this->galagaEndPos - this->galagaSprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > this->flyInVelocity) {
        this->highScore->move(0, -this->flyInVelocity);
        this->highScoreInt->move(0, -this->flyInVelocity);
        this->firstPlayer->move(0, -this->flyInVelocity);
        this->secondPlayer->move(0, -this->flyInVelocity);
        this->chooseArrow->move(0, -this->flyInVelocity);
        this->firstRedLine->move(0, -this->flyInVelocity);
        this->firstWhiteLine->move(0, -this->flyInVelocity);
        this->secondWhiteLine->move(0, -this->flyInVelocity);

        this->galagaSprite.move(0, -this->flyInVelocity);
        this->copyrightSprite.move(0, -this->flyInVelocity);
    } else {
        this->finishFlyIn();
    }
}

void Menu::handleChoosingState() {
    sf::Vector2<float> newPosition;
    if (this->currentSelection) {
        newPosition = {300.f, 420.f};
    } else {
        newPosition = {318.f, 384.f};
    }
    if (this->chooseArrow->getPosition() != newPosition) {
        this->chooseArrow->setPosition(newPosition);
    }
}

void Menu::handleStates() {
    switch (this->currentState) {
        case STATES::flyIn:
            this->handleFlyInState();
            break;
        case STATES::choosing:
            this->handleChoosingState();
            break;
        case STATES::onePlayer:
            break;
        case STATES::twoPlayers:
            break;
    }
}

void Menu::updateInput() {
    if (this->currentState == STATES::flyIn) {
        this->selectionTimer += this->deltaTime->asSeconds();
        if (this->selectionTimer >= this->selectionDelay) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                this->finishFlyIn();
                this->selectionTimer = {};
            }
        }
    } else if (this->currentState == STATES::choosing) {
        this->selectionTimer += this->deltaTime->asSeconds();
        if (this->selectionTimer >= this->selectionDelay) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                this->currentSelection = !this->currentSelection;
                this->selectionTimer = {};
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (this->currentSelection) {
                    this->currentState = STATES::twoPlayers;
                } else {
                    this->currentState = STATES::onePlayer;
                }
                this->selectionTimer = {};
            }
        }

    }
}

void Menu::reset() {
    this->galagaEndPos = {};
    this->copyrightEndPos = {};

    this->currentState = STATES::flyIn;

    this->selectionTimer = {};

    this->currentSelection = {};

    this->initLabels();
    this->initSprites();
    this->initBackground();
}

void Menu::update() {
    if (std::to_string(this->highScoreObj->read()) != this->highScoreInt->getText()) {
        this->highScoreInt->update(std::to_string(this->highScoreObj->read()));
    }
    this->background->update();
    this->updateInput();
    this->handleStates();
}

void Menu::render() {
    this->background->render(*this->window);

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

Menu::STATES & Menu::getCurrentState() {
    return this->currentState;
}

