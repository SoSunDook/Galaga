//
// Created by SoSunDook on 10.10.2023.
//

#ifndef GALAGA_FORMATION_H
#define GALAGA_FORMATION_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Formation {
private:
    sf::Vector2<float> position;

    sf::Clock clock;

    float offsetAmount;
    float offsetTimer;
    float offsetDelay;
    int offsetCounter;
    int offsetDirection;

    float spreadTimer;
    float spreadDelay;
    int spreadCounter;
    int spreadDirection;

    sf::Vector2<float> gridSize;

    bool locked;
public:
    Formation();
    ~Formation() = default;

    sf::Vector2<float> & getPosition();
    sf::Vector2<float> & getGridSize();

    void lock();

    void update();
};


#endif //GALAGA_FORMATION_H
