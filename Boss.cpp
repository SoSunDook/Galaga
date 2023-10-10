//
// Created by SoSunDook on 28.09.2023.
//

#include "Boss.h"

Boss::Boss(std::shared_ptr<Formation> & enemyFormationPtr, std::shared_ptr<sf::Texture> & managedTexture, float & velocity,float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex) {
    this->healthPoints = 2;
    this->worthPoints = 150;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->wantedRotation = 0.f;
    this->currentState = STATES::flyIn;
    this->initFormation(enemyFormationPtr);
    this->targetPosition = this->formationPosition();
    this->initTexture(managedTexture);
    this->initSprite();
    this->initOrigin();
}

sf::Vector2<float> Boss::formationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 2)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = -this->formationPtr->getGridSize().y;
    return pos;
}

void Boss::handleDiveState() {

}

void Boss::handleDeadState() {

}


