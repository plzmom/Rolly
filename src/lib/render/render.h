#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int texture;
    int useTexture;
    float color[4];
    int indexCount;
    float x, y;
    float alphaCutoff;
    int hasCollider;
    float colliderW;
    float colliderH;
    float colliderOx;
    float colliderOy;
    float vx, vy;
} Object;

void ColorBG(float r, float g, float b, float a);

unsigned int CreateStandartShader(void);

void update_body(unsigned int id, float g);

unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize,
                          const char *texturePath, float r, float g, float b, float a);

void SetObjectPosition(unsigned int objectId, float x, float y);

int CheckObjectCollision(unsigned int objectA, unsigned int objectB);
int MoveObject(unsigned int objectId, float dx, float dy);

void SetObjectCollisionEnabled(unsigned int objectId, int enable);

void SetObjectAlphaCutoff(unsigned int objectId, float cutoff);
void SetObjectRemoveBackground(unsigned int objectId, int enable);

void DrawAll(void);

void DeleteTrash(void);

#ifdef __cplusplus
}
#endif

#endif