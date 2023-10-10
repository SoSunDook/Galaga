//
// Created by SoSunDook on 27.09.2023.
//

#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale, float & velocity) {
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
}
