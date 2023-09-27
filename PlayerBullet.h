//
// Created by SoSunDook on 27.09.2023.
//

#ifndef GALAGA_PLAYERBULLET_H
#define GALAGA_PLAYERBULLET_H
#include "Bullet.h"

class PlayerBullet : public Bullet {
public:
    PlayerBullet(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale, float & velocity);
    ~PlayerBullet() = default;
};

#endif //GALAGA_PLAYERBULLET_H
