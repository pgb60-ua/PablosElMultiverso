#pragma once

#include "SpriteAnimation.hpp"
extern "C"
{
#include <raylib.h>
}

typedef struct
{
    // Texture2D texture;
    float rotation;
} RenderItemComponent;

typedef struct
{
    float angle;
    SpriteAnimation animation;
} RenderEntityComponent;
