//
// Created by SoSunDook on 28.09.2023.
//

#include "Zako.h"

Zako::Zako(std::shared_ptr<sf::Texture> & managedTexture, float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale) {
    this->healthPoints = 1;
    this->worthPoints = 50;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->wantedRotation = 0.f;
    this->currentState = STATES::flyIn;
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
    this->initDynamicPath();
}