//
// Created by SoSunDook on 10.10.2023.
//

#ifndef GALAGA_FORMATION_H
#define GALAGA_FORMATION_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>

class Formation {
private:
    sf::Vector2<float> position;

    std::shared_ptr<sf::Time> deltaTime;

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

    bool changed;
public:
    explicit Formation(std::shared_ptr<sf::Time> & timer);
    ~Formation() = default;

    sf::Vector2<float> & getPosition();
    sf::Vector2<float> & getGridSize();

    void lock();
    bool isLocked();

    void update();

    int getTick();
    bool & changedTick();
};


#endif //GALAGA_FORMATION_H
