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

    sf::Sound fighterCaptured;
    sf::Sound fighterRescued;

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
    void initFighterCapturedSound(std::shared_ptr<sf::SoundBuffer> & managedFighterCaptured, float & volume);
    void initFighterRescuedSound(std::shared_ptr<sf::SoundBuffer> & managedFighterRescued, float & volume);
public:
    bool playerLocked;

    bool savedNextLevel;

    bool playerDoubled;
    bool playerRespawnUnDoubled;
public:
    explicit CapturedPlayer(std::shared_ptr<sf::Time> & timer,
                            std::shared_ptr<BezierPath> & spawningPath,
                            std::shared_ptr<Formation> & enemyFormationPtr,
                            std::shared_ptr<Boss> & divingBoss,
                            std::shared_ptr<Player> & player,
                            std::shared_ptr<sf::Texture> & managedDeathTexture,
                            std::shared_ptr<sf::Texture> & managedPlayerTexture,
                            std::shared_ptr<sf::Texture> & managedCapturedPlayerTexture,
                            std::shared_ptr<sf::SoundBuffer> & managedDeathSound,
                            std::shared_ptr<sf::SoundBuffer> & managedDiveSound,
                            std::shared_ptr<sf::SoundBuffer> & managedFighterCaptured,
                            std::shared_ptr<sf::SoundBuffer> & managedFighterRescued,
                            float & volume,
                            float & velocity,
                            float & enemyRotationVelocity,
                            sf::Time & enemyShootCooldown,
                            float & spriteScale,
                            int enemyIndex);
    ~CapturedPlayer() = default;

    void die() override;

    void initSpriteSaved();

    void setState(const STATES & newState);
    void setCapturedState(const CAPTURED_STATES & newState);

    CAPTURED_STATES & getCapturedState();
};


#endif //GALAGA_CAPTUREDPLAYER_H
