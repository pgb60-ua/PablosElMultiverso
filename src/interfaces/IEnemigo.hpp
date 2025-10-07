#pragma once

#include "AEntity.hpp"
extern "C" {
	#include "raylib.h"
}

/// @brief Interfaz que representa el comportamiento de un enemigo en el juego.
/// Los enemigos son entidades que atacan al jugador, se mueven con IA y
/// sueltan botín al morir
class IEnemigo : public AEntity {
protected:
	/// @brief Velocidad de movimiento del enemigo
	float velocidadMovimiento;

	/// @brief Cantidad de Pablo Coins que suelta al morir
	int pabloCoinsAlMorir;

	/// @brief Indica si el enemigo está activo en la escena
	bool activo;

	/// @brief Objetivo actual del enemigo (posición del jugador)
	Vector2 objetivo;

	/// @brief Contador de tiempo para actualizar la IA
	float tiempoActualizacionIA;

	/// @brief Constructor protegido para clases derivadas
	IEnemigo(Stats stats, const Shape &hitbox,
		std::vector<Texture2D *> textures,
		float velocidadMovimiento, int pabloCoinsAlMorir);

	/// @brief Calcula si el ataque del enemigo es crítico
	/// @return true si el ataque es crítico, false en caso contrario
	bool EsAtaqueCritico() const = 0;
public:
	/*--------------------------*/
	// Movimiento e IA
	/*--------------------------*/

	/// @brief Mueve el enemigo hacia su objetivo
	/// @param deltaTime Tiempo transcurrido desde el último frame
	virtual void Mover(float deltaTime) = 0;

	/// @brief Establece la posición objetivo del enemigo (generalmente el jugador)
	/// @param nuevoObjetivo Posición hacia donde debe moverse el enemigo
	void SetObjetivo(Vector2 nuevoObjetivo) { objetivo = nuevoObjetivo; }

	/// @brief Obtiene el objetivo actual del enemigo
	Vector2 GetObjetivo() const { return objetivo; }

	/*--------------------------*/
	// Combate
	/*--------------------------*/

	/// @brief Realiza un ataque al jugador
	/// @return Daño infligido por el ataque
	virtual float Atacar() = 0;

	/// @brief Recibe daño de una fuente externa
	/// @param amount Cantidad de daño a recibir
	void TakeDamage(float amount) override = 0;

	/*--------------------------*/
	// Botín y Economía
	/*--------------------------*/

	/// @brief Suelta Pablo Coins al morir
	/// @return Cantidad de Pablo Coins que suelta
	virtual int SoltarBotin() const = 0;

	/// @brief Establece la cantidad de Pablo Coins que suelta al morir
	void SetPabloCoinsAlMorir(int cantidad) { pabloCoinsAlMorir = cantidad; }

	/// @brief Obtiene la cantidad de Pablo Coins que suelta al morir
	int GetPabloCoinsAlMorir() const { return pabloCoinsAlMorir; }

	/*--------------------------*/
	// Colisiones
	/*--------------------------*/

	/// @brief Verifica si el enemigo colisiona con una forma
	/// @param forma Forma con la que verificar la colisión
	/// @return true si hay colisión, false en caso contrario
	virtual bool VerificarColision(const Shape &forma) const = 0;

	/// @brief Maneja la lógica cuando ocurre una colisión con el jugador
	virtual void AlColisionarConJugador() = 0;

	/*--------------------------*/
	// Estado y Propiedades
	/*--------------------------*/

	/// @brief Obtiene la velocidad de movimiento del enemigo
	float GetVelocidadMovimiento() const { return velocidadMovimiento; }

	/// @brief Establece la velocidad de movimiento del enemigo
	void SetVelocidadMovimiento(float velocidad) {
		velocidadMovimiento = velocidad;
	}

	/// @brief Destructor virtual
	virtual ~IEnemigo() = default;
};