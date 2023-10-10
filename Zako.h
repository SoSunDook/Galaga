//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_ZAKO_H
#define GALAGA_ZAKO_H
#include "Enemy.h"

class Zako : public Enemy {
private:
    sf::Vector2<float> formationPosition();

    void handleDiveState() override;

    void handleDeadState() override;
public:
    explicit Zako(std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture, float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Zako() = default;
};


#endif //GALAGA_ZAKO_H
