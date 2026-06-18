#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render.h"



static Object *objects = NULL;
static size_t objectsCount = 0;

static unsigned int shader;

static int object_collision(unsigned int a, unsigned int b)
{
    if (a >= objectsCount || b >= objectsCount)
        return 0;

    const Object *o1 = &objects[a];
    const Object *o2 = &objects[b];

    if (!o1->hasCollider || !o2->hasCollider)
        return 0;

    float o1x = o1->x + o1->colliderOx;
    float o1y = o1->y + o1->colliderOy;
    float o2x = o2->x + o2->colliderOx;
    float o2y = o2->y + o2->colliderOy;

    return o1x - o1->colliderW * 0.5f < o2x + o2->colliderW * 0.5f &&
           o1x + o1->colliderW * 0.5f > o2x - o2->colliderW * 0.5f &&
           o1y - o1->colliderH * 0.5f < o2y + o2->colliderH * 0.5f &&
           o1y + o1->colliderH * 0.5f > o2y - o2->colliderH * 0.5f;
}


float g_frame_dt = 1.0f/60.0f;

void update_body(unsigned int id, float g)
{
    if (id >= objectsCount) return;
    Object *b = &objects[id];

    float dt = g_frame_dt;

    b->vy += g * dt;

    float dx = b->vx * dt;
    float dy = b->vy * dt;

    MoveObject(id, dx, dy);
}

static const char *vs =
"#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec2 aUV;\n"
"uniform vec2 offset;\n"
"out vec2 UV;\n"
"void main(){\n"
"UV=aUV;\n"
"gl_Position=vec4(aPos.xy+offset,aPos.z,1.0);\n"
"}\n";

static const char *fs =
"#version 330 core\n"
"in vec2 UV;\n"
"uniform sampler2D tex;\n"
"uniform int useTex;\n"
"uniform vec4 color;\n"
"uniform float alphaCutoff;\n"
"out vec4 FragColor;\n"
"void main(){\n"
"    if(useTex==1){\n"
"        vec4 col = texture(tex,UV);\n"
"        if(col.a < alphaCutoff) discard;\n"
"        FragColor = col;\n"
"    } else {\n"
"        FragColor = color;\n"
"    }\n"
"}\n";


static unsigned int compile(unsigned int t, const char *s) {
    unsigned int sh = glCreateShader(t);
    glShaderSource(sh, 1, &s, NULL);
    glCompileShader(sh);
    return sh;
}

unsigned int CreateStandartShader(void) {
    unsigned int v = compile(GL_VERTEX_SHADER, vs);
    unsigned int f = compile(GL_FRAGMENT_SHADER, fs);

    shader = glCreateProgram();
    glAttachShader(shader, v);
    glAttachShader(shader, f);
    glLinkProgram(shader);

    glDeleteShader(v);
    glDeleteShader(f);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);

    return shader;
}

unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize,
                          const char *texturePath,
                          float r, float g, float b, float a)
{
    Object o = {0};

    o.color[0] = r;
    o.color[1] = g;
    o.color[2] = b;
    o.color[3] = a;

    if (texturePath) {
        int w,h,c;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *data =
            stbi_load(texturePath,&w,&h,&c,STBI_rgb_alpha);

        if (!data) return -1;

        o.useTexture = 1;

        glGenTextures(1,&o.texture);
        glBindTexture(GL_TEXTURE_2D,o.texture);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    glGenVertexArrays(1,&o.vao);
    glGenBuffers(1,&o.vbo);
    glGenBuffers(1,&o.ebo);

    glBindVertexArray(o.vao);

    glBindBuffer(GL_ARRAY_BUFFER,o.vbo);
    glBufferData(GL_ARRAY_BUFFER,verticesSize,vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,o.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indicesSize,indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    o.indexCount = indicesSize / sizeof(unsigned int);
    o.x = 0;
    o.y = 0;
    o.alphaCutoff = 0.0f;

    if (vertices && verticesSize >= 5 * sizeof(float)) {
        size_t count = verticesSize / (5 * sizeof(float));
        float minX = vertices[0];
        float maxX = vertices[0];
        float minY = vertices[1];
        float maxY = vertices[1];

        for (size_t i = 1; i < count; i++) {
            float x = vertices[i * 5 + 0];
            float y = vertices[i * 5 + 1];

            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }

        o.colliderW = maxX - minX;
        o.colliderH = maxY - minY;
        o.colliderOx = (minX + maxX) * 0.5f;
        o.colliderOy = (minY + maxY) * 0.5f;
        o.hasCollider = 1;
    }

    Object *tmp = realloc(objects,(objectsCount+1)*sizeof(Object));
    if (!tmp) return -1;

    objects = tmp;
    objects[objectsCount] = o;

    return objectsCount++;
}

void SetObjectPosition(unsigned int id, float x, float y) {
    if (id >= objectsCount) return;
    objects[id].x = x;
    objects[id].y = y;
}

void DrawAll(void) {
    glUseProgram(shader);
    int dbg = getenv("DEBUG_OBJECTS") != NULL;
    if (dbg) fprintf(stderr, "DrawAll: objectsCount=%zu\n", objectsCount);

    for (size_t i = 0; i < objectsCount; i++) {
        Object *o = &objects[i];
        if (dbg) {
            fprintf(stderr, "Obj %zu: x=%f y=%f vx=%f vy=%f color=%f,%f,%f,%f useTex=%d hasCollider=%d\n",
                    i, o->x, o->y, o->vx, o->vy,
                    o->color[0], o->color[1], o->color[2], o->color[3],
                    o->useTexture, o->hasCollider);
        }

        glUniform4f(glGetUniformLocation(shader,"color"),
                    o->color[0],o->color[1],o->color[2],o->color[3]);

        glUniform1i(glGetUniformLocation(shader,"useTex"),o->useTexture);

        if (o->useTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,o->texture);
        }

        glUniform2f(glGetUniformLocation(shader,"offset"),o->x,o->y);
        glUniform1f(glGetUniformLocation(shader,"alphaCutoff"), o->alphaCutoff);

        glBindVertexArray(o->vao);
        glDrawElements(GL_TRIANGLES,o->indexCount,GL_UNSIGNED_INT,0);
    }
}

int CheckObjectCollision(unsigned int a, unsigned int b) {
    return object_collision(a, b);
}

void SetObjectCollisionEnabled(unsigned int id, int enable) {
    if (id >= objectsCount)
        return;

    objects[id].hasCollider = enable ? (objects[id].colliderW > 0.0f && objects[id].colliderH > 0.0f) : 0;
}

int MoveObject(unsigned int id, float dx, float dy) {
    if (id >= objectsCount)
        return 0;

    if (!objects[id].hasCollider) {
        objects[id].x += dx;
        objects[id].y += dy;
        return 1;
    }

    float oldX = objects[id].x;
    float oldY = objects[id].y;
    int moved = 0;
    int collision_blocked = 0;

    if (dx != 0.0f) {
        objects[id].x += dx;
        for (size_t i = 0; i < objectsCount; i++) {
            if (i == id || !objects[i].hasCollider)
                continue;

            if (object_collision(id, i)) {
                objects[id].x = oldX;
                collision_blocked = 1;
                break;
            }
        }
        if (objects[id].x != oldX)
            moved = 1;
    }

    if (dy != 0.0f) {
        objects[id].y += dy;
        for (size_t i = 0; i < objectsCount; i++) {
            if (i == id || !objects[i].hasCollider)
                continue;

            if (object_collision(id, i)) {
                objects[id].y = oldY;
                collision_blocked = 1;
                break;
            }
        }
        if (objects[id].y != oldY)
            moved = 1;
    }

    if (collision_blocked && !moved)
        return 2; /* movement blocked by collision */

    return moved;
}

void SetObjectAlphaCutoff(unsigned int id, float cutoff) {
    if (id >= objectsCount) return;
    objects[id].alphaCutoff = cutoff;
}

void SetObjectRemoveBackground(unsigned int id, int enable) {
    if (id >= objectsCount) return;
    objects[id].alphaCutoff = enable ? 0.1f : 0.0f;
}

void ColorBG(float r,float g,float b,float a) {
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DeleteTrash(void) {
    for (size_t i=0;i<objectsCount;i++) {
        glDeleteVertexArrays(1,&objects[i].vao);
        glDeleteBuffers(1,&objects[i].vbo);
        glDeleteBuffers(1,&objects[i].ebo);
        if (objects[i].useTexture)
            glDeleteTextures(1,&objects[i].texture);
    }

    free(objects);
    glDeleteProgram(shader);
    objects=NULL;
    objectsCount=0;
}