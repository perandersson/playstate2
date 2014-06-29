#include "../Memory.h"
#include "Point.h"
using namespace core;

Point Point::ZERO;

Point::Point() : x(0), y(0)
{
}

Point::Point(const Point& point) : x(point.x), y(point.y)
{
}

Point::Point(int _x, int _y) : x(_x), y(_y)
{
}
