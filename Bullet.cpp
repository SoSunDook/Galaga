//
// Created by SoSunDook on 27.09.2023.
//

#include "Bullet.h"

void Bullet::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Bullet::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
}

void Bullet::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void Bullet::setPosition(float & pos_x, float & pos_y) {
    this->sprite.setPosition(pos_x, pos_y);
}

void Bullet::setDirection(float & new_dir_x, float & new_dir_y) {
    this->dir_x = new_dir_x;
    this->dir_y = new_dir_y;
}

void Bullet::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Bullet::move() {
    this->sprite.move(this->velocity * this->dir_x, this->velocity * this->dir_y);
}

void Bullet::update() {
    this->move();
}

sf::FloatRect Bullet::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Bullet::getLocalBounds() {
    return this->sprite.getLocalBounds();
}

sf::Vector2<float> Bullet::getOrigin() {
    return this->sprite.getOrigin();
}