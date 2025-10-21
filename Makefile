# Makefile para PablosElMultiverso

# Compilador y banderas
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3
TARGET := game-dev

# Directorios
SRC_DIR := src
INCLUDE_DIR := include
VENDOR_INCLUDE := vendor/include
VENDOR_LIB := vendor/lib
BUILD_DIR := build

# Encontrar todos los archivos fuente y generar objetos
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Directorios de include
INCLUDE_FLAGS := $(shell find $(INCLUDE_DIR) -type d -exec printf -- "-I %s " {} \;)
INCLUDE_FLAGS += -I $(VENDOR_INCLUDE)

# Flags del linker
LDFLAGS := -L $(VENDOR_LIB)
LDLIBS := -lraylib -lGL -lm -lpthread -lrt -lX11

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# Regla para compilar archivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	@rm -rf $(BUILD_DIR) $(TARGET)

# Recompilar todo desde cero
rebuild: clean all

# Ejecutar el juego
run: $(TARGET)
	@./$(TARGET)

# Mostrar información de compilación
info:
	@echo "Información del proyecto:"
	@echo "  Target: $(TARGET)"
	@echo "  Compilador: $(CXX)"
	@echo "  Flags: $(CXXFLAGS)"
	@echo "  Archivos fuente encontrados: $(words $(SRCS))"
	@echo "  Directorios de include: $(INCLUDE_FLAGS)"

# Declarar reglas que no son archivos
.PHONY: all clean rebuild run info