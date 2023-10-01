//
// Created by SoSunDook on 29.09.2023.
//

#ifndef GALAGA_BEZIERPATH_H
#define GALAGA_BEZIERPATH_H
#include "BezierCurve.h"
#include <vector>
#include <memory>
#include <cmath>

class BezierPath {
private:
    std::vector<BezierCurve> curves;
    std::vector<int> samples;

    std::vector<sf::Vector2<float>> path;
public:
    BezierPath() = default;
    ~BezierPath() = default;

    void addCurve(BezierCurve & curve, int & sample);
    void makePath();
    std::vector<sf::Vector2<float>> & getPath();
};


#endif //GALAGA_BEZIERPATH_H
