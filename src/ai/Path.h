#pragma once

#include <vector>
#include "util/Vector.h"

class Path
{
public:
    Path();
    Path(std::vector<Vector2f> points, bool loop = false);

    Vector2f getCurrentPoint() const;
    const std::vector<Vector2f> getPoints() const;
    void setNextPoint();
    bool isFinished() const;
    bool isEmpty() const;

private:
    std::vector<Vector2f> mPoints;
    std::size_t mCurPoint;
    bool mLoop;
};
