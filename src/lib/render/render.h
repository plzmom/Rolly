#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <stddef.h>

void ColorBG(float r, float g, float b, float a);

unsigned int InitEP(float *vertices,
                    size_t verticesSize,
                    unsigned int *indices,
                    size_t indicesSize);

unsigned int CreateStandartShader(void);

void CreateCharacterTexture(void);

void CreateTextureFromFile(const char *path);

void SetColors(float r, float g, float b, float a);

void SetTextureEnabled(int enabled);

void SetAlphaCutoffEnabled(int enabled);

void SetAlphaThreshold(float threshold);

void Draw(void);

void DeleteTrash(void);

#endif