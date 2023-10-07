//
// Created by SoSunDook on 28.09.2023.
//

#include "Enemy.h"
#include <cmath>
#include <iostream>

void Enemy::initTexture(std::shared_ptr<sf::Texture> & managedTexture) {
    this->texture = managedTexture;
}

void Enemy::initSprite() {
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(this->spriteScale, this->spriteScale);
//    Animation startup
    auto size = this->texture->getSize();
    sf::Vector2<int> point(size.x / this->spriteDivisor, 0);
    sf::Vector2<int> vector(size.x / this->spriteDivisor, size.y);
    const sf::Rect<int> rectangle(point, vector);
    this->sprite.setTextureRect(rectangle);
}

void Enemy::initDynamicPath() {
    int sample = 120;
    sf::Vector2<float> origin(300.f, 300.f);
    sf::Vector2<float> start(620.f, 620.f);
    this->dynamicPath = std::make_shared<DynamicBezierPath>(DynamicBezierPath(sample, origin.x, origin.y, start.x, start.y));
}

void Enemy::render(sf::RenderTarget & target) {
    target.draw(this->sprite);
}

void Enemy::move() {
//---    Working with scripted path
//    sf::Vector2f direction = this->currentPath->getPath().at(this->currentPoint) - this->sprite.getPosition();
//    this->updateRotation(direction.x, direction.y);
//    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//    float movement = this->velocity * static_cast<float>(moveClock.restart().asMilliseconds());
//    if (distance <= movement) {
//        this->currentPoint++;
//        if (this->currentPoint >= this->currentPath->getPath().size()){
//            this->currentPoint = 0;
//        }
//    } else {
//        this->sprite.move((direction / distance) * movement);
//    }
//---

//---    Working with dynamic path
    this->dynamicPath->setOrigin(this->dynamicPath->getOrigin().x + (rand() % 5 - 2), 0.f);

    sf::Vector2f direction = this->dynamicPath->getOldPoint() - this->sprite.getPosition();
    this->setWantedRotation(direction.x, direction.y);
    float distanceNormalize = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float movement = this->velocity * static_cast<float>(moveClock.restart().asMilliseconds());

    if (distanceNormalize <= movement) {

//        auto directionToOrigin= this->sprite.getPosition() - this->dynamicPath->getOrigin();
//        float distanceToOrigin = std::sqrt(directionToOrigin.x * directionToOrigin.x + directionToOrigin.y * directionToOrigin.y);

//        auto tmpDist = distanceToOrigin / this->dynamicPath->getDistance();
//        if (tmpDist > 0.05f) {

        if (this->dynamicPath->getCurrentSample() == static_cast<unsigned>(std::ceil(static_cast<int>(this->dynamicPath->getFullSample()) / 2.))) {
            auto tmp_en = this->sprite.getPosition();
            auto tmp_dp = this->dynamicPath->getOrigin();

            BezierCurve curve(tmp_en, tmp_en, tmp_dp, tmp_dp);
            this->dynamicPath->setOldCurve(curve);
            this->dynamicPath->setFullSample(this->dynamicPath->getCurrentSample());
            this->dynamicPath->updateDelta();
        }

        sf::Vector2<float> newPoint;

        if (this->dynamicPath->calculateNewPoint(newPoint)) {
            this->dynamicPath->setOldPoint(newPoint);
        }
    } else {
        this->sprite.move((direction / distanceNormalize) * movement);
    }
//---
}

void Enemy::updateAttack() {

}

void Enemy::updateRotation() {
    float rotationDifference = this->wantedRotation - this->sprite.getRotation();

    float maxRotation = this->rotationVelocity * static_cast<float>(rotationClock.restart().asSeconds());

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
    this->updateAttack();
    this->move();
    this->updateRotation();
}

bool Enemy::canAttack() {
    if (this->shootClock.getElapsedTime() > this->enemyShootCooldown) {
        this->shootClock.restart();
        return true;
    }
    return false;
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
    this->wantedRotation = angle;
}

void Enemy::setPosition(float & x, float & y) {
    this->sprite.setPosition(x, y);
}

void Enemy::setPath(std::shared_ptr<BezierPath> & path) {
    this->currentPath = path;
    this->currentPoint = 0;
}

sf::FloatRect Enemy::getGlobalBounds() {
    return this->sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getLocalBounds() {
    return this->sprite.getLocalBounds();
}