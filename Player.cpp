//
// Created by SoSunDook on 25.09.2023.
//

#include <iostream>
#include "Player.h"

void Player::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Player::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
    auto size = this->texture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x), static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Player::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void Player::setStartPos() {
    const float startPosX = 720.f / 2;
    const float startPosY = 720.f - (this->sprite.getGlobalBounds().getSize().y + this->sprite.getOrigin().y * this->spriteScale);
    this->sprite.setPosition(startPosX, startPosY);
    this->startPos = {startPosX, startPosY};
}

void Player::initDeathSound(std::shared_ptr<sf::SoundBuffer> & managedDeathSound, float & volume) {
    this->deathSound.setBuffer(*(managedDeathSound));
    this->deathSound.setVolume(volume);
}

Player::Player(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedTexture, std::shared_ptr<sf::SoundBuffer> & managedDeathSound,
               float & velocity, sf::Time & playerShootCooldown, float & spriteScale, float & volume) {
    this->velocity = velocity;
    this->playerShootCooldown = playerShootCooldown;
    this->playerShootTimer = {};
    this->deltaTime = timer;
    this->spriteScale = spriteScale;
    this->deathAnimationTimer = {};
    this->deathAnimationDelay = 0.1f;
    this->currentDeathAnimationFrame = {};
    this->deathAnimationDone = {};
    this->currentState = STATES::alive;
    this->currentDoubledSate = STATES::alive;
    this->healthPoints = std::make_shared<int>(3);
    this->doubledPlayer = false;
    this->spriteDoubled = {};
    this->deathAnimationDoubledTimer = {};
    this->currentDeathAnimationDoubledFrame = {};
    this->deathAnimationDoubledDone = {};
    this->initTexture(managedTexture);
    this->deathTexture = managedDeathTexture;
    this->initSprite();
    this->initOrigin();
    this->setStartPos();
    this->initDeathSound(managedDeathSound, volume);
}

void Player::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
    if (this->doubledPlayer) {
        target.draw(this->spriteDoubled);
    } else {
        if (!this->deathAnimationDoubledDone) {
            target.draw(this->spriteDoubled);
        }
    }
}

void Player::runDeathAnimation() {
    if (!this->deathAnimationDone) {
        this->deathAnimationTimer += this->deltaTime->asSeconds();
        if (this->deathAnimationTimer >= this->deathAnimationDelay) {
            auto size = this->deathTexture->getSize();
            auto frame_x = static_cast<int>(size.x) / this->deathSpriteDivisor;
            sf::Vector2<int> point(frame_x * this->currentDeathAnimationFrame, 0);
            sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
            const sf::Rect<int> rectangle(point, vector);
            this->sprite.setTextureRect(rectangle);
            if (this->currentDeathAnimationFrame < this->deathSpriteDivisor) {
                this->currentDeathAnimationFrame++;
                this->deathAnimationTimer = 0.f;
            } else {
                this->deathAnimationDone = true;
                sf::Vector2<int> point(0, 0);
                sf::Vector2<int> vector(0, 0);
                const sf::Rect<int> rectangle(point, vector);
                this->sprite.setTextureRect(rectangle);
            }
        }
    }
}

void Player::runDeathDoubledAnimation() {
    if (!this->deathAnimationDoubledDone) {
        this->deathAnimationDoubledTimer += this->deltaTime->asSeconds();
        if (this->deathAnimationDoubledTimer >= this->deathAnimationDelay) {
            auto size = this->deathTexture->getSize();
            auto frame_x = static_cast<int>(size.x) / this->deathSpriteDivisor;
            sf::Vector2<int> point(frame_x * this->currentDeathAnimationDoubledFrame, 0);
            sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
            const sf::Rect<int> rectangle(point, vector);
            this->spriteDoubled.setTextureRect(rectangle);
            if (this->currentDeathAnimationDoubledFrame < this->deathSpriteDivisor) {
                this->currentDeathAnimationDoubledFrame++;
                this->deathAnimationDoubledTimer = 0.f;
            } else {
                this->deathAnimationDoubledDone = true;
                sf::Vector2<int> point(0, 0);
                sf::Vector2<int> vector(0, 0);
                const sf::Rect<int> rectangle(point, vector);
                this->spriteDoubled.setTextureRect(rectangle);
            }
        }
    }
}

void Player::move(const float x, const float y) {
    auto new_x = this->velocity * x;
    auto new_y = this->velocity * y;
    auto leftBorder = this->sprite.getOrigin().x * this->spriteScale;
    float rightBorder;
    if (!this->doubledPlayer || this->currentDoubledSate == STATES::dead) {
        rightBorder = 720.f - this->sprite.getOrigin().x * this->spriteScale;
        auto currentLocX = this->sprite.getPosition().x;
        if ((currentLocX + new_x < rightBorder) && (currentLocX + new_x > leftBorder)) {
            this->sprite.move(new_x, new_y);
        }
    } else {
        rightBorder = 720.f - this->spriteDoubled.getOrigin().x * this->spriteScale;
        auto currentLocLeft = this->sprite.getPosition().x;
        auto currentLocRight = this->spriteDoubled.getPosition().x;
        if ((currentLocRight + new_x < rightBorder) && (currentLocLeft + new_x > leftBorder)) {
            this->sprite.move(new_x, new_y);
            this->spriteDoubled.move(new_x, new_y);
        }
    }
}

void Player::handleHitState() {
    this->runDeathAnimation();
}

void Player::handleDeadState() {
    this->runDeathAnimation();
}

void Player::handleDeadDoubledState() {
    this->runDeathDoubledAnimation();
}

void Player::handleStates() {
    switch (this->currentState) {
        case STATES::alive:
            break;
        case STATES::hit:
            this->handleHitState();
            break;
        case STATES::dead:
            this->handleDeadState();
            break;
        case STATES::captured:
            break;
        default:
            break;
    }
    if (this->doubledPlayer) {
        switch (this->currentDoubledSate) {
            case STATES::alive:
                break;
            case STATES::dead:
                this->handleDeadDoubledState();
                break;
            default:
                break;
        }
    } else {
        if (!this->deathAnimationDoubledDone) {
            switch (this->currentDoubledSate) {
                case STATES::alive:
                    break;
                case STATES::dead:
                    this->handleDeadDoubledState();
                    break;
                default:
                    break;
            }
        }
    }
}

void Player::update() {
    this->playerShootTimer += this->deltaTime.operator*();
    this->handleStates();
}

bool Player::canAttack() {
    if (this->playerShootTimer > this->playerShootCooldown) {
        this->playerShootTimer = {};
        return true;
    }
    return false;
}

void Player::reset() {
    this->playerShootTimer = {};
    this->deathAnimationTimer = {};
    this->currentDeathAnimationFrame = {};
    this->deathAnimationDone = {};
    this->currentState = STATES::alive;
    this->currentDoubledSate = STATES::alive;
    *(this->healthPoints) = 3;
    this->doubledPlayer = false;
    this->spriteDoubled = {};
    this->deathAnimationDoubledTimer = {};
    this->currentDeathAnimationDoubledFrame = {};
    this->deathAnimationDoubledDone = {};
    this->initSprite();
    this->initOrigin();
    this->setStartPos();
}

void Player::die(bool instant) {
    if (!instant) {
        this->deathAnimationDone = false;
        this->currentDeathAnimationFrame = 0;
        this->sprite.setTexture(*this->deathTexture);
        this->sprite.setScale(2, 2);
        auto size = this->deathTexture->getSize();
        sf::Vector2<int> point(0, 0);
        sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
        const sf::Rect<int> rectangle(point, vector);
        this->sprite.setTextureRect(rectangle);
        this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
        float zeroRotation = 0;
        this->sprite.setRotation(zeroRotation);
    } else {
        this->deathAnimationDone = true;
        sf::Vector2<int> point;
        sf::Vector2<int> vector;
        const sf::Rect<int> rectangle(point, vector);
        this->sprite.setTextureRect(rectangle);
    }
}

void Player::dieDouble() {
    this->deathAnimationDoubledDone = false;
    this->currentDeathAnimationDoubledFrame = 0;
    this->spriteDoubled.setTexture(*this->deathTexture);
    this->spriteDoubled.setScale(2, 2);
    auto size = this->deathTexture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->spriteDoubled.setTextureRect(rectangle);
    this->spriteDoubled.setOrigin(this->spriteDoubled.getLocalBounds().getSize() / 2.f);
    float zeroRotation = 0;
    this->spriteDoubled.setRotation(zeroRotation);
}

void Player::toDouble() {
    this->doubledPlayer = true;
    this->currentDoubledSate = STATES::alive;

    this->spriteDoubled.setTexture(*this->texture);
    this->spriteDoubled.setScale(this->spriteScale, this->spriteScale);
    auto size = this->texture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x), static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->spriteDoubled.setTextureRect(rectangle);

    this->spriteDoubled.setOrigin(this->spriteDoubled.getLocalBounds().getSize() / 2.f);

    sf::Vector2<float> pos = {this->sprite.getGlobalBounds().getPosition().x + (this->sprite.getOrigin().x * this->spriteScale * 3), this->sprite.getGlobalBounds().getPosition().y + this->sprite.getOrigin().y * this->spriteScale};
    this->spriteDoubled.setPosition(pos);
}

void Player::toGetHit(bool side) {
    this->deathSound.play();
    if (!this->doubledPlayer) {
        *(this->healthPoints) -= 1;
        this->die();
        if (*(this->healthPoints) > 0) {
            this->currentState = STATES::hit;
        } else {
            this->currentState = STATES::dead;
        }
    } else {
        if (!side) {
            std::swap(this->sprite, this->spriteDoubled);
            std::swap(this->deathAnimationTimer, this->deathAnimationDoubledTimer);
            std::swap(this->currentDeathAnimationFrame, this->currentDeathAnimationDoubledFrame);
            std::swap(this->deathAnimationDone, this->deathAnimationDoubledDone);
        }
        this->dieDouble();
        this->currentDoubledSate = STATES::dead;
        this->doubledPlayer = false;
    }
}

void Player::toGetCaptured() {
    *(this->healthPoints) -= 1;
    this->die(true);
    if (*(this->healthPoints) != 0) {
        this->currentState = STATES::captured;
    } else {
        this->currentState = STATES::dead;
    }
}

void Player::respawn(bool normal) {
    this->initSprite();
    this->initOrigin();
    if (normal) {
        this->setStartPos();
    } else {
        sf::Vector2<float> pos = {this->startPos.x + this->sprite.getOrigin().x * this->spriteScale, this->startPos.y};
        this->sprite.setPosition(pos);
        *(this->healthPoints) += 1;
    }
    this->currentState = STATES::alive;
}

std::shared_ptr<int> & Player::getHealth() {
    return this->healthPoints;
}

bool & Player::getDoubledPlayer() {
    return this->doubledPlayer;
}

Player::STATES & Player::getCurrentState() {
    return this->currentState;
}

Player::STATES & Player::getCurrentDoubledState() {
    return this->currentDoubledSate;
}

sf::FloatRect Player::getGlobalBoundsMain() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Player::getGlobalBoundsDoubled() {
    return this->spriteDoubled.getGlobalBounds();
}

sf::FloatRect Player::getLocalBounds() {
    return this->sprite.getLocalBounds();
}

sf::Vector2<float> Player::getOrigin() {
    return this->sprite.getOrigin();
}

sf::Vector2<float> & Player::getStartPos() {
    return this->startPos;
}
