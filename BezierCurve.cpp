//
// Created by SoSunDook on 29.09.2023.
//

#include "BezierCurve.h"

BezierCurve::BezierCurve(sf::Vector2<float> p0, sf::Vector2<float> p1, sf::Vector2<float> p2, sf::Vector2<float> p3) {
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}

sf::Vector2<float> BezierCurve::calculateCurvePoint(float t) {
    float tt = t * t;
    float ttt = tt * t;
    float u = 1.f - t;
    float uu = u * u;
    float uuu = uu * u;
    return (uuu * p0) + (3.f * uu * t * p1) + (3.f * u * tt * p2) + (ttt * p3);
}