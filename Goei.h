//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_GOEI_H
#define GALAGA_GOEI_H
#include "Enemy.h"

class Goei : public Enemy {
private:
    sf::Vector2<float> localFormationPosition() override;

    void handleDiveState() override;
public:
    explicit Goei(std::shared_ptr<sf::Time> & timer,
                  std::shared_ptr<BezierPath> & spawningPath,
                  std::shared_ptr<Formation> & enemyFormationPtr,
                  std::shared_ptr<sf::Texture> & managedDeathTexture,
                  std::shared_ptr<sf::Texture> & managedTexture,
                  std::shared_ptr<sf::SoundBuffer> & managedDeathSound,
                  std::shared_ptr<sf::SoundBuffer> & managedDiveSound,
                  float & volume,
                  float & velocity,
                  float & enemyRotationVelocity,
                  sf::Time & enemyShootCooldown,
                  float & spriteScale,
                  int enemyIndex);
    ~Goei() = default;
};


#endif //GALAGA_GOEI_H
