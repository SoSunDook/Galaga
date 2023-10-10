//
// Created by SoSunDook on 25.09.2023.
//

#include "Player.h"

void Player::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Player::initSprite() {
    this->sprite.setTexture(*this->texture);
}

void Player::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void Player::setStartPos() {
    const float startPosX = 720.f / 2;
    const float startPosY = 720.f - this->sprite.getLocalBounds().getSize().y;
    this->sprite.setPosition(startPosX, startPosY);
}

Player::Player(std::shared_ptr<sf::Texture> & managedTexture, float & velocity, sf::Time & playerShootCooldown) {
    this->velocity = velocity;
    this->playerShootCooldown = playerShootCooldown;
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
    this->setStartPos();
}

void Player::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Player::move(const float x, const float y) {
    auto new_x = this->velocity * x;
    auto new_y = this->velocity * y;
    auto leftBorder = this->sprite.getOrigin().x;
    auto rightBorder = 720.f - this->sprite.getOrigin().x;
    auto currentLocX = this->sprite.getPosition().x;
    if ((currentLocX + new_x < rightBorder) && (currentLocX + new_x > leftBorder)) {
        this->sprite.move(new_x, new_y);
    }
}

void Player::update() {

}

bool Player::canAttack() {
    if (this->clock.getElapsedTime() > this->playerShootCooldown) {
        this->clock.restart();
        return true;
    }
    return false;
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
