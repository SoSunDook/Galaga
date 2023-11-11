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

    sf::Sound hit2Sound;
    sf::Sound beamShoot;

    CaptureBeam captureBeam;

    bool captureDive;

    bool capturing;

    void initHit1Sound(std::shared_ptr<sf::SoundBuffer> & managedHit2Sound, float & volume);
    void initBeamShootSound(std::shared_ptr<sf::SoundBuffer> & managedBeamShoot, float & volume);
    void initCaptureBeam(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedBeamTexture);

    sf::Vector2<float> localFormationPosition() override;

    void handleCaptureBeam();

    void handleDiveState() override;

    void handleDeadState() override;
public:
    bool captured;
public:
    explicit Boss(std::shared_ptr<sf::Time> & timer,
                  std::shared_ptr<BezierPath> & spawningPath,
                  std::shared_ptr<Formation> & enemyFormationPtr,
                  std::shared_ptr<sf::Texture> & managedDeathTexture,
                  std::shared_ptr<sf::Texture> & managedBossTexture,
                  std::shared_ptr<sf::Texture> & managedBossHitTexture,
                  std::shared_ptr<sf::Texture> & managedBeamTexture,
                  std::shared_ptr<sf::SoundBuffer> & managedHit1Sound,
                  std::shared_ptr<sf::SoundBuffer> & managedHit2Sound,
                  std::shared_ptr<sf::SoundBuffer> & managedDiveSound,
                  std::shared_ptr<sf::SoundBuffer> & managedBeamShoot,
                  float & volume,
                  float & velocity,
                  float & enemyRotationVelocity,
                  sf::Time & enemyShootCooldown,
                  float & spriteScale,
                  int enemyIndex);
    ~Boss() = default;

    void toDive(bool tp = false) override;

    void hit() override;

    CaptureBeam & getCaptureBeam();

    bool & getCapturing();

    void render(sf::RenderTarget & target) override;

    float & getSpriteScale();
};


#endif //GALAGA_BOSS_H
