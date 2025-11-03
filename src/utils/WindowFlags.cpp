#include "WindowFlags.hpp"
#include "raylib.h"

void HandleWindowFlags()
{
    if (IsKeyPressed(KEY_F11))
        ToggleFullscreen();
}
