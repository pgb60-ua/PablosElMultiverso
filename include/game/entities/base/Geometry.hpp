#pragma once

extern "C"
{
#include <raylib.h>
}

/// @brief Enumeracion del tipo de figuras disponibles para una entidad
typedef enum
{
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE
} ShapeType;

/// @brief Struct que representa un circulo
typedef struct
{
    Vector2 center;
    float radius;
} Circle;

/// @brief Union para almacenar el tipo de figura
typedef union
{
    Rectangle rectangle;
    Circle circle;
} ShapeData;

/// @brief Struct que representa una figura
typedef struct
{
    ShapeType type;
    ShapeData data;
} Shape;

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