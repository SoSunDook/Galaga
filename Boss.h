//
// Created by SoSunDook on 28.09.2023.
//

#ifndef GALAGA_BOSS_H
#define GALAGA_BOSS_H
#include "Enemy.h"
#include "CaptureBeam.h"

class Boss : public Enemy {
private:
    std::shared_ptr<sf::Texture> hitTexture;

    CaptureBeam captureBeam;

    bool captureDive;

    bool capturing;

    void initCaptureBeam(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedBeamTexture);

    sf::Vector2<float> localFormationPosition() override;

    void handleCaptureBeam();

    void handleDiveState() override;

    void handleDeadState() override;
public:
    bool captured;
public:
    explicit Boss(std::shared_ptr<sf::Time> & timer, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr,
                  std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedBossTexture, std::shared_ptr<sf::Texture> & managedBossHitTexture, std::shared_ptr<sf::Texture> & managedBeamTexture,
                  float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~Boss() = default;

    void toDive(bool tp = false) override;

    void hit() override;

    CaptureBeam & getCaptureBeam();

    bool & getCapturing();

    void render(sf::RenderTarget & target) override;

    float & getSpriteScale();
};


#endif //GALAGA_BOSS_H
