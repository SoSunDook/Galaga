//
// Created by SoSunDook on 02.11.2023.
//

#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale, float & velocity) {
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
}
