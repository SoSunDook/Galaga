//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_BOSS_H
#define GALAGA_BOSS_H
#include "Enemy.h"
#include "CaptureBeam.h"

class Boss : public Enemy {
private:
    std::unique_ptr<CaptureBeam> captureBeam;

    bool captureDive;

    bool capturing;

    void initCaptureBeam(std::shared_ptr<sf::Texture> & managedBeamTexture);

    sf::Vector2<float> localFormationPosition() override;

    void handleCaptureBeam();

    void handleDiveState() override;

    void handleDeadState() override;

    void toDive(bool tp = false) override;
public:
    explicit Boss(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedBossTexture,
                  std::shared_ptr<sf::Texture> & managedBeamTexture, float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Boss() = default;
};


#endif //GALAGA_BOSS_H
