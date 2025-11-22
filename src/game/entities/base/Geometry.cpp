#include "Geometry.hpp"
#include <cmath>
#include <raymath.h>

// Función auxiliar para proyectar esquinas sobre un eje
static void ProjectOntoAxis(Vector2 corners[4], Vector2 axis, float *min, float *max)
{
    *min = *max = Vector2DotProduct(corners[0], axis);

    for (int i = 1; i < 4; i++)
    {
        float projection = Vector2DotProduct(corners[i], axis);
        if (projection < *min)
            *min = projection;
        if (projection > *max)
            *max = projection;
    }
}

Vector2 getShapePosition(const Shape &shape)
{
    switch (shape.type)
    {
    case SHAPE_CIRCLE:
        return shape.data.circle.center;
    case SHAPE_RECTANGLE:
        return {shape.data.rectangle.x, shape.data.rectangle.y};
    case SHAPE_ROTATED_RECTANGLE:
        return {shape.data.rotatedRectangle.x + shape.data.rotatedRectangle.width / 2.0f,
                shape.data.rotatedRectangle.y + shape.data.rotatedRectangle.height / 2.0f};
    default:
        break;
    }

    return {-1, -1};
}

void setShapePosition(Shape &shape, const Vector2 &position)
{
    switch (shape.type)
    {
    case SHAPE_CIRCLE:
        shape.data.circle.center = position;
        break;
    case SHAPE_RECTANGLE:
        shape.data.rectangle.x = position.x;
        shape.data.rectangle.y = position.y;
        break;
    case SHAPE_ROTATED_RECTANGLE:
        shape.data.rotatedRectangle.x = position.x - shape.data.rotatedRectangle.width / 2.0f;
        shape.data.rotatedRectangle.y = position.y - shape.data.rotatedRectangle.height / 2.0f;
        break;
    default:
        break;
    }
}

bool checkCollisionShapes(const Shape &shape1, const Shape &shape2)
{
    // Colisión círculo - círculo
    if (shape1.type == SHAPE_CIRCLE && shape2.type == SHAPE_CIRCLE)
    {
        return CheckCollisionCircles(
            shape1.data.circle.center,
            shape1.data.circle.radius,
            shape2.data.circle.center,
            shape2.data.circle.radius);
    }
    // Colisión círculo - rectángulo
    else if (shape1.type == SHAPE_CIRCLE && shape2.type == SHAPE_RECTANGLE)
    {
        return CheckCollisionCircleRec(
            shape1.data.circle.center,
            shape1.data.circle.radius,
            shape2.data.rectangle);
    }
    // Colisión rectángulo - círculo
    else if (shape1.type == SHAPE_RECTANGLE && shape2.type == SHAPE_CIRCLE)
    {
        return CheckCollisionCircleRec(
            shape2.data.circle.center,
            shape2.data.circle.radius,
            shape1.data.rectangle);
    }
    // Colisión rectángulo - rectángulo
    else if (shape1.type == SHAPE_RECTANGLE && shape2.type == SHAPE_RECTANGLE)
    {
        return CheckCollisionRecs(
            shape1.data.rectangle,
            shape2.data.rectangle);
    }
    // Colisión rectángulo rotado - círculo
    else if (shape1.type == SHAPE_ROTATED_RECTANGLE && shape2.type == SHAPE_CIRCLE)
    {
        Vector2 center = {shape1.data.rotatedRectangle.x + shape1.data.rotatedRectangle.width / 2.0f,
                          shape1.data.rotatedRectangle.y + shape1.data.rotatedRectangle.height / 2.0f};
        return CheckCollisionRotatedRectWithCircle(
            center,
            shape1.data.rotatedRectangle.width,
            shape1.data.rotatedRectangle.height,
            shape1.data.rotatedRectangle.rotation,
            shape2.data.circle.center,
            shape2.data.circle.radius);
    }
    // Colisión círculo - rectángulo rotado
    else if (shape1.type == SHAPE_CIRCLE && shape2.type == SHAPE_ROTATED_RECTANGLE)
    {
        Vector2 center = {shape2.data.rotatedRectangle.x + shape2.data.rotatedRectangle.width / 2.0f,
                          shape2.data.rotatedRectangle.y + shape2.data.rotatedRectangle.height / 2.0f};
        return CheckCollisionRotatedRectWithCircle(
            center,
            shape2.data.rotatedRectangle.width,
            shape2.data.rotatedRectangle.height,
            shape2.data.rotatedRectangle.rotation,
            shape1.data.circle.center,
            shape1.data.circle.radius);
    }
    // Colisión rectángulo rotado - rectángulo
    else if (shape1.type == SHAPE_ROTATED_RECTANGLE && shape2.type == SHAPE_RECTANGLE)
    {
        Vector2 center = {shape1.data.rotatedRectangle.x + shape1.data.rotatedRectangle.width / 2.0f,
                          shape1.data.rotatedRectangle.y + shape1.data.rotatedRectangle.height / 2.0f};
        return CheckCollisionRotatedRectWithRect(
            center,
            shape1.data.rotatedRectangle.width,
            shape1.data.rotatedRectangle.height,
            shape1.data.rotatedRectangle.rotation,
            shape2.data.rectangle);
    }
    // Colisión rectángulo - rectángulo rotado
    else if (shape1.type == SHAPE_RECTANGLE && shape2.type == SHAPE_ROTATED_RECTANGLE)
    {
        Vector2 center = {shape2.data.rotatedRectangle.x + shape2.data.rotatedRectangle.width / 2.0f,
                          shape2.data.rotatedRectangle.y + shape2.data.rotatedRectangle.height / 2.0f};
        return CheckCollisionRotatedRectWithRect(
            center,
            shape2.data.rotatedRectangle.width,
            shape2.data.rotatedRectangle.height,
            shape2.data.rotatedRectangle.rotation,
            shape1.data.rectangle);
    }

    return false;
}

void GetRotatedRectCorners(Vector2 center, float width, float height, float rotationDeg, Vector2 corners[4])
{
    float rad = rotationDeg * DEG2RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    // Local corners (before rotation)
    Vector2 local[4] = {
        {-halfW, -halfH}, // Top-left
        {halfW, -halfH},  // Top-right
        {halfW, halfH},   // Bottom-right
        {-halfW, halfH}   // Bottom-left
    };

    // Rotate and translate to world space
    for (int i = 0; i < 4; i++)
    {
        corners[i].x = center.x + (local[i].x * cosA - local[i].y * sinA);
        corners[i].y = center.y + (local[i].x * sinA + local[i].y * cosA);
    }
}

bool CheckCollisionRotatedRectWithRect(Vector2 center, float width, float height, float rotationDeg, Rectangle rect)
{
    Vector2 corners1[4], corners2[4];
    GetRotatedRectCorners(center, width, height, rotationDeg, corners1);

    // Get corners of the non-rotated rectangle
    corners2[0] = {rect.x, rect.y};
    corners2[1] = {rect.x + rect.width, rect.y};
    corners2[2] = {rect.x + rect.width, rect.y + rect.height};
    corners2[3] = {rect.x, rect.y + rect.height};

    // Test axes from both rectangles
    Vector2 axes[4];

    // Get axes from rotated rect (perpendicular to edges)
    axes[0] = Vector2Subtract(corners1[1], corners1[0]);
    axes[1] = Vector2Subtract(corners1[2], corners1[1]);

    // Get axes from regular rect
    axes[2] = Vector2Subtract(corners2[1], corners2[0]);
    axes[3] = Vector2Subtract(corners2[2], corners2[1]);

    // Test each axis using Separating Axis Theorem (SAT)
    for (int i = 0; i < 4; i++)
    {
        Vector2 axis = Vector2Normalize(axes[i]);

        float min1, max1, min2, max2;
        ProjectOntoAxis(corners1, axis, &min1, &max1);
        ProjectOntoAxis(corners2, axis, &min2, &max2);

        // Check for separation
        if (max1 < min2 || max2 < min1)
        {
            return false; // Separation found, no collision
        }
    }

    return true; // No separation found, collision detected
}

bool CheckCollisionRotatedRectWithCircle(Vector2 center, float width, float height, float rotationDeg, Vector2 circleCenter, float radius)
{
    // Transform circle center to rectangle's local space
    float rad = rotationDeg * DEG2RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    float dx = circleCenter.x - center.x;
    float dy = circleCenter.y - center.y;
    float localX = dx * cosA + dy * sinA;
    float localY = -dx * sinA + dy * cosA;

    // Find closest point on rectangle to circle center
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    float closestX = fmaxf(-halfW, fminf(halfW, localX));
    float closestY = fmaxf(-halfH, fminf(halfH, localY));

    // Calculate distance from circle center to closest point
    float distX = localX - closestX;
    float distY = localY - closestY;
    float distanceSquared = distX * distX + distY * distY;

    return distanceSquared < (radius * radius);
}