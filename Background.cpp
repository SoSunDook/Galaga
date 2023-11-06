//
// Created by SoSunDook on 06.11.2023.
//

#include "Background.h"

Background::Background(std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
                       std::shared_ptr<sf::Time> & timer,
                       const sf::Vector2<float> & pos,
                       const float & bgScale) {
    this->textureManager = textures;
    this->deltaTime = timer;
    this->backgroundScale = bgScale;
    this->initConstants();
    this->initRectangles(pos);
}

void Background::initConstants() {
    this->frontVector = {};
    this->backVector = {};

    this->frontBackgroundTimer = {};
    this->backBackgroundTimer = {};

    this->frontBackgroundFlashDelay = 0.12f;
    this->frontBackgroundPlayDelay = 0.36f;

    this->backBackgroundFlashDelay = 0.06f;
    this->backBackgroundPlayDelay = 0.50f;

    this->frontBackgroundVelocity = 0.25f;
    this->backBackgroundVelocity = 0.15f;

    this->frontShow = true;
    this->backShow = true;
}

void Background::initRectangles(const sf::Vector2<float> & pos) {
    this->frontBackgroundTexture = this->textureManager->operator[]("bg_front");
    this->frontBackgroundSprite.setTexture(*this->frontBackgroundTexture);
    this->frontBackgroundSprite.setScale(this->backgroundScale, this->backgroundScale);

    auto size = this->frontBackgroundTexture->getSize();
    this->frontPoint = {0, static_cast<float>(size.y - 720)};
    this->frontVector = {static_cast<float>(size.x), 720};
    const sf::Rect<int> rectangle(static_cast<sf::Vector2<int>>(this->frontPoint), static_cast<sf::Vector2<int>>(this->frontVector));
    this->frontBackgroundSprite.setTextureRect(rectangle);

    this->frontBackgroundSprite.setOrigin(this->frontBackgroundSprite.getLocalBounds().getSize() / 2.f);
    this->frontBackgroundSprite.setPosition(pos);

    this->backBackgroundTexture = this->textureManager->operator[]("bg_back");
    this->backBackgroundSprite.setTexture(*this->backBackgroundTexture);
    this->backBackgroundSprite.setScale(this->backgroundScale, this->backgroundScale);

    size = this->backBackgroundTexture->getSize();
    this->backPoint = {0, static_cast<float>(size.y - 720)};
    this->backVector = {static_cast<float>(size.x), 720};
    const sf::Rect<int> rect(static_cast<sf::Vector2<int>>(this->backPoint), static_cast<sf::Vector2<int>>(this->backVector));
    this->backBackgroundSprite.setTextureRect(rect);

    this->backBackgroundSprite.setOrigin(this->backBackgroundSprite.getLocalBounds().getSize() / 2.f);
    this->backBackgroundSprite.setPosition(pos);
}

void Background::update() {
    this->frontBackgroundTimer += this->deltaTime->asSeconds();
    this->backBackgroundTimer += this->deltaTime->asSeconds();

    if (this->frontShow) {
        if (this->frontBackgroundTimer >= this->frontBackgroundPlayDelay && this->backShow) {
            this->frontShow = false;
            this->frontBackgroundTimer = {};
        }
    } else {
        if (this->frontBackgroundTimer >= this->frontBackgroundFlashDelay) {
            this->frontShow = true;
            this->frontBackgroundTimer = {};
        }
    }

    if (this->backShow) {
        if (this->backBackgroundTimer >= this->backBackgroundPlayDelay && this->frontShow) {
            this->backShow = false;
            this->backBackgroundTimer = {};
        }
    } else {
        if (this->backBackgroundTimer >= this->backBackgroundFlashDelay) {
            this->backShow = true;
            this->backBackgroundTimer = {};
        }
    }

    if (this->frontPoint.y - this->frontBackgroundVelocity <= 0) {
        this->frontPoint.y = this->frontBackgroundTexture->getSize().y - 720;
    } else {
        this->frontPoint.y -= this->frontBackgroundVelocity;
    }
    const sf::Rect<int> rectangle(static_cast<sf::Vector2<int>>(this->frontPoint), static_cast<sf::Vector2<int>>(this->frontVector));
    this->frontBackgroundSprite.setTextureRect(rectangle);

    if (this->backPoint.y - this->backBackgroundVelocity <= 0) {
        this->backPoint.y = this->backBackgroundTexture->getSize().y - 720;
    } else {
        this->backPoint.y -= this->backBackgroundVelocity;
    }
    const sf::Rect<int> rect(static_cast<sf::Vector2<int>>(this->backPoint), static_cast<sf::Vector2<int>>(this->backVector));
    this->backBackgroundSprite.setTextureRect(rect);
}

void Background::render(sf::RenderTarget & target) {
    if (this->backShow) {
        target.draw(this->backBackgroundSprite);
    }
    if (this->frontShow) {
        target.draw(this->frontBackgroundSprite);
    }
}

