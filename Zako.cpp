//
// Created by SoSunDook on 28.09.2023.
//

#include "Zako.h"

Zako::Zako(std::shared_ptr<sf::Texture> & managedTexture, float & velocity, float & enemyShootCooldownMax) {
    this->healthPoints = 1;
    this->worthPoints = 50;
    this->velocity = velocity;
    this->enemyShootCooldownMax = enemyShootCooldownMax;
    this->enemyShootCooldown = 0.f;
    this->initTexture(managedTexture);
    this->initSprite();
}