//
// Created by SoSunDook on 21.10.2023.
//

#include "CapturedPlayer.h"

CapturedPlayer::CapturedPlayer(std::shared_ptr<sf::Time> & timer, std::shared_ptr<Formation> & enemyFormationPtr, sf::Vector2<float> & bossPosition,
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
    this->nextLevel = false;
    this->bossShotWhileDiving = false;
    this->bossShotInFormation = false;
    this->deltaTime = timer;
    this->startPos = bossPosition;
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedPlayerTexture);
    this->deathTexture = managedDeathTexture;
    this->initSprite();
    this->initOrigin();
    this->initRotation();
    this->initSpawnPosition();
}


sf::Vector2<float> CapturedPlayer::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 2)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = -this->formationPtr->getGridSize().y * 2;
    return pos;
}

void CapturedPlayer::handleDiveState() {
    if (!this->bossShotInFormation && !this->bossShotWhileDiving) {
        if (this->currentPoint < this->currentPath->getPath().size()) {
            sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) + this->diveStartPosition -
                                     this->sprite.getPosition();
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
    } else if (this->bossShotInFormation) {
        if (this->currentPoint < this->currentPath->getPath().size()) {
            sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) + this->diveStartPosition -
                                     this->sprite.getPosition();
            this->setWantedRotation(direction.x, direction.y);
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
            if (distance <= movement) {
                this->currentPoint++;
            } else {
                this->sprite.move((direction / distance) * movement);
            }

            if (this->currentPoint >= this->currentPath->getPath().size()) {
                this->nextLevel = true;
            }
        }
    } else if (this->bossShotWhileDiving) {
        sf::Vector2f endPoint = {360 + this->sprite.getOrigin().x * this->spriteScale, 570.f};
        sf::Vector2f direction = endPoint - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance > movement) {
            this->sprite.move((direction / distance) * movement);
        } else {
            if (this->playerLocked) {
                endPoint = {360 + this->sprite.getOrigin().x * this->spriteScale, 620.f};
                direction = endPoint - this->sprite.getPosition();
                distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
                if (distance > movement) {
                    this->sprite.move((direction / distance) * movement);
                } else {
                    this->playerDoubled = true;
                }
            }
        }
    }
}

void CapturedPlayer::handleOnlyCapturedState() {
    sf::Vector2f endPoint = {startPos.x, startPos.y - this->sprite.getOrigin().y * this->spriteScale};
    sf::Vector2f direction = endPoint - this->sprite.getPosition();
    this->setWantedRotation(direction.x, direction.y);
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
    if (distance > movement) {
        this->sprite.move((direction / distance) * movement);
    } else {
        this->currentCapturedState = CAPTURED_STATES::onlyCapturedWithBoss;
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
        this->currentCapturedState = CAPTURED_STATES::normal;
    }
}

CapturedPlayer::CAPTURED_STATES & CapturedPlayer::getCapturedState() {
    return this->currentCapturedState;
}

void CapturedPlayer::handleStates() {
    if (!this->nextLevel) {
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
        }
    }
}

