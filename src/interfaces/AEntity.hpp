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
private:
  static constexpr float MIN_HEALTH = 1.0f;
  static constexpr float MIN_REGENERATION = 0.0f;

protected:
  /// @brief Vector de texturas de una la entidad
  std::vector<Texture2D *> textures;

  /// @brief La entidad puede disparar?
  bool canShoot;

  /// @brief Hitbox de la entidad (rectangulo - circulo - triangulo)
  Shape hitbox;

  /// @brief Variable para almacenar todas las stats de una entidad
  Stats stats;

  /// @brief Propiedad autocalculada, tiempo que ha de pasar para volver atacar
  float attackCooldown;

  AEntity(Stats stats, bool canShoot, const Shape &hitbox,
          std::vector<Texture2D *> textures);

public:
  /// @brief Obtiene el tiempo de recarga del ataque actual
  float GetAttackCooldown() const { return attackCooldown; }
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
  /// @brief Devuelve si la entidad tiene mas de 0 de vida
  bool IsAlive();
  /// @brief Devuelve si la entidad tiene mas de 0 de velocidad de ataque
  bool CanAttack();
  virtual void TakeDamage(float amount) = 0;
  virtual void Update() = 0;
  virtual ~AEntity() { textures.clear(); };
};
