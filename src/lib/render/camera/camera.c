#include "camera.h"

static float camX = 0.0f;
static float camY = 0.0f;

void CameraSet(float x, float y)
{
    camX = x;
    camY = y;
}

float CameraX(void)
{
    return camX;
}

float CameraY(void)
{
    return camY;
}