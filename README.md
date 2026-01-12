# PablosElMultiverso

## Licenses and Attributions

This project incorporates code from multiple sources. Please see [NOTICES.md](NOTICES.md) for complete attribution and licensing information.

- **Project License:** MIT License - See [LICENSE](LICENSE)
- **DCAGII Game Template:** MIT License - See [LICENSE.DCAGII-Game-Template](LICENSE.DCAGII-Game-Template)
- **raylib:** zlib/libpng License - See [LICENSE.raylib](LICENSE.raylib)
- **nlohmann/json:** MIT License - See [LICENSE.nlohmann-json](LICENSE.nlohmann-json)
- **spdlog:** MIT License - See [LICENSE.spdlog](LICENSE.spdlog)

## Estrategia de gestión de ramas

Nuestra estrategia para la gestión de ramas se basa en el modelo de **GitHub Flow**, utilizando Jira para el seguimiento de incidencias. Puedes consultar el proyecto en [este enlace de Jira](https://pablogarciabelando.atlassian.net/jira/software/projects/PEM/boards/1). Necesitas iniciar sesión para acceder.

Cada nueva rama se crea a partir de una incidencia asignada en Jira. El nombre de la rama debe seguir la convención `PEM-X-descripcion-corta`, donde `X` es el número de la incidencia y `descripcion-corta` es un resumen breve del objetivo de la rama.

Las Pull Requests (PR) se nombrarán siguiendo el formato `PEM-X | descripcion-corta`.

**Ejemplo:**

- **Rama:** `PEM-8-Inicializar-proyecto-base`
- **PR:** `PEM-8 | Inicializar proyecto base`

Todas las Pull Requests se integrarán en la rama principal mediante un **squash merge**.

## Installation and Compilation

### Windows

#### Requisitos previos

- **CMake** (versión 3.15 o superior): [Descargar CMake](https://cmake.org/download/)
- **Visual Studio** (2019 o superior) con herramientas de desarrollo de C++
- **Gettext** para internacionalización (opcional, solo necesario si quieres generar/actualizar traducciones y ejecutar el juego en varios idiomas):
    - Opción 1: Instalar via vcpkg: `vcpkg install gettext:x64-windows`
    - Opción 2: Descargar binarios desde [gnuwin32](http://gnuwin32.sourceforge.net/packages/gettext.htm)
    - Si no lo instalas, el proyecto debería compilarse igualmente, pero solo estará disponible el idioma por defecto configurado en el juego y no podrás gestionar archivos de traducción (`.po`/`.mo`).

#### Compilación con CMake y Visual Studio

Desde el directorio raíz del proyecto en PowerShell o CMD:

```bash
# Crear directorio de compilación
mkdir build
cd build

# Configurar el proyecto con CMake (detectará automáticamente la versión de Visual Studio instalada)
cmake ..

# Compilar desde línea de comandos
cmake --build . --config Release

# O abrir la solución en Visual Studio
start pablos-el-multiverso.sln

# Ejecutar el juego
Release\pablos-el-multiverso.exe
```

**Nota**: El proyecto está configurado específicamente para MSVC (Microsoft Visual C++). No es compatible con MinGW debido a flags de compilación y linkado específicos de MSVC.

### Linux (Ubuntu/Debian)

#### Requisitos previos

Instalar las dependencias necesarias:

```bash
sudo apt install xorg-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev cmake build-essential
```

#### Compilación con CMake

Desde el directorio raíz del proyecto:

```bash
# Crear directorio de compilación
mkdir -p build
cd build

# Configurar el proyecto
cmake ..

# Compilar
make

# Ejecutar el juego
./pablos-el-multiverso
```

#### Compilación con Make (alternativa)

Si prefieres usar el Makefile tradicional:

```bash
# Compilar
make

# Ejecutar el juego
./game-dev
```

**Nota**: El Makefile genera un ejecutable llamado `game-dev`, mientras que CMake genera `pablos-el-multiverso`.

## Juego

### 1. Mecánica Principal

El juego se desarrolla dentro de una sala cuadrada cerrada estática, donde el objetivo del jugador es sobrevivir y esquivar a los enemigos durante un número determinado de rondas. La acción se centra en la movilidad y la mejora progresiva del personaje.

### 2. Rondas y Progresión

- El juego tendrá varias dificultades.
- El juego está dividido en un total de **15 rondas**.
- Cada ronda tiene una duración en tiempo variable, determinada por el nivel de dificultad seleccionado.
- A medida que avanzan las rondas, los enemigos incrementan su cantidad, fuerza y complejidad en función de la dificultad.
- En las rondas **5, 10 y 15** aparecerán **jefes especiales**, que supondrán un desafío mayor y darán mejor botín.

### 3. Personajes Jugables y Estadísticas

#### 3.1. Clases Disponibles

El jugador podrá elegir entre cuatro clases principales, cada una con un estilo de juego diferenciado:

- **Bárbaro**: especializado en fuerza bruta y gran resistencia.
- **Arquero**: enfocado en la velocidad de ataque y ataques a distancia.
- **Mago**: centrado en el daño mágico y habilidades especiales.
- **Curandero**: personaje de apoyo con capacidad de regeneración y sanación.

#### 3.2. Estadísticas del Personaje

Cada clase comparte un conjunto de atributos que determinan su desempeño en combate:

- **Vida**: cantidad de daño que puede resistir antes de morir.
- **Velocidad de movimiento**: rapidez con la que se desplaza por el escenario.
- **Agilidad**: probabilidad de que al ser atacado no recibas daño.
- **Velocidad de ataque**: frecuencia con la que puede efectuar ataques.
- **Daño físico**: potencia de los ataques físicos.
- **Daño mágico**: potencia de las habilidades mágicas.
- **Resistencia**: capacidad para reducir el daño mágico recibido.
- **Armadura**: Capacidad para reducir el daño físico recibido.
- **Crítico**: probabilidad de infligir un golpe crítico con daño extra.
- **Daño crítico**: daño adicional que se inflige cuando se produce un crítico.
- **Robo de vida**: porcentaje de vida recuperada al infligir daño.
- **Regeneración de vida**: recuperación pasiva de salud a lo largo del tiempo.

### 4. Enemigos

#### 4.1. Tipos de Enemigos Básicos

Algunos ejemplos de enemigos comunes son:

- **Melee con palo de madera**: enemigo sencillo que se abalanza cuerpo a cuerpo contra el jugador.
- **Colocador de torretas**: enemigo más avanzado que evita al jugador mientras instala torretas ofensivas. Su comportamiento integra un algoritmo de machine learning, lo que le permite esquivar y adaptarse al movimiento del jugador.
- **Enemigos de botín**: enemigos que están diferenciados e intentan huir del jugador. Soltarán más botín que de normal al morir.

#### 4.2. Jefes Principales

Algunos ejemplos de jefes son:

- Un jefe con mucha salud y resistencias, que intenta a toda costa acercarse al jugador para hacerle daño. Va más rápido que el jugador pero al ser golpeado retrocede.
- Un jefe que tiene poca salud y escapa del jugador. A medida que huye va generando enemigos básicos.
- Un jefe que dispara balas en un patrón concreto y tienes que ir esquivando mientras le disparas.
- Un jefe que tiene mucha salud pero se dedica a aumentar las estadísticas de los enemigos actuales.
- Un jefe que tiene poca vida y no tiene un patrón de movimiento determinado.
    - Se mueve de forma aleatoria.
    - En caso de morir, explota matando al jugador o jugadores.
    - La manera de ganar a este jefe es no pegarle durante la ronda; si no se le ataca y se acaba el tiempo, se irá solo.

### 5. Armas, Objetos y Economía

#### 5.1. Armas

El sistema de armas se basa en apuntado y disparo automático en un rango determinado por la clase, de manera que el jugador puede centrarse en esquivar enemigos y gestionar su posición dentro del escenario.

#### 5.2. Objetos

Durante el transcurso del juego, el jugador podrá adquirir objetos que mejoren sus estadísticas mediante la tienda.

**Economía interna: Pablo Coin**

- Los enemigos derrotados dejan caer una moneda llamada **Pablo Coin**.
- La cantidad obtenida dependerá del tipo de enemigo.
- Estas monedas son el recurso principal para realizar compras dentro de la tienda.
- La tienda se abrirá después de cada ronda.

#### 5.3. Tienda

La tienda es un menú que se abrirá después de cada ronda. Esta permite comprar objetos con Pablo Coins. Permitirá bloquear objetos entre tiendas para que no desaparezcan y permitirá actualizar la tienda con un coste de monedas.

### 6. Controles

El título ofrecerá un sistema de control adaptable con varias opciones de entrada:

- **Teclado**: WASD por defecto + E para comprar + Espacio para bloquear tienda.
- **Flechas direccionales**: + K para comprar + L para bloquear tienda.
- **Puntero del ratón**: + Clic izquierdo para comprar + Clic derecho para bloquear tienda.
- **Mando**: + Botón de abajo comprar + Botón de la izquierda para bloquear tienda.

### 7. Multijugador Local

- El juego incluirá un modo de multijugador local para dos jugadores en la misma pantalla sin ser pantalla dividida durante las rondas.
- Cada jugador deberá utilizar un método de control diferente.
- La acción se compartirá en un mismo espacio de juego, fomentando la cooperación.
- Cada jugador tiene una tienda distinta, por tanto al entrar en la tienda la pantalla se divide en 2.
- Cada jugador tiene unas estadísticas personales independientes del otro jugador y un inventario independiente.
- La dificultad aumenta en modo cooperativo además de la elegida. La dificultad elegida será la misma para ambos jugadores.

### 8. Gestión de Estados

El juego contará con varios estados principales que estructuran la experiencia:

- **Pantalla principal**: menú inicial del juego donde te aparece "Solitario" o "Cooperativo".
- **Pantalla de selección de personaje**: menú donde él/los jugadores elegirán personaje y dificultad en común.
- **Pantalla de juego**: desarrollo de la partida dentro de la sala cuadrada.
- **Tienda**: espacio donde gastar Pablo Coins en objetos y mejoras.
- **Pantalla de estadísticas**: permite ver el inventario y las estadísticas de cada jugador en la partida.
- **Pantalla de Game Over**: aparece al perder la partida, mostrando estadísticas y resultados.

### 9. Roles Definidos por Personas

#### Carlos María Casado López

**Rol principal**: Mecánicas jugables y ECS

**Responsabilidades**:

- **Personaje**:
    - Movimiento
    - Inventario de armas
    - Estadísticas
    - Hitbox

- **Tienda**:
    - Mecánica de compra
    - Mecánica de reroll
    - Mecánica de bloqueo de ítem

- **Items**:
    - Stats
    - Rareza - probabilidad

#### Pablo García Belando

**Rol principal**: Gráficos

**Responsabilidades**:

- **Personaje**:
    - Sprite y animaciones
    - Sprite de estadísticas

- **Bala**:
    - Sprite y animaciones

- **Menús**:
    - Sprite y animaciones

- **Enemigos**:
    - Sprite y animaciones

- **Tienda**:
    - Sprite

- **Items**:
    - Sprite

- **Dinero**:
    - Sprite

#### Manuel José Tornero Fuster

**Rol principal**: IA (comportamiento de los NPCs u obstáculos) y Físicas

**Responsabilidades**:

- **Enemigos**:
    - Movimiento
    - IA
    - Stats
    - Drops de dinero
    - Colisiones

- **Balas**:
    - Movimiento
    - Lógica de disparo
    - Colisiones

#### Alejandro Jiménez Martínez

**Rol principal**: Sonidos y ECS

**Responsabilidades**:

- **Personaje**:
    - Efectos de sonido

- **Enemigo**:
    - Efectos de sonido

- **Balas**:
    - Efectos de sonido

- **Tienda**:
    - Efectos de sonido

- **Menús y pantallas**:
    - Música y efectos de sonido
