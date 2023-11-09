//
// Created by SoSunDook on 21.10.2023.
//

#include <iostream>
#include "CapturedPlayer.h"

CapturedPlayer::CapturedPlayer(std::shared_ptr<sf::Time> & timer, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<Boss> & divingBoss, std::shared_ptr<Player> & player,
                               std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedPlayerTexture, std::shared_ptr<sf::Texture> & managedCapturedPlayerTexture,
                               float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex) {
    this->healthPoints = 1;
    this->worthPoints = 1000;
    this->type = TYPES::capturedPlayer;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->currentState = STATES::formation;
    this->currentCapturedState = CAPTURED_STATES::onlyCaptured;
    this->spriteChanged = false;
    this->savedNextLevel = false;
    this->reachedFirstEndPoint = false;
    this->playerDoubled = false;
    this->playerRespawnUnDoubled = false;
    this->playerLocked = false;
    this->visitedLastRespawn = false;
    this->visitedSaveNextLevel = false;
    this->deltaTime = timer;
    this->relatedBoss = divingBoss;
    this->startPos = {this->relatedBoss->getGlobalBounds().getPosition().x + this->relatedBoss->getOrigin().x * this->relatedBoss->getSpriteScale(),
                     this->relatedBoss->getGlobalBounds().getPosition().y - this->relatedBoss->getOrigin().y * this->relatedBoss->getSpriteScale()};
    this->relatedPlayer = player;
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedPlayerTexture);
    this->deathTexture = managedDeathTexture;
    this->textureRed = managedCapturedPlayerTexture;
    this->initSprite();
    this->initOrigin();
    if (spawningPath) {
        this->initSpawnPath(spawningPath);
        this->initSpawnPosition();
    }
}

void CapturedPlayer::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
    auto size = this->texture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x), static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void CapturedPlayer::initSpriteSaved() {
    this->sprite.setTexture(*this->textureRed);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
    auto size = this->textureRed->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x), static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

sf::Vector2<float> CapturedPlayer::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 2)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = -this->formationPtr->getGridSize().y * 2;
    return pos;
}

void CapturedPlayer::die() {
    this->currentCapturedState = CAPTURED_STATES::normal;
    Enemy::die();
}

void CapturedPlayer::handleDiveState() {
    this->enemyShootTimer += this->deltaTime.operator*();

    if (this->currentPoint < this->currentPath->getPath().size()) {
        sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) + this->diveStartPosition - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance <= movement) {
            this->currentPoint++;
        } else {
            this->sprite.move((direction / distance) * movement);
        }

        if (this->currentPoint >= this->currentPath->getPath().size()) {
            this->sprite.setPosition(this->globalFormationPosition().x, this->globalFormationPosition().y - 200);
        }
    } else {
        sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance > movement) {
            this->sprite.move((direction / distance) * movement);
        } else {
            this->joinFormation();
        }
    }
}

void CapturedPlayer::handleOnlyCapturedState() {
    sf::Vector2f endPoint = startPos;
    sf::Vector2f direction = endPoint - this->sprite.getPosition();
    float rotation = 180;
    this->setWantedRotation(rotation);
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
    if (distance > movement) {
        this->sprite.move((direction / distance) * movement);
    } else {
        if (!this->spriteChanged) {
            this->sprite.setTexture(*this->textureRed);
            this->spriteChanged = true;
        }
        if (this->relatedBoss->getCaptureBeam().finishedAnimation()) {
            this->spriteChanged = false;
            this->currentCapturedState = CAPTURED_STATES::onlyCapturedWithBoss;
        }
    }
}

void CapturedPlayer::handleOnlyCapturedWithBossState() {
    sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
    this->setWantedRotation(direction.x, direction.y);
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
    if (distance > movement) {
        this->sprite.move((direction / distance) * movement);
    } else {
        this->joinFormation();
        this->currentCapturedState = CAPTURED_STATES::normal;
    }
}

void CapturedPlayer::handleBossShotWhileDivingState() {
    if (!reachedFirstEndPoint) {
        sf::Vector2f endPoint = {360 + this->sprite.getOrigin().x * this->spriteScale, 570.f};
        sf::Vector2f direction = endPoint - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance > movement) {
            this->sprite.move((direction / distance) * movement);
        } else {
            this->reachedFirstEndPoint = true;
        }
    } else {
        if (!this->playerDoubled) {
            float zeroRotation = 0;
            this->setWantedRotation(zeroRotation);
        }
        if (this->playerLocked) {
            sf::Vector2f endPoint = {this->relatedPlayer->getStartPos().x + this->sprite.getOrigin().x * this->spriteScale, this->relatedPlayer->getStartPos().y};
            sf::Vector2f direction = endPoint - this->sprite.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
            if (distance > movement) {
                this->sprite.move((direction / distance) * movement);
            } else {
                this->currentState = Enemy::STATES::dead;
                this->playerLocked = false;
                this->playerDoubled = true;
                sf::Vector2<int> point(0, 0);
                sf::Vector2<int> vector(0, 0);
                const sf::Rect<int> rectangle(point, vector);
                this->sprite.setTextureRect(rectangle);
            }
        }
        if ((!this->visitedLastRespawn) && (*(this->relatedPlayer->getHealth()) == 0 && this->relatedPlayer->getCurrentState() == Player::STATES::dead)) {
            sf::Vector2f endPoint = {this->relatedPlayer->getStartPos().x + this->sprite.getOrigin().x * this->spriteScale, this->relatedPlayer->getStartPos().y};
            sf::Vector2f direction = endPoint - this->sprite.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
            if (distance > movement) {
                this->sprite.move((direction / distance) * movement);
            } else {
                this->currentState = Enemy::STATES::dead;
                this->playerRespawnUnDoubled = true;
                this->visitedLastRespawn = true;
                sf::Vector2<int> point(0, 0);
                sf::Vector2<int> vector(0, 0);
                const sf::Rect<int> rectangle(point, vector);
                this->sprite.setTextureRect(rectangle);
            }
        }
    }
}

void CapturedPlayer::handleBossShotInFormationState() {
    if (this->currentPoint < this->currentPath->getPath().size()) {
        if ((this->sprite.getGlobalBounds().getPosition() + this->sprite.getOrigin() * this->spriteScale).y < 570) {
            this->enemyShootTimer += this->deltaTime.operator*();
        }

        sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) + this->diveStartPosition - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance <= movement) {
            this->currentPoint++;
        } else {
            this->sprite.move((direction / distance) * movement);
        }

        if ((this->currentPoint >= this->currentPath->getPath().size()) && (!this->visitedSaveNextLevel)) {
            this->currentState = Enemy::STATES::dead;
            this->savedNextLevel = true;
            this->visitedSaveNextLevel = true;
        }
    }
}

void CapturedPlayer::setState(const Enemy::STATES & newState) {
    this->currentState = newState;
}

void CapturedPlayer::setCapturedState(const CapturedPlayer::CAPTURED_STATES & newState) {
    this->currentCapturedState = newState;
}

CapturedPlayer::CAPTURED_STATES & CapturedPlayer::getCapturedState() {
    return this->currentCapturedState;
}

void CapturedPlayer::handleStates() {
    if (!this->savedNextLevel) {
        switch (this->currentCapturedState) {
            case normal:
                switch (this->currentState) {
                    case flyIn:
                        this->handleFlyInState();
                        break;
                    case formation:
                        this->handleFormationState();
                        break;
                    case dive:
                        this->handleDiveState();
                        break;
                    case dead:
                        this->handleDeadState();
                        break;
                }
                break;
            case onlyCaptured:
                this->handleOnlyCapturedState();
                break;
            case onlyCapturedWithBoss:
                this->handleOnlyCapturedWithBossState();
                break;
            case bossShotInFormation:
                this->handleBossShotInFormationState();
                break;
            case bossShotWhileDiving:
                this->handleBossShotWhileDivingState();
                break;
        }
    }
}

