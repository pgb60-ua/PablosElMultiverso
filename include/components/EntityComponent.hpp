#pragma once

typedef enum
{
    ALIVE,
    DEAD,
    SHOP
} EntityState;

typedef struct
{
    EntityState state;
} EntityComponent;
