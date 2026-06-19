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
    int hasTrigger;
    float triggerW;
    float triggerH;
    float triggerOx;
    float triggerOy;
    float vx, vy;
} Object;

void ColorBG(float r, float g, float b, float a);

unsigned int CreateStandartShader(void);

extern float g_frame_dt;

void update_body(unsigned int id, float g);

unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize,
                          const char *texturePath, float r, float g, float b, float a);

void SetObjectPosition(unsigned int objectId, float x, float y);

int CheckObjectCollision(unsigned int objectA, unsigned int objectB);
int MoveObject(unsigned int objectId, float dx, float dy);

void SetObjectCollisionEnabled(unsigned int objectId, int enable, float colliderW, float colliderH, float colliderOx, float colliderOy);

void SetObjectAlphaCutoff(unsigned int objectId, float cutoff);
void SetObjectRemoveBackground(unsigned int objectId, int enable);

/* Set a small extra margin applied to collision checks so objects don't end up exactly flush.
 * Pass 0.0f to disable the margin. Default is 0.02f in the implementation.
 */
void SetCollisionMargin(float margin);

/* Trigger (non-blocking area) API:
 * - `SetObjectTriggerEnabled` configures an object's trigger area (does not block movement).
 * - `CheckObjectTrigger` returns non-zero when `objectId` overlaps the trigger defined by `triggerId`.
 */
void SetObjectTriggerEnabled(unsigned int objectId, int enable, float triggerW, float triggerH, float triggerOx, float triggerOy);
int CheckObjectTrigger(unsigned int triggerId, unsigned int objectId);

void DrawAll(void);

void DeleteTrash(void);

#ifdef __cplusplus
}
#endif

#endif