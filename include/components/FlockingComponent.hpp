#pragma once

typedef struct
{
    // Radios de percepción
    float perceptionRadius; // 150.0f
    float separationRadius; // 50.0f

    // Pesos de las fuerzas
    float separationWeight; // 1.5f
    float alignmentWeight;  // 1.0f
    float cohesionWeight;   // 0.5f
    float targetWeight;     // 2.0f

    float maxForceMultiplier; // 2.0f
} FlockingComponent;
