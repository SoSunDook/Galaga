//
// Created by SoSunDook on 28.09.2023.
//

#include "Enemy.h"

void Enemy::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Enemy::initSprite() {
    this->sprite.setTexture(*this->texture);
}

void Enemy::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Enemy::move() {

}

void Enemy::updateAttack() {
    if (this->enemyShootCooldown < this->enemyShootCooldownMax) {
        this->enemyShootCooldown += 0.1f;
    }
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

sf::FloatRect Enemy::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getLocalBounds() {
    return this->sprite.getLocalBounds();
}