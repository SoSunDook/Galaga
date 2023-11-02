//
// Created by SoSunDook on 02.11.2023.
//

#ifndef GALAGA_ENEMYBULLET_H
#define GALAGA_ENEMYBULLET_H
#include "Bullet.h"

class EnemyBullet : public Bullet {
public:
    explicit EnemyBullet(std::shared_ptr<sf::Texture> & managedTexture, float & spriteScale, float & velocity);
    ~EnemyBullet() = default;
};


#endif //GALAGA_ENEMYBULLET_H
