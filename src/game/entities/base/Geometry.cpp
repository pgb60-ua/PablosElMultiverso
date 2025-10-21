#include "Geometry.hpp"

Vector2 getShapePosition(const Shape &shape)
{
    switch (shape.type)
    {
    case SHAPE_CIRCLE:
        return shape.data.circle.center;
    case SHAPE_RECTANGLE:
        return {shape.data.rectangle.x, shape.data.rectangle.y};
    default:
        break;
    }

    return {-1, -1};
}

void setShapePosition(Shape &shape, const Vector2 &position)
{
    switch (shape.type)
    {
    case SHAPE_CIRCLE:
        shape.data.circle.center = position;
        break;
    case SHAPE_RECTANGLE:
        shape.data.rectangle.x = position.x;
        shape.data.rectangle.y = position.y;
        break;
    default:
        break;
    }
}