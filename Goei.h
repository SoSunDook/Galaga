//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_GOEI_H
#define GALAGA_GOEI_H
#include "Enemy.h"

class Goei : public Enemy {
private:
    sf::Vector2<float> formationPosition();

    void handleDiveState() override;

    void handleDeadState() override;
public:
    explicit Goei(std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture, float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Goei() = default;
};


#endif //GALAGA_GOEI_H
