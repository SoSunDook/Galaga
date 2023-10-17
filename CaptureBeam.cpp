//
// Created by SoSunDook on 16.10.2023.
//

#include <iostream>
#include "CaptureBeam.h"

CaptureBeam::CaptureBeam(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale) {
    this->spriteScale = spriteScale;
    this->totalCaptureTime = 6.f;
    this->captureTimer = 0.f;
    this->clock = {};
    this->deltaTime = {};
    this->animationDone = false;
    this->initTexture(managedTexture);
    this->initSprite();
}

void CaptureBeam::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void CaptureBeam::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
    auto size = this->texture->getSize();
    sf::Vector2<int> point(static_cast<int>(size.x) / this->spriteDivisor, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void CaptureBeam::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void CaptureBeam::setPosition(float & pos_x, float & pos_y) {
    this->sprite.setPosition(pos_x, pos_y);
}

void CaptureBeam::resetAnimation() {
//    Reset the animation
    this->captureTimer = 0.f;
    this->animationDone = false;
}

void CaptureBeam::resetClock() {
    this->clock.restart();
}

void CaptureBeam::runAnimation() {
    this->captureTimer += this->deltaTime.asSeconds();
    if (this->captureTimer >= this->totalCaptureTime) {
        this->animationDone = true;
    } else {

    }
}

bool & CaptureBeam::finishedAnimation() {
    return this->animationDone;
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

