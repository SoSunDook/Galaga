//
// Created by SoSunDook on 28.09.2023.
//

#include <iostream>
#include "Boss.h"

Boss::Boss(std::shared_ptr<sf::Time> & timer, std::shared_ptr<BezierPath> & spawningPath, std::shared_ptr<Formation> & enemyFormationPtr,
           std::shared_ptr<sf::Texture> & managedDeathTexture, std::shared_ptr<sf::Texture> & managedBossTexture, std::shared_ptr<sf::Texture> & managedBossHitTexture, std::shared_ptr<sf::Texture> & managedBeamTexture,
           float & velocity,float & enemyRotationVelocity, sf::Time & enemyShootCooldown, float & spriteScale, int enemyIndex) {
    this->healthPoints = 2;
    this->worthPoints = 150;
    this->type = TYPES::boss;
    this->captureDive = false;
    this->capturing = false;
    this->captured = false;
    this->spriteScale = spriteScale;
    this->velocity = velocity;
    this->rotationVelocity = enemyRotationVelocity;
    this->enemyShootCooldown = enemyShootCooldown;
    this->index = enemyIndex;
    this->currentState = STATES::flyIn;
    this->deltaTime = timer;
    this->initSpawnPath(spawningPath);
    this->initFormation(enemyFormationPtr);
    this->initTexture(managedBossTexture);
    this->deathTexture = managedDeathTexture;
    this->hitTexture = managedBossHitTexture;
    this->initSprite();
    this->initOrigin();
    this->initRotation();
    this->initSpawnPosition();
    this->initCaptureBeam(timer, managedBeamTexture);
}

void Boss::initCaptureBeam(std::shared_ptr<sf::Time> & timer, std::shared_ptr<sf::Texture> & managedBeamTexture) {
    this->captureBeam = CaptureBeam(timer, managedBeamTexture, this->spriteScale);
}

CaptureBeam & Boss::getCaptureBeam() {
    return this->captureBeam;
}

sf::Vector2<float> Boss::localFormationPosition() {
    sf::Vector2<float> pos;
    pos.x = (this->formationPtr->getGridSize().x + this->formationPtr->getGridSize().x * 2 * static_cast<float>(this->index / 2)) * static_cast<float>(std::pow(-1, this->index % 2 + 1));
	pos.y = -this->formationPtr->getGridSize().y;
    return pos;
}

void Boss::handleCaptureBeam() {
    this->captureBeam.update();

    if (this->captureBeam.finishedAnimation()) {
        if (!captured) {
            sf::Vector2f direction = sf::Vector2<float>(this->sprite.getPosition().x, 740.f) - this->sprite.getPosition();
            this->setWantedRotation(direction.x, direction.y);
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
            if (distance > movement) {
                this->sprite.move((direction / distance) * movement);
            } else {
                this->sprite.setPosition(this->globalFormationPosition().x, this->globalFormationPosition().y - 100);
                this->capturing = false;
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
}

void Boss::handleDiveState() {
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
            if (this->captureDive) {
                this->capturing = true;
                float rotation = 180.f;
                this->setWantedRotation(rotation);
                auto pos_x = this->sprite.getPosition().x;
                auto pos_y = this->sprite.getPosition().y + this->sprite.getOrigin().y * this->spriteScale + this->captureBeam.getOrigin().y * this->spriteScale;
                this->captureBeam.setPosition(pos_x, pos_y);
            } else {
                this->sprite.setPosition(this->globalFormationPosition().x, this->globalFormationPosition().y - 200);
            }
        }
    } else {
        if (!this->capturing || !this->captureDive) {
            sf::Vector2f direction = this->globalFormationPosition() - this->sprite.getPosition();
            this->setWantedRotation(direction.x, direction.y);
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float movement = this->velocity * static_cast<float>(this->deltaTime->asMilliseconds());
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

void Boss::hit() {
    Enemy::hit();
    if (this->healthPoints == 1) {
        this->sprite.setTexture(*this->hitTexture);
        this->sprite.setScale(this->spriteScale, this->spriteScale);
        auto size = this->hitTexture->getSize();
        sf::Vector2<int> point(static_cast<int>(size.x) / this->spriteDivisor, 0);
        sf::Vector2<int> vector(static_cast<int>(size.x) / this->spriteDivisor, static_cast<int>(size.y));
        const sf::Rect<int> rectangle(point, vector);
        this->sprite.setTextureRect(rectangle);
        this->sprite.setOrigin(this->sprite.getLocalBounds().getSize() / 2.f);
    }
}

void Boss::handleDeadState() {
    Enemy::handleDeadState();
    if (!this->captureBeam.finishedAnimation()) {
        this->captureBeam.endAnimation();
    }
}

void Boss::toDive(bool tp) {
    this->captureDive = tp;
    Enemy::toDive(tp);

    if (this->captureDive) {
        this->capturing = false;
        this->captureBeam.resetAnimation();
    }
}

bool & Boss::getCapturing() {
    return this->capturing;
}

void Boss::render(sf::RenderTarget &target) {
    Enemy::render(target);
    if (this->capturing && !this->captureBeam.finishedAnimation()) {
        this->captureBeam.render(target);
    }
}

float & Boss::getSpriteScale() {
    return this->spriteScale;
}


