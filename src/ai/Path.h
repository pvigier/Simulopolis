#pragma once

#include <vector>
#include "util/Vector.h"

class Path
{
public:
    Path();
    Path(std::vector<Vector2f> points, bool loop = false);

    Vector2f getCurrentPoint() const;
    void setNextPoint();
    bool isFinished() const;

private:
    std::vector<Vector2f> mPoints;
    std::size_t mCurPoint;
    bool mLoop;
};
