//
// Created by SoSunDook on 25.09.2023.
//

#include "Player.h"

void Player::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Player::initSprite(sf::RenderTarget & target) {
    this->sprite.setTexture(*this->texture);
    const float startPosX = (static_cast<float>(target.getSize().x) - this->sprite.getLocalBounds().getSize().x) / 2;
    const float startPosY = static_cast<float>(target.getSize().y) - this->sprite.getLocalBounds().getSize().y;
    this->sprite.setPosition(startPosX, startPosY);
}

Player::Player(std::shared_ptr<sf::Texture> & managedTexture, sf::RenderTarget & target, float & velocity, sf::Time & playerShootCooldown) {
    this->velocity = velocity;
    this->playerShootCooldown = playerShootCooldown;
    this->initTexture(managedTexture);
    this->initSprite(target);
}

void Player::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Player::move(const float x, const float y, sf::RenderTarget & target) {
    auto new_x = this->velocity * x;
    auto new_y = this->velocity * y;
    auto leftBorder = 0.f;
    auto rightBorder = static_cast<float>(target.getSize().x) - this->sprite.getLocalBounds().getSize().x;
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
