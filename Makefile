# ============================================================================
# COLORES PARA SALIDA EN CONSOLA
# ============================================================================

GREEN := $(shell printf "\033[1;32m")
YELLOW := $(shell printf "\033[1;33m")
BLUE := $(shell printf "\033[1;34m")
RED := $(shell printf "\033[1;31m")
RESET := $(shell printf "\033[0m")



# ============================================================================
# CONFIGURACIÓN DE COMPILACIÓN
# ============================================================================

# Compilador
CXX := g++

# Caché de compilación
CCACHE := ccache

# Flags de compilación base
CXXFLAGS := -std=c++17   # Usa el estándar de C++17
CXXFLAGS += -Wall        # Activa la mayoría de advertencias del compilador
CXXFLAGS += -pedantic    # Fuerza estricta conformidad al estándar C++
CXXFLAGS += -fPIC        # Genera código independiente de posición (para librerías compartidas)
CXXFLAGS += -O3          # Nivel máximo de optimización

DEPS_FLAGS := -MMD -MP
# Paralelización automática
MAKEFLAGS += --jobs=$(shell nproc 2>/dev/null || echo 4)

# ============================================================================
# DIRECTORIOS Y ARCHIVOS
# ============================================================================

SRC_DIR := src
INCLUDE_DIR := include
VENDOR_INCLUDE := vendor/include
VENDOR_LIB := vendor/lib
BUILD_DIR := build
# Archivos fuente
SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Directorios de include
INCLUDE_DIRS := $(shell find $(INCLUDE_DIR) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS)) -I $(VENDOR_INCLUDE)

# Flags del linker
LDFLAGS := -L $(VENDOR_LIB)
LDLIBS := -lraylib -lGL -lm -lpthread -lrt -lX11

# Target
TARGET := game-dev

# ============================================================================
# DIRECTORIOS DE INSTALACION
# ============================================================================

APP_NAME := pablos-el-multiverso
PREFIX ?= /usr
BINDIR := $(PREFIX)/bin
LIBDIR := $(PREFIX)/lib/$(APP_NAME)
DATADIR := $(PREFIX)/share/$(APP_NAME)
ASSETS_DIR := assets

# ============================================================================
# Regla principal
all: check-raylib $(TARGET)

$(TARGET): $(OBJS) | check-raylib
	$(info $(GREEN)Enlazando $@...$(RESET))
	$(CCACHE) $(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	$(info $(GREEN)Compilación completada: $@$(RESET))

# Regla para compilar archivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CCACHE) $(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) $(DEPS_FLAGS) -c $< -o $@

# Con las DEPS_FLAGS, generamos archivos .d para las dependencias y con este include las añade de verdad
-include $(OBJS:.o=.d)

# Crear directorios si no existen
$(BUILD_DIR):
	@mkdir -p $@

# Regla de instalación
install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(LIBDIR)
	install -m 0755 $(TARGET) $(DESTDIR)$(BINDIR)/$(APP_NAME)
	install -d $(DESTDIR)$(DATADIR)/assets
	if [ -d "$(ASSETS_DIR)" ]; then cp -r $(ASSETS_DIR)/* $(DESTDIR)$(DATADIR)/assets/; fi
	install -d $(DESTDIR)$(DATADIR)/share/locale
	if [ -d "locale" ]; then cp -r locale/* $(DESTDIR)$(DATADIR)/share/locale/; fi

# Regla de distribución
dist:
	@dpkg-buildpackage -us -uc -b
# ============================================================================
# REGLAS DE LIMPIEZA Y UTILIDAD
# ============================================================================

# Limpiar archivos compilados
clean:
	@$(info $(RED)Limpiando archivos compilados...$(RESET))
	@rm -rf $(BUILD_DIR) $(TARGET)*
	@$(info $(GREEN)Limpieza completada$(RESET))

# Recompilar todo desde cero
rebuild:
	$(MAKE) clean
	$(MAKE) all

# Ejecutar el juego en modo release
run: $(TARGET)
	@./$(TARGET)

run_FR: $(TARGET) msgfmt_FR
	@LANGUAGE=fr_FR LANG=fr_FR.UTF-8 ./$(TARGET)

run_ES: $(TARGET) msgfmt_ES
	@LANGUAGE=es_ES LANG=es_ES.UTF-8 ./$(TARGET)

# Mostrar información de compilación
info:
	$(info ╔════════════════════════════════════════════════╗)
	$(info ║       $(BLUE)Información del Proyecto$(RESET)                 ║)
	$(info ╠════════════════════════════════════════════════╣)
	$(info ║ $(BLUE)Target:$(RESET) $(TARGET))
	$(info ║ $(BLUE)Compilador:$(RESET) $(CXX))
	$(info ║ $(BLUE)CXXFLAGS:$(RESET) $(CXXFLAGS))
	$(info ║ $(BLUE)Archivos fuente:$(RESET) $(words $(SRCS)))
	$(info ║ $(BLUE)Directorios de include:$(RESET) $(words $(INCLUDE_DIRS)))
	$(info ║ $(BLUE)Dependencias externas:$(RESET) $(RAYLIB_DEP))
	$(info ╚════════════════════════════════════════════════╝)

# Ver estadísticas de compilación
stats:
	$(info Estadísticas del Proyecto:)
	$(info   Líneas de código: $(shell find $(SRC_DIR) -name "*.cpp" -o -name "*.hpp" | xargs wc -l | tail -1))
	$(info   Archivos fuente: $(words $(SRCS)))
	$(info   Archivos header: $(shell find $(INCLUDE_DIR) -name "*.hpp" | wc -l))
	$(info   Tamaño ejecutable: $(shell ls -lh $(TARGET) 2>/dev/null | awk '{print $$5}' || echo 'no compilado'))

clean-cache:
	ccache --clear
	ccache --zero-stats


# ============================================================================
# DEPENDENCIAS EXTERNAS
# ============================================================================

RAYLIB := libraylib.a
RAYLIB_DEP := $(VENDOR_LIB)/$(RAYLIB)

# Verificar e instalar raylib si es necesario
check-raylib: | $(VENDOR_LIB)
	@if [ ! -f "$(RAYLIB_DEP)" ]; then \
		echo "$(YELLOW)Raylib no está instalada. Descargando...$(RESET)"; \
		git clone --depth 1 https://github.com/raysan5/raylib.git; \
		$(MAKE) -C raylib/src/ PLATFORM=PLATFORM_DESKTOP; \
		mkdir -p $(VENDOR_LIB); \
		mv raylib/src/libraylib.a $(VENDOR_LIB)/; \
		rm -rf raylib; \
		echo "$(GREEN)Raylib instalada correctamente.$(RESET)"; \
	else \
		echo "$(GREEN)Raylib ya está instalada.$(RESET)"; \
	fi

$(VENDOR_LIB):
	@mkdir -p $(VENDOR_LIB)

# ============================================================================
# LOCALIZACIÓN CON GETTEXT
# ============================================================================


pot:
	@mkdir -p po
	xgettext --keyword=_ --keyword=N_ --from-code=UTF-8 --output=po/pablos.pot $(SRCS)

msginit_ES:
	msginit -i po/pablos.pot -l es_ES -o po/es_ES.po

msginit_FR:
	msginit -i po/pablos.pot -l fr_FR -o po/fr_FR.po

msgfmt_FR:
	@mkdir -p locale/fr_FR/LC_MESSAGES
	msgfmt po/fr_FR.po -o locale/fr_FR/LC_MESSAGES/pablos.mo
msgfmt_ES:
	@mkdir -p locale/es_ES/LC_MESSAGES
	msgfmt po/es_ES.po -o locale/es_ES/LC_MESSAGES/pablos.mo

# Declarar reglas que no son archivos
.PHONY: all clean rebuild run info stats clean-cache check-raylib pot msginit_FR msginit_ES msgfmt_FR msgfmt_ES
