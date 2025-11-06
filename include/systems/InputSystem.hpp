#pragma once
#include <EnTT/entt.hpp>
extern "C"
{
#include <raylib.h>
}

typedef struct
{
    int up, down, left, right;
} KeyBindings;

class InputSystem
{
private:
    // Configuración de teclas por jugador
    std::unordered_map<int, KeyBindings> m_playerBindings = {{1, {KEY_W, KEY_S, KEY_A, KEY_D}},
                                                             {2, {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT}},
                                                             {3, {KEY_I, KEY_K, KEY_J, KEY_L}},
                                                             {4, {KEY_KP_8, KEY_KP_5, KEY_KP_4, KEY_KP_6}}};

public:
    InputSystem() {};
    ~InputSystem() {};
    void Update(entt::registry &registry);
};
