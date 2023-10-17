//
// Created by SoSunDook on 28.09.2023.
//

#include <iostream>
#include "Boss.h"

Boss::Boss(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedBossTexture,
           std::shared_ptr<sf::Texture> & managedBeamTexture, float & velocity,float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex) {
    this->healthPoints = 2;
    this->worthPoints = 150;
    this->type = TYPES::boss;
    this->captureDive = false;
    this->capturing = false;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->currentState = STATES::flyIn;
    this->initSpawnPath(spawningPath);
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedBossTexture);
    this->initPaths(managedPaths);
    this->initSprite();
    this->initOrigin();
    this->initRotation();
    this->initSpawnPosition();
    this->initCaptureBeam(managedBeamTexture);
}

void Boss::initCaptureBeam(std::shared_ptr<sf::Texture> & managedBeamTexture) {
    this->captureBeam = std::make_unique<CaptureBeam>(managedBeamTexture, this->spriteScale);
}

sf::Vector2<float> Boss::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 2)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = -this->formationPtr->getGridSize().y;
    return pos;
}

void Boss::handleCaptureBeam() {
    this->captureBeam->update();

    if (this->captureBeam->finishedAnimation()) {
        sf::Vector2f direction = sf::Vector2<float>(this->sprite.getPosition().x, 740.f) - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime.asMilliseconds());
        if (distance > movement) {
            this->sprite.move((direction / distance) * movement);
        } else {
            this->sprite.setPosition(this->globalFormationPosition().x, this->globalFormationPosition().y - 100);
            this->capturing = false;
        }
    }
}

void Boss::handleDiveState() {
    if (this->currentPoint < this->currentPath->getPath().size()) {
        sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) + this->diveStartPosition - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime.asMilliseconds());
        if (distance <= movement) {
            this->currentPoint++;
        } else {
            this->sprite.move((direction / distance) * movement);
        }

        if (this->currentPoint >= this->currentPath->getPath().size()) {
            if (this->captureDive) {
                this->capturing = true;
                float rotation = 180.f;
                this->setWantedRotation(rotation);
                this->captureBeam->resetClock();
            } else {
                this->sprite.setPosition(this->globalFormationPosition().x, this->globalFormationPosition().y - 100);
            }
        }
    } else {
        if (!this->capturing || !this->captureDive) {
            sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
            this->setWantedRotation(direction.x, direction.y);
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime.asMilliseconds());
            if (distance > movement) {
                this->sprite.move((direction / distance) * movement);
            } else {
                this->joinFormation();
            }
        } else {
            this->handleCaptureBeam();
        }
    }
}

void Boss::handleDeadState() {

}

void Boss::toDive(bool tp) {
    this->captureDive = tp;
    Enemy::toDive(tp);

    if (this->captureDive) {
        this->capturing = false;
        this->captureBeam->resetAnimation();
    }
}


