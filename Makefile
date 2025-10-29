# ============================================================================
# CONFIGURACIÓN DE COMPILACIÓN
# ============================================================================

# Compilador
CXX := g++

# Flags de compilación base
CXXFLAGS := -std=c++17 -Wall -pedantic
CXXFLAGS += -fPIC -O3 -DNDEBUG -march=native

# Dependencias automáticas
CXXFLAGS += -MMD -MP

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
DEP_DIR := $(BUILD_DIR)/.deps

# Archivos fuente
SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(SRCS:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

# Directorios de include
INCLUDE_DIRS := $(shell find $(INCLUDE_DIR) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS)) -I $(VENDOR_INCLUDE)

# Flags del linker
LDFLAGS := -L $(VENDOR_LIB)
LDLIBS := -lraylib -lGL -lm -lpthread -lrt -lX11

# Target
TARGET := game-dev

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJS)
	$(info Enlazando $@...)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	$(info ✓ Compilación completada: $@)

# Regla para compilar archivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR) $(DEP_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@
	@cp $(BUILD_DIR)/$*.d $(DEP_DIR)/$*.d 2>/dev/null || true

# Crear directorios si no existen
$(BUILD_DIR) $(DEP_DIR):
	@mkdir -p $@

# Incluir archivos de dependencias
-include $(DEPS)

# ============================================================================
# REGLAS DE LIMPIEZA Y UTILIDAD
# ============================================================================

# Limpiar archivos compilados
clean:
	@$(info Limpiando archivos compilados...)
	@rm -rf $(BUILD_DIR) $(TARGET)*
	@$(info ✓ Limpieza completada)

# Recompilar todo desde cero
rebuild: 
    $(MAKE) clean 
    $(MAKE) all

# Ejecutar el juego en modo release
run: $(TARGET)
	@./$(TARGET)

# Mostrar información de compilación
info:
	$(info ╔════════════════════════════════════════════════╗)
	$(info ║       Información del Proyecto                 ║)
	$(info ╠════════════════════════════════════════════════╣)
	$(info ║ Target: $(TARGET))
	$(info ║ Compilador: $(CXX))
	$(info ║ CXXFLAGS: $(CXXFLAGS))
	$(info ║ Archivos fuente: $(words $(SRCS)))
	$(info ║ Directorios de include: $(words $(INCLUDE_DIRS)))
	$(info ╚════════════════════════════════════════════════╝)

# Ver estadísticas de compilación
stats:
	$(info Estadísticas del Proyecto:)
	$(info   Líneas de código: $(shell find $(SRC_DIR) -name "*.cpp" -o -name "*.hpp" | xargs wc -l | tail -1))
	$(info   Archivos fuente: $(words $(SRCS)))
	$(info   Archivos header: $(shell find $(INCLUDE_DIR) -name "*.hpp" | wc -l))
	$(info   Tamaño ejecutable: $(shell ls -lh $(TARGET) 2>/dev/null | awk '{print $$5}' || echo 'no compilado'))

# Declarar reglas que no son archivos
.PHONY: all clean rebuild run info stats