find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

# Usar vendor para nlohmann_json y spdlog
set(VENDOR_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/include")


# DataFileManager Test
add_executable(DataFileManager_test DataFileManager_test.cpp
    ${CMAKE_SOURCE_DIR}/src/common/DataFileManager.cpp
    ${CMAKE_SOURCE_DIR}/src/game/entities/base/Stats.cpp
)

target_link_libraries(DataFileManager_test 
    PRIVATE 
    Boost::unit_test_framework
)

target_include_directories(DataFileManager_test 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/game/entities
    ${CMAKE_SOURCE_DIR}/include/game/entities/base
    ${CMAKE_SOURCE_DIR}/include/game/shop
    ${CMAKE_SOURCE_DIR}/include/game/rounds
    ${VENDOR_INCLUDE}
)

add_test(NAME DataFileManagerTest COMMAND DataFileManager_test)

# SpriteLoaderManager Test
add_executable(SpriteLoaderManager_test SpriteLoaderManager_test.cpp
    ${CMAKE_SOURCE_DIR}/src/common/SpriteLoaderManager.cpp
    ${CMAKE_SOURCE_DIR}/src/game/entities/base/Geometry.cpp
)

target_link_libraries(SpriteLoaderManager_test 
    PRIVATE 
    Boost::unit_test_framework
    raylib
)

target_include_directories(SpriteLoaderManager_test 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/game/entities/base
    ${CMAKE_SOURCE_DIR}/include/utils
    ${VENDOR_INCLUDE}
)

add_test(NAME SpriteLoaderManagerTest COMMAND SpriteLoaderManager_test)