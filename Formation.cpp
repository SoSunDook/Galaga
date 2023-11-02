//
// Created by SoSunDook on 10.10.2023.
//

#include "Formation.h"

Formation::Formation(std::shared_ptr<sf::Time> & timer) {
    this->position = {360.f, 124.f};

    this->deltaTime = timer;

    this->offsetAmount = 20.f;
    this->offsetTimer = 0.f;
    this->offsetDelay = 0.6f;
    this->offsetCounter = 4;
    this->offsetDirection = 1;

    this->spreadTimer = 0.f;
    this->spreadDelay = 0.6f;
    this->spreadCounter = 0 ;
    this->spreadDirection = 1;

    this->gridSize = {24.f, 48.f};

    this->locked = false;

    this->changed = false;
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

bool Formation::isLocked() {
    return this->locked && this->offsetCounter == 4;
}

void Formation::update() {
    if ((!this->locked) || (this->offsetCounter != 4)) {
        this->offsetTimer += this->deltaTime->asSeconds();
        if (this->offsetTimer >= this->offsetDelay) {
            this->changed = true;
            this->offsetCounter++;

            sf::Vector2<float> right{1, 0};
            this->position += right * static_cast<float>(this->offsetDirection) * this->offsetAmount;

            if (this->offsetCounter == 8) {
                this->offsetCounter = 0;
                this->offsetDirection *= -1;
            }

            this->offsetTimer = 0.f;
        } else {
            this->changed = false;
        }
    } else {
        this->spreadTimer += this->deltaTime->asSeconds();
        if (this->spreadTimer >= this->spreadDelay) {
            this->changed = true;
            this->spreadCounter += this->spreadDirection;

            this->gridSize.x += static_cast<float>(this->spreadDirection * ((this->spreadCounter % 2 == 0) ? 1 : 2));

            if (this->spreadCounter == 4 || this->spreadCounter == 0) {
                this->spreadDirection *= -1;
            }

            this->spreadTimer = 0.f;
        } else {
            this->changed = false;
        }
    }
}

int Formation::getTick() {
    if (!this->locked || this->offsetCounter != 4) {
        return this->offsetCounter;
    }
    return this->spreadCounter;
}

bool & Formation::changedTick() {
    return this->changed;
}
