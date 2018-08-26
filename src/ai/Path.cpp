#include "Path.h"

Path::Path() : mCurPoint(0), mLoop(false)
{

}

Path::Path(std::vector<Vector2f> points, bool loop) : mPoints(std::move(points)), mCurPoint(0), mLoop(loop)
{
    //ctor
}

Vector2f Path::getCurrentPoint() const
{
    return mPoints[mCurPoint];
}

const std::vector<Vector2f> Path::getPoints() const
{
    return mPoints;
}

void Path::setNextPoint()
{
    ++mCurPoint;
    if (mLoop && mCurPoint >= mPoints.size())
        mCurPoint = 0;
}

bool Path::isFinished() const
{
    return !mLoop && mCurPoint == (mPoints.size() - 1);
}

bool Path::isEmpty() const
{
    return mPoints.empty();
}
