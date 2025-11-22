#pragma once

extern "C"
{
#include <raylib.h>
}

/// @brief Enumeracion del tipo de figuras disponibles para una entidad
typedef enum
{
    SHAPE_RECTANGLE,
    SHAPE_ROTATED_RECTANGLE,
    SHAPE_CIRCLE
} ShapeType;

/// @brief Struct que representa un circulo
typedef struct
{
    Vector2 center;
    float radius;
} Circle;

/// @brief Struct que representa un rectángulo con rotación
typedef struct
{
    float x;
    float y;
    float width;
    float height;
    float rotation; // Rotación en grados (por defecto 0.0f)
} RotatedRectangle;

/// @brief Union para almacenar el tipo de figura
typedef union
{
    Rectangle rectangle;
    RotatedRectangle rotatedRectangle;
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

/// @brief Obtiene las 4 esquinas de un rectángulo rotado
/// @param center Centro del rectángulo
/// @param width Ancho del rectángulo
/// @param height Alto del rectángulo
/// @param rotationDeg Rotación en grados
/// @param corners Array de 4 vectores donde se almacenarán las esquinas
void GetRotatedRectCorners(Vector2 center, float width, float height, float rotationDeg, Vector2 corners[4]);

/// @brief Verifica colisión entre un rectángulo rotado y un rectángulo regular
/// @param center Centro del rectángulo rotado
/// @param width Ancho del rectángulo rotado
/// @param height Alto del rectángulo rotado
/// @param rotationDeg Rotación en grados del rectángulo
/// @param rect Rectángulo regular
/// @return true si hay colisión, false en caso contrario
bool CheckCollisionRotatedRectWithRect(Vector2 center, float width, float height, float rotationDeg, Rectangle rect);

/// @brief Verifica colisión entre un rectángulo rotado y un círculo
/// @param center Centro del rectángulo rotado
/// @param width Ancho del rectángulo rotado
/// @param height Alto del rectángulo rotado
/// @param rotationDeg Rotación en grados del rectángulo
/// @param circleCenter Centro del círculo
/// @param radius Radio del círculo
/// @return true si hay colisión, false en caso contrario
bool CheckCollisionRotatedRectWithCircle(Vector2 center, float width, float height, float rotationDeg, Vector2 circleCenter, float radius);