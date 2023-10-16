//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_GOEI_H
#define GALAGA_GOEI_H
#include "Enemy.h"

class Goei : public Enemy {
private:
    bool escort;

    sf::Vector2<float> localFormationPosition() override;

    void handleDiveState() override;

    void handleDeadState() override;

    void toDive(bool tp = false);
public:
    explicit Goei(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture,
                  float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Goei() = default;
};


#endif //GALAGA_GOEI_H
