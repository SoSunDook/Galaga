//
// Created by SoSunDook on 03.10.2023.
//

#ifndef GALAGA_DYNAMICBEZIERPATH_H
#define GALAGA_DYNAMICBEZIERPATH_H
#include "BezierCurve.h"
#include <list>

class DynamicBezierPath {
private:
    unsigned fullSample;
    unsigned currentSample;

    float delta;

    float t;

    sf::Vector2<float> origin;

    float distance;

    BezierCurve oldCurve;
    sf::Vector2<float> oldPoint;
public:
    explicit DynamicBezierPath(unsigned sample, float originPosX, float originPosY, float startPosX, float startPosY);
    ~DynamicBezierPath() = default;

    float & getDistance();

    void setFullSample(unsigned newSample);

    unsigned & getFullSample();
    unsigned & getCurrentSample();

    void updateDelta();

    void setOldCurve(BezierCurve newCurve);

    void setOldPoint(sf::Vector2<float> newOldPoint);
    sf::Vector2<float> & getOldPoint();

    void setOrigin(float originX, float originY);
    sf::Vector2<float> & getOrigin();

    bool calculateNewPoint(sf::Vector2<float> & newPoint);
};

#endif //GALAGA_DYNAMICBEZIERPATH_H
