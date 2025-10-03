#pragma once

#include "raylib.h"

/// @brief Enumeracion del tipo de figuras disponibles para una entidad
typedef enum { SHAPE_RECTANGLE, SHAPE_CIRCLE, SHAPE_TRIANGLE } ShapeType;

/// @brief Struct que representa un circulo
typedef struct {
  float x, y, radius;
} Circle;

/// @brief Struct que representa un triangulo
typedef struct {
  Vector2 v1, v2, v3;
} Triangle;

/// @brief Union para almacenar el tipo de figura
typedef union {
  Rectangle rectangle;
  Circle circle;
  Triangle triangle;
} ShapeData;

/// @brief Struct que representa una figura
typedef struct {
  ShapeType type;
  ShapeData data;
} Shape;
