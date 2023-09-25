//
// Created by SoSunDook on 25.09.2023.
//

#include "Player.h"

void Player::initTexture() {
    sf::Texture texture1;
    if (!texture1.loadFromFile("Data/Textures/galaga.png")) {
        throw std::invalid_argument("Galaga image can not be loaded");
    }
    this->texture = std::move(std::make_unique<sf::Texture>(texture1));
//    if (!this->texture->loadFromFile("Data/Textures/galaga.png")) {
//        throw std::invalid_argument("Galaga image can not be loaded");
//    }
}

void Player::initSprite() {
    this->sprite.setTexture(*this->texture);
}

Player::Player() {
    this->initTexture();
    this->initSprite();
}

void Player::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
}