//
// Created by SoSunDook on 21.10.2023.
//

#ifndef GALAGA_CAPTUREDPLAYER_H
#define GALAGA_CAPTUREDPLAYER_H
#include "Enemy.h"
#include "Boss.h"
#include "Player.h"

class CapturedPlayer : public Enemy {
public:
    enum CAPTURED_STATES {
        normal,
        onlyCaptured,
        onlyCapturedWithBoss,
        bossShotWhileDiving,
        bossShotInFormation
    };
private:
    std::shared_ptr<sf::Texture> textureRed;

    std::shared_ptr<Boss> relatedBoss;

    sf::Vector2<float> startPos;

    std::shared_ptr<Player> relatedPlayer;

    CAPTURED_STATES currentCapturedState;

    sf::Vector2<float> localFormationPosition() override;

    void handleDiveState() override;

    void handleOnlyCapturedState();
    void handleOnlyCapturedWithBossState();
    void handleBossShotWhileDivingState();
    void handleBossShotInFormationState();

    void handleStates() override;

    bool spriteChanged;

    bool visitedLastRespawn;
    bool visitedSaveNextLevel;

    bool reachedFirstEndPoint;

    void initSprite() override;
public:
    bool playerLocked;

    bool savedNextLevel;

    bool playerDoubled;
    bool playerRespawnUnDoubled;
public:
    explicit CapturedPlayer(std::shared_ptr<sf::Time> & timer, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<Boss> & divingBoss, std::shared_ptr<Player> & player,
                  std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedPlayerTexture, std::shared_ptr<sf::Texture> & managedCapturedPlayerTexture,
                  float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex);
    ~CapturedPlayer() = default;

    void die() override;

    void setCapturedState(CAPTURED_STATES & newState);

    CAPTURED_STATES & getCapturedState();
};


#endif //GALAGA_CAPTUREDPLAYER_H
