//
// Created by SoSunDook on 28.09.2023.
//

#include "Zako.h"

Zako::Zako(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture,
           float & velocity, float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex) {
    this->healthPoints = 1;
    this->worthPoints = 50;
    this->type = TYPES::zako;
    this->diver = false;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->currentState = STATES::flyIn;
    this->initSpawnPath(spawningPath);
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedTexture);
    this->initPaths(managedPaths);
    this->initSprite();
    this->initOrigin();
    this->initRotation();
    this->initSpawnPosition();
}

sf::Vector2<float> Zako::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 4)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = this->formationPtr->getGridSize().y * 2 + this->formationPtr->getGridSize().y * static_cast<float>((this->index % 4) / 2);
    return pos;
}

void Zako::handleDiveState() {
    
}

void Zako::handleDeadState() {

}

void Zako::flyInComplete() {
    if (this->diver) {
        this->currentState = STATES::dead;
    } else {
        this->joinFormation();
    }
}

