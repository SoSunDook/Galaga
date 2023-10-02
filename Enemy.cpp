//
// Created by SoSunDook on 28.09.2023.
//

#include "Enemy.h"

#include <cmath>

void Enemy::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Enemy::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
//    Animation startup
    auto size = this->texture->getSize();
    sf::Vector2<int> point(size.x / this->spriteDivisor, 0);
    sf::Vector2<int> vector(size.x / this->spriteDivisor, size.y);
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Enemy::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Enemy::move() {
    sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) - this->sprite.getPosition();
    this->updateRotation(direction.x, direction.y);
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance < this->velocity) {
        this->currentPoint++;
        if (this->currentPoint >= this->currentPath->getPath().size()) {
            this->currentPoint = 0;
        }
    } else {
        sf::Vector2f step = (direction / distance) * this->velocity;
        this->sprite.move(step);
    }
}

void Enemy::updateAttack() {
    if (this->enemyShootCooldown < this->enemyShootCooldownMax) {
        this->enemyShootCooldown += 0.1f;
    }
}

void Enemy::updateRotation(float & x, float & y) {
    auto angle = (static_cast<float>(std::atan2(y,x)) * 180.f / static_cast<float>(M_PI)) + 90;
    this->sprite.setRotation(angle);
}

void Enemy::update() {
    this->updateAttack();
    this->move();
}

bool Enemy::canAttack() {
    if (this->enemyShootCooldown >= this->enemyShootCooldownMax) {
        this->enemyShootCooldown = 0.f;
        return true;
    }
    return false;
}

void Enemy::setPosition(float & x, float & y) {
    this->sprite.setPosition(x, y);
}

void Enemy::setPath(std::shared_ptr<BezierPath> & path) {
    this->currentPath = path;
    this->currentPoint = 0;
}

sf::FloatRect Enemy::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getLocalBounds() {
    return this->sprite.getLocalBounds();
}