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
}

Player::Player(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedTexture,
               float & velocity, sf::Time & playerShootCooldown, float & spriteScale) {
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
    this->healthPoints = 3;
    this->doubledPlayer = false;
    this->initTexture(managedTexture);
    this->deathTexture = managedDeathTexture;
    this->initSprite();
    this->initOrigin();
    this->setStartPos();
}

void Player::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
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

void Player::move(const float x, const float y) {
    auto new_x = this->velocity * x;
    auto new_y = this->velocity * y;
    auto leftBorder = this->sprite.getOrigin().x * this->spriteScale;
    auto rightBorder = 720.f - this->sprite.getOrigin().x * this->spriteScale;
    auto currentLocX = this->sprite.getPosition().x;
    if ((currentLocX + new_x < rightBorder) && (currentLocX + new_x > leftBorder)) {
        this->sprite.move(new_x, new_y);
    }
}

void Player::handleHitState() {
    this->runDeathAnimation();
}

void Player::handleDeadState() {
    this->runDeathAnimation();
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
        default:
            break;
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

void Player::die() {
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
}


void Player::toGetHit() {
    this->healthPoints--;
    this->die();
    if (this->healthPoints > 0) {
        this->currentState = STATES::hit;
    } else if (this->healthPoints == 0) {
        this->currentState = STATES::dead;
    }
}

void Player::respawn() {
    this->initSprite();
    this->initOrigin();
    this->setStartPos();
    this->currentState = STATES::alive;
}

int & Player::getHealth() {
    return this->healthPoints;
}

Player::STATES & Player::getCurrentState() {
    return this->currentState;
}

sf::FloatRect Player::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Player::getLocalBounds() {
    return this->sprite.getLocalBounds();
}

sf::Vector2<float> Player::getOrigin() {
    return this->sprite.getOrigin();
}
