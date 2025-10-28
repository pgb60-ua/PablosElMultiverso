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

bool checkCollisionShapes(const Shape &shape1, const Shape &shape2)
{
    // Colisión círculo - círculo
    if (shape1.type == SHAPE_CIRCLE && shape2.type == SHAPE_CIRCLE)
    {
        return CheckCollisionCircles(
            shape1.data.circle.center,
            shape1.data.circle.radius,
            shape2.data.circle.center,
            shape2.data.circle.radius);
    }
    // Colisión círculo - rectángulo
    else if (shape1.type == SHAPE_CIRCLE && shape2.type == SHAPE_RECTANGLE)
    {
        return CheckCollisionCircleRec(
            shape1.data.circle.center,
            shape1.data.circle.radius,
            shape2.data.rectangle);
    }
    // Colisión rectángulo - círculo
    else if (shape1.type == SHAPE_RECTANGLE && shape2.type == SHAPE_CIRCLE)
    {
        return CheckCollisionCircleRec(
            shape2.data.circle.center,
            shape2.data.circle.radius,
            shape1.data.rectangle);
    }
    // Colisión rectángulo - rectángulo
    else if (shape1.type == SHAPE_RECTANGLE && shape2.type == SHAPE_RECTANGLE)
    {
        return CheckCollisionRecs(
            shape1.data.rectangle,
            shape2.data.rectangle);
    }

    return false;
}