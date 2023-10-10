//
// Created by SoSunDook on 10.10.2023.
//

#include "Formation.h"

Formation::Formation() {
    this->position = {360.f, 124.f};

    this->clock = {};

    this->offsetAmount = 20.f;
    this->offsetTimer = 0.f;
    this->offsetDelay = 1.f;
    this->offsetCounter = 4;
    this->offsetDirection = 1;

    this->spreadTimer = 0.f;
    this->spreadDelay = 1.f;
    this->spreadCounter = 0 ;
    this->spreadDirection = 1;

    this->gridSize = {25.f, 42.f};

    this->locked = false;
}

sf::Vector2<float> & Formation::getPosition() {
    return this->position;
}

sf::Vector2<float> & Formation::getGridSize() {
    return this->gridSize;
}

void Formation::lock() {
    this->locked = true;
}

void Formation::update() {
    if ((!this->locked) || (this->offsetCounter != 4)) {
        this->offsetTimer += this->clock.restart().asSeconds();
        if (this->offsetTimer >= this->offsetDelay) {
            this->offsetCounter++;

            sf::Vector2<float> right{1, 0};
            this->position += right * static_cast<float>(this->offsetDirection) * this->offsetAmount;

            if (this->offsetCounter == 8) {
                this->offsetCounter = 0;
                this->offsetDirection *= -1;
            }

            this->offsetTimer = 0.f;
        }
    }
}
