//
// Created by SoSunDook on 28.09.2023.
//

#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <utility>

void Enemy::initSpawnPath(std::shared_ptr<BezierPath> & spawningPath) {
    this->setPath(spawningPath);
}

void Enemy::initFormation(std::shared_ptr<Formation> & formationP) {
    this->formationPtr = formationP;
}

void Enemy::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Enemy::initPaths(std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & managedPaths) {
    this->paths = managedPaths;
}

void Enemy::initRotation() {
    sf::Vector2<float> direction = this->currentPath->getPath().at(1) - this->currentPath->getPath().at(0);
    auto angle = static_cast<float>(std::atan2(-direction.y, direction.x)) * 180.f / static_cast<float>(M_PI);
    if (angle < 0) {
        angle += 360;
    }
    angle -= 90;
    angle = 360 - angle;
    if (angle > 360) {
        angle -= 360;
    }
    if (angle < 0) {
        angle += 360;
    }
    this->setRotation(angle);
}

void Enemy::initSpawnPosition() {
    this->sprite.setPosition(this->currentPath->getPath().at(0));
}

void Enemy::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
//    Animation startup
    auto size = this->texture->getSize();
    sf::Vector2<int> point(static_cast<int>(size.x) / this->spriteDivisor, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Enemy::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void Enemy::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Enemy::updateDeltaTime() {
    this->deltaTime = this->clock.restart();
}

void Enemy::updateAttack() {

}

void Enemy::updateRotation() {
    float rotationDifference = this->wantedRotation - this->sprite.getRotation();

    float maxRotation = this->rotationVelocity * this->deltaTime.asSeconds();

    if ((rotationDifference > 0 ? rotationDifference : -rotationDifference) < 0.001f) {
        return;
    }

    float angleToRotate;

    if (this->wantedRotation >= this->sprite.getRotation()) {
        if ((rotationDifference > 0 ? rotationDifference : -rotationDifference) <= (360 - (rotationDifference > 0 ? rotationDifference : -rotationDifference))) {
            angleToRotate = rotationDifference;
        } else {
            angleToRotate = rotationDifference - 360;
        }
    } else {
        if ((rotationDifference > 0 ? rotationDifference : -rotationDifference) <= (360 - (rotationDifference > 0 ? rotationDifference : -rotationDifference))) {
            angleToRotate = rotationDifference;
        } else {
            angleToRotate = rotationDifference + 360;
        }
    }

    angleToRotate = static_cast<float>(std::fmod(angleToRotate, 360.0));

    float angleToRotateEpsilon = angleToRotate * static_cast<float>(M_PI) / 180.f;

    if ((angleToRotateEpsilon > 0 ? angleToRotateEpsilon : -angleToRotateEpsilon) <= maxRotation) {
        this->sprite.rotate(angleToRotate);
    } else {
        this->sprite.rotate(static_cast<float>(angleToRotate > 0 ? 1 : -1) * maxRotation * 180.f / static_cast<float>(M_PI));
    }
}

void Enemy::update() {
    this->updateDeltaTime();
    this->updateAttack();
    this->handleStates();
    this->updateRotation();
}

bool Enemy::canAttack() {
    if (this->shootClock.getElapsedTime() > this->enemyShootCooldown) {
        this->shootClock.restart();
        return true;
    }
    return false;
}

void Enemy::setRotation(float & angle) {
    this->sprite.setRotation(angle);
}

void Enemy::setWantedRotation(float & x, float & y) {
    auto angle = static_cast<float>(std::atan2(-y, x)) * 180.f / static_cast<float>(M_PI);
    if (angle < 0) {
        angle += 360;
    }
    angle -= 90;
    angle = 360 - angle;
    if (angle > 360) {
        angle -= 360;
    }
    if (angle < 0) {
        angle += 360;
    }
    this->wantedRotation = angle;
}

void Enemy::setWantedRotation(float & angle) {
    this->wantedRotation = angle;
}

void Enemy::setPosition(float & x, float & y) {
    this->sprite.setPosition(x, y);
}

void Enemy::setPath(std::shared_ptr<BezierPath> & path) {
    this->currentPath = path;
    this->currentPoint = 0;
}

sf::Vector2<float> Enemy::globalFormationPosition() {
    return this->formationPtr->getPosition() + this->localFormationPosition();
}

void Enemy::joinFormation() {
    this->sprite.setPosition(this->globalFormationPosition());
    float zeroRotation = 0.f;
    this->setWantedRotation(zeroRotation);
    this->currentState = STATES::formation;
}

void Enemy::flyInComplete() {
    this->joinFormation();
}

void Enemy::handleFlyInState() {
    if (this->currentPoint < this->currentPath->getPath().size()) {
        sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime.asMilliseconds());
        if (distance <= movement) {
            this->currentPoint++;
        } else {
            this->sprite.move((direction / distance) * movement);
        }
    } else {
        sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime.asMilliseconds());
        if (distance > movement) {
            this->sprite.move((direction / distance) * movement);
        } else {
            this->flyInComplete();
        }
    }
}

void Enemy::handleFormationState() {
    this->sprite.setPosition(this->globalFormationPosition());
}

void Enemy::toDive(bool tp) {
    this->currentState = STATES::dive;
    this->diveStartPosition = this->sprite.getPosition();
}

void Enemy::handleStates() {
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
}

Enemy::STATES & Enemy::getCurrentState() {
    return this->currentState;
}

Enemy::TYPES & Enemy::getType() {
    return this->type;
}

int & Enemy::getIndex() {
    return this->index;
}

sf::FloatRect Enemy::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getLocalBounds() {
    return this->sprite.getLocalBounds();
}