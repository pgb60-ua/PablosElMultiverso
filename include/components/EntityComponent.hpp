#pragma once

typedef enum
{
    ALIVE,
    DEAD,
    SHOP,
    DAMAGED,
    CRITIC
} EntityState;

typedef struct
{
    EntityState state;
} EntityComponent;
