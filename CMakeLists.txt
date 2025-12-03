cmake_minimum_required(VERSION 3.15)
project(PablosElMultiverso 
    VERSION 0.4.0 
    DESCRIPTION "Es un juego estilo vampire survivor desarrollado en C++ con la libreria Raylib."
    HOMEPAGE_URL "https://github.com/pgb60-ua/PablosElMultiverso"
    LANGUAGES CXX C
)

# ============================================================================
# CONFIGURACIÓN DEL PROYECTO
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Flags de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -fPIC -O3")

# Usar ccache si está disponible
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
endif()

# ============================================================================
# DIRECTORIOS
# ============================================================================

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(VENDOR_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/include")
set(VENDOR_LIB "${CMAKE_SOURCE_DIR}/vendor/lib")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

message("${INCLUDE_DIR}")

# ============================================================================
# ARCHIVOS FUENTE
# ============================================================================

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Recoger automáticamente todos los subdirectorios de include
file(GLOB_RECURSE INCLUDE_SUBDIRS LIST_DIRECTORIES true "${INCLUDE_DIR}/*")
set(ALL_INCLUDE_DIRS ${INCLUDE_DIR})
foreach(SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${SUBDIR})
        list(APPEND ALL_INCLUDE_DIRS ${SUBDIR})
    endif()
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

message("${ALL_INCLUDE_DIRS}")

# ============================================================================
# EJECUTABLE
# ============================================================================

add_executable(pablos-el-multiverso)

target_sources(pablos-el-multiverso PRIVATE ${SOURCES})


add_custom_command(TARGET pablos-el-multiverso POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        $<TARGET_FILE_DIR:pablos-el-multiverso>/assets
    COMMENT "Copiando assets al directorio de build..."
)

# Directorios de include
target_include_directories(pablos-el-multiverso PRIVATE
    ${ALL_INCLUDE_DIRS}
    ${VENDOR_INCLUDE}
)
# ============================================================================
# DEPENDENCIAS Y LINKADO
# ============================================================================

include(FetchContent)

# Buscar raylib instalada en el sistema
find_package(raylib)

# Si no se encuentra, descargarla automáticamente
if(NOT raylib_FOUND)
    message(STATUS "Raylib no encontrada. Descargando automáticamente...")
    
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib
        GIT_TAG 5.5
    )
    
    FetchContent_MakeAvailable(raylib)
    message(STATUS "Raylib descargada e integrada correctamente.")
else()
    message(STATUS "Raylib ya está instalada en el sistema.")
endif()

# Librerías del sistema y linkado con raylib
target_link_libraries(pablos-el-multiverso PRIVATE
    raylib
)
