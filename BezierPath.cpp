//
// Created by SoSunDook on 29.09.2023.
//

#include "BezierPath.h"
#include <iostream>

void BezierPath::addCurve(BezierCurve & curve, int & sample) {
    this->curves.push_back(curve);
    this->samples.push_back(sample);
}

void BezierPath::makePath() {
    for (auto i = 0; i < this->curves.size(); ++i) {
        auto delta = 1.f / static_cast<float>(samples[i]);
        for (float t = 0.f; t <= 1.f; t += delta) {
            path.push_back(this->curves[i].calculateCurvePoint(t));
        }
    }
//    Deallocating memory
    this->curves = std::vector<BezierCurve>();
    this->samples = std::vector<int>();
}

std::vector<sf::Vector2<float>> & BezierPath::getPath() {
    return path;
}
