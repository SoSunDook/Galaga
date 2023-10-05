//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_ZAKO_H
#define GALAGA_ZAKO_H
#include "Enemy.h"

class Zako : public Enemy {
public:
    explicit Zako(std::shared_ptr<sf::Texture> & managedTexture, float & velocity, sf::Time & enemyShootCooldown, float & spriteScale);
    ~Zako() = default;
};


#endif //GALAGA_ZAKO_H
