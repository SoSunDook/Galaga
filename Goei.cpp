//
// Created by SoSunDook on 28.09.2023.
//

#include <iostream>
#include "Goei.h"

Goei::Goei(std::shared_ptr<sf::Time> & timer,
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
           int enemyIndex) {
    this->healthPoints = 1;
    this->worthPoints = 80;
    this->type = TYPES::goei;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->currentState = STATES::flyIn;
    this->deltaTime = timer;
    this->initSpawnPath(spawningPath);
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedTexture);
    this->deathTexture = managedDeathTexture;
    this->initSprite();
    this->initOrigin();
    this->initRotation();
    this->initSpawnPosition();
    this->initHitSound(managedDeathSound, volume);
    this->initDiveSound(managedDiveSound, volume);
}

sf::Vector2<float> Goei::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 4)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = this->formationPtr->getGridSize().y * static_cast<float>((this->index % 4) / 2);
    return pos;
}

void Goei::handleDiveState() {
    if ((this->sprite.getGlobalBounds().getPosition() + this->sprite.getOrigin() * this->spriteScale).y < 570) {
        this->enemyShootTimer += this->deltaTime.operator*();
    }

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
