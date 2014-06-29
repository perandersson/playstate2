#include "../Memory.h"
#include "Rect.h"
using namespace core;

Rect core::Rect::ZERO;

Rect::Rect()
: x(0), y(0), width(0), height(0)
{
}

Rect::Rect(const Rect& rect)
: position(rect.position), size(rect.size)
{
}

Rect::Rect(const Point& position, const Size& size)
: position(position), size(size)
{
}

Rect::Rect(int32 _x, int32 _y, int32 _width, int32 _height)
: position(_x, _y), size(_width, _height)
{
}
