//
// Created by SoSunDook on 29.09.2023.
//

#ifndef GALAGA_BEZIERCURVE_H
#define GALAGA_BEZIERCURVE_H
#include <SFML/Graphics.hpp>

class BezierCurve {
public:
    sf::Vector2<float> p0;
    sf::Vector2<float> p1;
    sf::Vector2<float> p2;
    sf::Vector2<float> p3;

    explicit BezierCurve(sf::Vector2<float> & p0, sf::Vector2<float> & p1, sf::Vector2<float> & p2, sf::Vector2<float> & p3);
    ~BezierCurve() = default;

    sf::Vector2<float> calculateCurvePoint(float t);
};


#endif //GALAGA_BEZIERCURVE_H
