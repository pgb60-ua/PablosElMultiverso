
#include <vector>
extern "C" {
#include "raylib.h"
}

// Clase que representa lo mas abstracto de un personaje y enemigo que tienen
// común
class AEntidad {
private:
  static constexpr float VIDA_MINIMA = 1.0f;
  static constexpr float REGENERACION_MINIMA = 0.0f;
  static constexpr float MULTIPLICADOR_BASE = 1.0f;

protected:
  /// @brief Vector de texturas de una la entidad
  std::vector<Texture2D *> texturas;
  // hitbox

  // items

  /// @brief Puntos de vida de la entidad
  float vida;
  /// @brief Modificador multiplicativo de vida
  float modificadorVida = MULTIPLICADOR_BASE;

  /// @brief Cuantos pixeles se mueve por frame
  float velocidadMovimiento;
  /// @brief Modificador multiplicativo de velocidad de movimiento
  float modificadorVelocidadMovimiento = MULTIPLICADOR_BASE;

  /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
  float agilidad;
  /// @brief Modificador multiplicativo de agilidad
  float modificadorAgilidad = MULTIPLICADOR_BASE;

  /// @brief Cuantos ataques por segundo se realizan
  float velocidadAtaque;
  /// @brief Modificador multiplicativo de velocidad de ataque
  float modificadorVelocidadAtaque = MULTIPLICADOR_BASE;

  /// @brief Propiedad autocalculada, tiempo que ha de pasar para volver atacar
  float tiempoRecargaAtaque;

  /// @brief Puntos de daño de tipo físico que se infligen al atacar
  float danyoFisico;
  /// @brief Modificador multiplicativo de daño físico
  float modificadorDanyoFisico = MULTIPLICADOR_BASE;

  /// @brief Puntos de daño de tipo mágico que se infligen al atacar
  float danyoMagico;
  /// @brief Modificador multiplicativo de daño mágico
  float modificadorDanyoMagico = MULTIPLICADOR_BASE;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// físico
  float armadura;
  /// @brief Modificador multiplicativo de armadura
  float modificadorArmadura = MULTIPLICADOR_BASE;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// mágico
  float resistencia;
  /// @brief Modificador multiplicativo de resistencia mágica
  float modificadorResistencia = MULTIPLICADOR_BASE;

  /// @brief Probabilidad entre 0 - 30 de aplicar más daño físico / mágico
  float probabilidadCritico;
  /// @brief Modificador multiplicativo de probabilidad de crítico
  float modificadorProbabilidadCritico = MULTIPLICADOR_BASE;

  /// @brief Multiplicador entre 1 - 10 al daño infligido
  float danyoCritico;
  /// @brief Modificador multiplicativo de daño crítico
  float modificadorDanyoCritico = MULTIPLICADOR_BASE;

  /// @brief Porcentaje entre 0 - 50 de curación del daño al atacar
  float roboVida;
  /// @brief Modificador multiplicativo de robo de vida
  float modificadorRoboVida = MULTIPLICADOR_BASE;

  /// @brief Cantidad de puntos de vida que te curas cada segundo
  float regeneracionVida;
  /// @brief Modificador multiplicativo de regeneración de vida
  float modificadorRegeneracionVida = MULTIPLICADOR_BASE;

  AEntidad(float vida, float velocidadMovimiento, float agilidad,
           float velocidadAtaque, float danyoFisico, float danyoMagico,
           float armadura, float resistencia, float probabilidadCritico,
           float danyoCritico, float roboVida, float regeneracionVida,
           std::vector<Texture2D *> texturas);

public:
  // Getters de stats
  /// @brief Obtiene los puntos de vida actuales
  float GetVida() const { return vida; }

  /// @brief Obtiene la velocidad de movimiento actual
  float GetVelocidadMovimiento() const { return velocidadMovimiento; }

  /// @brief Obtiene la agilidad actual
  float GetAgilidad() const { return agilidad; }

  /// @brief Obtiene la velocidad de ataque actual
  float GetVelocidadAtaque() const { return velocidadAtaque; }

  /// @brief Obtiene el tiempo de recarga del ataque actual
  float GetTiempoRecargaAtaque() const { return tiempoRecargaAtaque; }

  /// @brief Obtiene el daño físico actual
  float GetDanyoFisico() const { return danyoFisico; }

  /// @brief Obtiene el daño mágico actual
  float GetDanyoMagico() const { return danyoMagico; }

  /// @brief Obtiene la armadura actual
  float GetArmadura() const { return armadura; }

  /// @brief Obtiene la resistencia mágica actual
  float GetResistencia() const { return resistencia; }

  /// @brief Obtiene la probabilidad de crítico actual
  float GetProbabilidadCritico() const { return probabilidadCritico; }

  /// @brief Obtiene el multiplicador de daño crítico actual
  float GetDanyoCritico() const { return danyoCritico; }

  /// @brief Obtiene el porcentaje de robo de vida actual
  float GetRoboVida() const { return roboVida; }

  /// @brief Obtiene la regeneración de vida por segundo actual
  float GetRegeneracionVida() const { return regeneracionVida; }

  // Getters de modificadores
  /// @brief Obtiene el modificador de vida
  float GetModificadorVida() const { return modificadorVida; }

  /// @brief Obtiene el modificador de velocidad de movimiento
  float GetModificadorVelocidadMovimiento() const {
    return modificadorVelocidadMovimiento;
  }

  /// @brief Obtiene el modificador de agilidad
  float GetModificadorAgilidad() const { return modificadorAgilidad; }

  /// @brief Obtiene el modificador de velocidad de ataque
  float GetModificadorVelocidadAtaque() const {
    return modificadorVelocidadAtaque;
  }

  /// @brief Obtiene el modificador de daño físico
  float GetModificadorDanyoFisico() const { return modificadorDanyoFisico; }

  /// @brief Obtiene el modificador de daño mágico
  float GetModificadorDanyoMagico() const { return modificadorDanyoMagico; }

  /// @brief Obtiene el modificador de armadura
  float GetModificadorArmadura() const { return modificadorArmadura; }

  /// @brief Obtiene el modificador de resistencia mágica
  float GetModificadorResistencia() const { return modificadorResistencia; }

  /// @brief Obtiene el modificador de probabilidad de crítico
  float GetModificadorProbabilidadCritico() const {
    return modificadorProbabilidadCritico;
  }

  /// @brief Obtiene el modificador de daño crítico
  float GetModificadorDanyoCritico() const { return modificadorDanyoCritico; }

  /// @brief Obtiene el modificador de robo de vida
  float GetModificadorRoboVida() const { return modificadorRoboVida; }

  /// @brief Obtiene el modificador de regeneración de vida
  float GetModificadorRegeneracionVida() const {
    return modificadorRegeneracionVida;
  }

  /**/
  // Setters de stats (aplican modificador automáticamente)

  /// @brief Establece los puntos de vida base y aplica modificador
  void SetVida(float nuevaVida) { vida = nuevaVida * modificadorVida; }

  /// @brief Establece la velocidad de movimiento base y aplica modificador
  void SetVelocidadMovimiento(float nuevaVelocidad) {
    velocidadMovimiento = nuevaVelocidad * modificadorVelocidadMovimiento;
  }

  /// @brief Establece la agilidad base y aplica modificador
  void SetAgilidad(float nuevaAgilidad) {
    agilidad = nuevaAgilidad * modificadorAgilidad;
  }

  /// @brief Establece la velocidad de ataque base, aplica modificador y
  /// recalcula el tiempo de recarga
  void SetVelocidadAtaque(float nuevaVelocidad) {
    velocidadAtaque = nuevaVelocidad * modificadorVelocidadAtaque;
    tiempoRecargaAtaque = 1.0f / velocidadAtaque;
  }

  /// @brief Establece el daño físico base y aplica modificador
  void SetDanyoFisico(float nuevoDanyo) {
    danyoFisico = nuevoDanyo * modificadorDanyoFisico;
  }

  /// @brief Establece el daño mágico base y aplica modificador
  void SetDanyoMagico(float nuevoDanyo) {
    danyoMagico = nuevoDanyo * modificadorDanyoMagico;
  }

  /// @brief Establece la armadura base y aplica modificador
  void SetArmadura(float nuevaArmadura) {
    armadura = nuevaArmadura * modificadorArmadura;
  }

  /// @brief Establece la resistencia mágica base y aplica modificador
  void SetResistencia(float nuevaResistencia) {
    resistencia = nuevaResistencia * modificadorResistencia;
  }

  /// @brief Establece la probabilidad de crítico base y aplica modificador
  void SetProbabilidadCritico(float nuevaProbabilidad) {
    probabilidadCritico = nuevaProbabilidad * modificadorProbabilidadCritico;
  }

  /// @brief Establece el multiplicador de daño crítico base y aplica
  /// modificador
  void SetDanyoCritico(float nuevoMultiplicador) {
    danyoCritico = nuevoMultiplicador * modificadorDanyoCritico;
  }

  /// @brief Establece el porcentaje de robo de vida base y aplica modificador
  void SetRoboVida(float nuevoRobo) {
    roboVida = nuevoRobo * modificadorRoboVida;
  }

  /// @brief Establece la regeneración de vida base y aplica modificador
  void SetRegeneracionVida(float nuevaRegeneracion) {
    regeneracionVida = nuevaRegeneracion * modificadorRegeneracionVida;
  }

  /*--------------------------*/
  // Setters de modificadores
  /*--------------------------*/

  /// @brief Establece el modificador de vida
  void SetModificadorVida(float nuevoModificador) {
    modificadorVida = nuevoModificador;
  }

  /// @brief Establece el modificador de velocidad de movimiento
  void SetModificadorVelocidadMovimiento(float nuevoModificador) {
    modificadorVelocidadMovimiento = nuevoModificador;
  }

  /// @brief Establece el modificador de agilidad
  void SetModificadorAgilidad(float nuevoModificador) {
    modificadorAgilidad = nuevoModificador;
  }

  /// @brief Establece el modificador de velocidad de ataque
  void SetModificadorVelocidadAtaque(float nuevoModificador) {
    modificadorVelocidadAtaque = nuevoModificador;
  }

  /// @brief Establece el modificador de daño físico
  void SetModificadorDanyoFisico(float nuevoModificador) {
    modificadorDanyoFisico = nuevoModificador;
  }

  /// @brief Establece el modificador de daño mágico
  void SetModificadorDanyoMagico(float nuevoModificador) {
    modificadorDanyoMagico = nuevoModificador;
  }

  /// @brief Establece el modificador de armadura
  void SetModificadorArmadura(float nuevoModificador) {
    modificadorArmadura = nuevoModificador;
  }

  /// @brief Establece el modificador de resistencia mágica
  void SetModificadorResistencia(float nuevoModificador) {
    modificadorResistencia = nuevoModificador;
  }

  /// @brief Establece el modificador de probabilidad de crítico
  void SetModificadorProbabilidadCritico(float nuevoModificador) {
    modificadorProbabilidadCritico = nuevoModificador;
  }

  /// @brief Establece el modificador de daño crítico
  void SetModificadorDanyoCritico(float nuevoModificador) {
    modificadorDanyoCritico = nuevoModificador;
  }

  /// @brief Establece el modificador de robo de vida
  void SetModificadorRoboVida(float nuevoModificador) {
    modificadorRoboVida = nuevoModificador;
  }

  /// @brief Establece el modificador de regeneración de vida
  void SetModificadorRegeneracionVida(float nuevoModificador) {
    modificadorRegeneracionVida = nuevoModificador;
  }

  void Renderizar();
  virtual void RecibirDanyo(int cantidad);
  virtual void Actualizar();
  virtual bool EstaVivo();
  virtual ~AEntidad();
};
