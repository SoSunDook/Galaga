//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_BOSS_H
#define GALAGA_BOSS_H
#include "Enemy.h"

class Boss : public Enemy {
private:
    sf::Vector2<float> localFormationPosition() override;

    void handleDiveState() override;

    void handleDeadState() override;
public:
    explicit Boss(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture,
                  float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Boss() = default;
};


#endif //GALAGA_BOSS_H
