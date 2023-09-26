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

Player::Player(std::shared_ptr<sf::Texture> & managedTexture) {
    this->initTexture(managedTexture);
    this->initSprite();
}

void Player::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
}