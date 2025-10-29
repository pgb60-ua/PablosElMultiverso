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

Vector2 getShapePosition(const Shape &shape);

void setShapePosition(Shape &shape, const Vector2 &position);

bool checkCollisionShapes(const Shape &shape1, const Shape &shape2);