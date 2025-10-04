#pragma once

#include "Geometry.hpp"
#include "Stats.hpp"
#include <vector>
extern "C" {
#include "raylib.h"
}

// Clase que representa lo mas abstracto de un personaje y enemigo que tienen
// común
class AEntity {
protected:
  /// @brief Vector de texturas de una la entidad
  std::vector<Texture2D *> textures;

  /// @brief Hitbox de la entidad (rectangulo - circulo - triangulo)
  Shape hitbox;

  /// @brief Variable para almacenar todas las stats de una entidad
  Stats stats;

  AEntity(Stats stats, const Shape &hitbox, std::vector<Texture2D *> textures);

public:
  /*--------------------------*/
  // Hitbox
  /*--------------------------*/

  /// @brief Devuelve la hitbox de la entidad
  Shape GetHitbox() { return hitbox; };

  /// @brief Establece la hitbox de la entidad a un rectangulo
  void SetRectangleHitbox(Rectangle rectangle) {
    hitbox.type = SHAPE_RECTANGLE;
    hitbox.data.rectangle = rectangle;
  };

  /// @brief Establece la hitbox de la entidad a un triangulo
  void SetTriangleHitbox(Vector2 v1, Vector2 v2, Vector2 v3) {
    hitbox.type = SHAPE_TRIANGLE;
    hitbox.data.triangle = {v1, v2, v3};
  };

  /// @brief Establece la hitbox de la entidad a un circulo
  void SetCircleHitbox(Vector2 center, float radius) {
    hitbox.type = SHAPE_CIRCLE;
    hitbox.data.circle = {center, radius};
  };

  void Render();
  virtual void TakeDamage(float amount) = 0;
  virtual void Update() = 0;
  virtual ~AEntity() { textures.clear(); };
};
