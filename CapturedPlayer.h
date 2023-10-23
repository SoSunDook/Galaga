//
// Created by SoSunDook on 21.10.2023.
//

#ifndef GALAGA_CAPTUREDPLAYER_H
#define GALAGA_CAPTUREDPLAYER_H
#include "Enemy.h"
#include "Boss.h"

class CapturedPlayer : public Enemy {
public:
    enum CAPTURED_STATES {
        normal,
        onlyCaptured,
        onlyCapturedWithBoss
    };
private:
    sf::Vector2<float> startPos;

    CAPTURED_STATES currentCapturedState;

    sf::Vector2<float> localFormationPosition() override;

    void handleDiveState() override;

    void handleOnlyCapturedState();
    void handleOnlyCapturedWithBossState();

    void handleStates() override;

    bool nextLevel;
public:
    bool bossShotWhileDiving;
    bool bossShotInFormation;

    bool playerLocked;

    bool playerDoubled;
public:
    explicit CapturedPlayer(std::shared_ptr<sf::Time> & timer, std::shared_ptr<Formation> & enemyFormationPtr, sf::Vector2<float> & bossPosition,
                  std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedPlayerTexture, std::shared_ptr<sf::Texture> & managedCapturedPlayerTexture,
                  float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~CapturedPlayer() = default;

    CAPTURED_STATES & getCapturedState();
};


#endif //GALAGA_CAPTUREDPLAYER_H
