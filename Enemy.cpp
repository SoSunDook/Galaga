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
    auto size = this->texture->getSize();
    sf::Vector2<int> point(static_cast<int>(size.x) / this->spriteDivisor, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Enemy::initOrigin() {
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
}

void Enemy::initHitSound(std::shared_ptr<sf::SoundBuffer> & managedDeathSound, float & volume) {
    this->hitSound.setBuffer(*(managedDeathSound));
    this->hitSound.setVolume(volume);
}

void Enemy::initDiveSound(std::shared_ptr<sf::SoundBuffer> & managedDiveSound, float & volume) {
    this->diveSound.setBuffer(*(managedDiveSound));
    this->diveSound.setVolume(volume);
}

void Enemy::render(sf::RenderTarget & target) {
    target.draw(this->sprite);

// For debug only
//    if (this->currentPath && this->currentPoint != this->currentPath->getPath().size()) {
//        for (auto i = 0; i < this->currentPath->getPath().size() - 2; ++i) {
//            if (this->diveStartPosition != sf::Vector2f{}) {
//                sf::Vertex line[] =
//                {
//                    sf::Vertex(this->diveStartPosition + this->currentPath->getPath()[i]),
//                    sf::Vertex(this->diveStartPosition + this->currentPath->getPath()[i + 1])
//                };
//                target.draw(line, 2, sf::Lines);
//            } else {
//                sf::Vertex line[] =
//                {
//                    sf::Vertex(this->currentPath->getPath()[i]),
//                    sf::Vertex(this->currentPath->getPath()[i + 1])
//                };
//                target.draw(line, 2, sf::Lines);
//            }
//        }
//    }
//
}

void Enemy::handleFlyInDiveAnimation() {
    auto module = static_cast<float>(std::fmod(this->sprite.getRotation(), 60.0));
    auto size = this->texture->getSize();
    sf::Vector2<int> point;
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
    if (module <= 30) {
        point = {static_cast<int>(size.x) / this->spriteDivisor, 0};
    } else {
        point = {0, 0};
    }
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Enemy::handleFormationAnimation() {
    if (this->formationPtr->changedTick()) {
        auto size = this->texture->getSize();
        sf::Vector2<int> point;
        sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
        if (this->formationPtr->getTick() % 2 == 0) {
            point = {static_cast<int>(size.x) / this->spriteDivisor, 0};
        } else {
            point = {0, 0};
        }
        const sf::Rect<int> rectangle(point, vector);
        this->sprite.setTextureRect(rectangle);
    }
}

void Enemy::handleAnimation() {
    if (this->type != Enemy::TYPES::capturedPlayer) {
        switch (this->currentState) {
            case flyIn:
                this->handleFlyInDiveAnimation();
                break;
            case formation:
                this->handleFormationAnimation();
                break;
            case dive:
                this->handleFlyInDiveAnimation();
                break;
            case dead:
                break;
        }
    }
}

void Enemy::updateRotation() {
    float rotationDifference = this->wantedRotation - this->sprite.getRotation();

    float maxRotation = this->rotationVelocity * this->deltaTime->asSeconds();

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
    this->handleStates();
    this->updateRotation();
    this->handleAnimation();
}

bool Enemy::canAttack() {
    if (this->enemyShootTimer > this->enemyShootCooldown) {
        this->enemyShootTimer = {};
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

void Enemy::runDeathAnimation() {
    if (!this->deathAnimationDone) {
        this->deathAnimationTimer += this->deltaTime->asSeconds();
        if (this->deathAnimationTimer >= this->deathAnimationDelay) {
            auto size = this->deathTexture->getSize();
            auto frame_x = static_cast<int>(size.x) / this->deathSpriteDivisor;
            sf::Vector2<int> point(frame_x * this->currentDeathAnimationFrame, 0);
            sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
            const sf::Rect<int> rectangle(point, vector);
            this->sprite.setTextureRect(rectangle);
            if (this->currentDeathAnimationFrame < this->deathSpriteDivisor) {
                this->currentDeathAnimationFrame++;
                this->deathAnimationTimer = 0.f;
            } else {
                this->deathAnimationDone = true;

                sf::Vector2<int> point(0, 0);
                sf::Vector2<int> vector(0, 0);
                const sf::Rect<int> rectangle(point, vector);
                this->sprite.setTextureRect(rectangle);
            }
        }
    }
}

void Enemy::handleFlyInState() {
    if (this->currentPoint < this->currentPath->getPath().size()) {
        sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
        if (distance <= movement) {
            this->currentPoint++;
        } else {
            this->sprite.move((direction / distance) * movement);
        }
    } else {
        sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
        this->setWantedRotation(direction.x, direction.y);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
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

void Enemy::handleDeadState() {
    this->runDeathAnimation();
}

void Enemy::toDive(bool tp) {
    this->currentState = STATES::dive;
    this->diveStartPosition = this->sprite.getPosition();
    if (tp) {
        if (this->type == Enemy::TYPES::boss) {
            this->diveSound.play();
        }
    } else {
        this->diveSound.play();
    }
}

void Enemy::die() {
    this->currentState = STATES::dead;
    this->deathAnimationDone = false;
    this->sprite.setTexture(*this->deathTexture);
    this->sprite.setScale(2, 2);
    auto size = this->deathTexture->getSize();
    sf::Vector2<int> point(0, 0);
    sf::Vector2<int> vector(static_cast<int>(size.x) / this->deathSpriteDivisor, static_cast<int>(size.y));
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
    this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
    float zeroRotation = 0;
    this->sprite.setRotation(zeroRotation);
    this->setWantedRotation(zeroRotation);
}

void Enemy::hit() {
    this->healthPoints--;
    if (this->healthPoints <= 0) {
        this->die();
    }
    if (this->type != Enemy::TYPES::boss) {
        this->hitSound.play();
    }
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

int & Enemy::getWorthPoints() {
    return this->worthPoints;
}

sf::Vector2<float> Enemy::getOrigin() {
    return this->sprite.getOrigin();
}

sf::FloatRect Enemy::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getLocalBounds() {
    return this->sprite.getLocalBounds();
}