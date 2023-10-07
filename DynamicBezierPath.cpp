//
// Created by SoSunDook on 03.10.2023.
//

#include "DynamicBezierPath.h"
#include <iostream>
#include <valarray>

DynamicBezierPath::DynamicBezierPath(unsigned sample, float originPosX, float originPosY, float startPosX, float startPosY) {
    this->fullSample = sample;
    this->currentSample = this->fullSample;
    this->delta = 1.f / static_cast<float>(this->fullSample);
    this->t = 0.f;
    this->origin.x = originPosX;
    this->origin.y = originPosY;
    this->oldPoint = {startPosX, startPosY};
    auto direction = this->origin - this->oldPoint;
    this->distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2<float> p1 = this->oldPoint;
    sf::Vector2<float> p2 = this->origin;
    this->oldCurve = BezierCurve(this->oldPoint, p1, p2, origin);
}

float & DynamicBezierPath::getDistance() {
    return this->distance;
}

void DynamicBezierPath::setFullSample(unsigned int newSample) {
    this->fullSample = newSample;
}

unsigned & DynamicBezierPath::getFullSample() {
    return this->fullSample;
}

unsigned & DynamicBezierPath::getCurrentSample() {
    return this->currentSample;
}

void DynamicBezierPath::updateDelta() {
    this->delta = 1.f / static_cast<float>(this->fullSample);
    this->t = 0.f;
}

void DynamicBezierPath::setOldCurve(BezierCurve newCurve) {
    this->oldCurve = newCurve;
}

void DynamicBezierPath::setOldPoint(sf::Vector2<float> newOldPoint) {
    this->oldPoint = newOldPoint;
}

sf::Vector2<float> & DynamicBezierPath::getOldPoint() {
    return oldPoint;
}

void DynamicBezierPath::setOrigin(float originX, float originY) {
    this->origin.x = originX;
    this->origin.y = originY;
}

sf::Vector2<float> & DynamicBezierPath::getOrigin() {
    return origin;
}

bool DynamicBezierPath::calculateNewPoint(sf::Vector2<float> & newPoint) {
    if (!this->currentSample) {
        return false;
    }
    this->t += this->delta;
    newPoint = oldCurve.calculateCurvePoint(this->t);
    --this->currentSample;
    return true;
}
