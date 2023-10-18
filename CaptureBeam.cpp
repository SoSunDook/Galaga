//
// Created by SoSunDook on 16.10.2023.
//

#include <iostream>
#include "CaptureBeam.h"

CaptureBeam::CaptureBeam(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale) {
    this->spriteScale = spriteScale;
    this->totalCaptureTime = 7.f;
    this->captureTimer = 0.f;
    this->animationDelay = 0.1f;
    this->animationTimer = {};
    this->clock = {};
    this->deltaTime = {};
    this->animationDone = false;
    this->currentAnimationFrame = 0;
    this->rollingDelay = 0.3f;
    this->currentRolling = 0;
    this->rollingTimer = 0.f;
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
    this->fixSprite();
}

void CaptureBeam::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void CaptureBeam::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
    auto size = this->texture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void CaptureBeam::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void CaptureBeam::fixSprite() {
    auto size = this->texture->getSize();
    auto frame_x = static_cast<int>(size.x) / this->spriteDivisor;
    auto frame_y = static_cast<int>(size.y) / 8;
    sf::Vector2<int> point(frame_x * this->currentAnimationFrame, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, frame_y * this->currentRolling);
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void CaptureBeam::setPosition(float & pos_x, float & pos_y) {
    this->sprite.setPosition(pos_x, pos_y);
}

void CaptureBeam::resetAnimation() {
    this->captureTimer = 0.f;
    this->animationDone = false;
    this->currentAnimationFrame = 0;
    this->currentRolling = 0;
}

void CaptureBeam::resetClock() {
    this->clock.restart();
}

void CaptureBeam::runAnimation() {
    this->captureTimer += this->deltaTime.asSeconds();
    if (this->captureTimer >= this->totalCaptureTime) {
        this->animationDone = true;
        this->sprite.setPosition(0, 0);
    } else {
        auto size = this->texture->getSize();
        auto frame_x = static_cast<int>(size.x) / this->spriteDivisor;
        auto frame_y = static_cast<int>(size.y) / 8;
        sf::Vector2<int> point(frame_x * this->currentAnimationFrame, 0);
        sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, frame_y * this->currentRolling);

        if (this->captureTimer <= 2.5f) {
            this->rollingTimer += this->deltaTime.asSeconds();
            if (this->rollingTimer >= this->rollingDelay) {
                if (this->currentRolling < 8) {
                    this->currentRolling++;
                }
                this->rollingTimer = 0.f;
            }
        } else if (this->captureTimer >= this->totalCaptureTime - 2.5f) {
            this->rollingTimer += this->deltaTime.asSeconds();
            if (this->rollingTimer >= this->rollingDelay) {
                if (this->currentRolling > 0) {
                    this->currentRolling--;
                }
                this->rollingTimer = 0.f;
            }
        }

        this->animationTimer += this->deltaTime.asSeconds();
        if (this->animationTimer >= this->animationDelay) {
            this->currentAnimationFrame++;
            if (this->currentAnimationFrame >= this->spriteDivisor) {
                this->currentAnimationFrame = 0;
            }
            this->animationTimer = 0.f;
        }

        const sf::Rect<int> rectangle(point, vector);
        this->sprite.setTextureRect(rectangle);
    }
}

bool & CaptureBeam::finishedAnimation() {
    return this->animationDone;
}

sf::Vector2<float> CaptureBeam::getOrigin() {
    return this->sprite.getOrigin();
}

void CaptureBeam::updateDeltaTime() {
    this->deltaTime = this->clock.restart();
}

void CaptureBeam::update() {
    this->updateDeltaTime();
    if (!this->animationDone) {
        this->runAnimation();
    }
}

void CaptureBeam::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

