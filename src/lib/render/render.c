#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "render.h"
#include <stdio.h>
#include <string.h>

static const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\n";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uColor;\n"
"uniform int uUseTexture;\n"
"uniform int uUseAlphaCutoff;\n"
"uniform float uAlphaThreshold;\n"
"void main()\n"
"{\n"
"    vec4 color = uUseTexture == 1 ? texture(uTexture, TexCoord) : uColor;\n"
"    if (uUseAlphaCutoff == 1 && color.a < uAlphaThreshold)\n"
"        discard;\n"
"    FragColor = color;\n"
"}\n";

static unsigned int VAO, VBO, EBO;
static unsigned int shaderProgram;
static unsigned int textureID;
static int useTexture = 0;
static int useAlphaCutoff = 0;
static float alphaThreshold = 0.1f;
static int colorLocation = -1;
static int useTextureLocation = -1;
static int useAlphaCutoffLocation = -1;
static int alphaThresholdLocation = -1;

void ColorBG(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int InitEP(float *vertices,
                    size_t verticesSize,
                    unsigned int *indices,
                    size_t indicesSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

static void FillCharacterImage(unsigned char *data, int width, int height)
{
    const unsigned char transparent[4] = {0, 0, 0, 0};
    const unsigned char body[4] = {200, 120, 60, 255};
    const unsigned char hair[4] = {80, 40, 20, 255};
    const unsigned char eye[4] = {0, 0, 0, 255};

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char *pixel = data + 4 * (y * width + x);
            memcpy(pixel, transparent, 4);

            int cx = x - width / 2;
            int cy = y - 18;
            int head = cx * cx + cy * cy;
            int inHead = head < 180;
            int inBody = x >= 22 && x < 42 && y >= 24 && y < 46;
            int inArm = ((x >= 16 && x < 24) || (x >= 40 && x < 48)) && y >= 26 && y < 38;
            int inLeg = ((x >= 24 && x < 30) || (x >= 34 && x < 40)) && y >= 46 && y < 62;

            if (inHead)
            {
                memcpy(pixel, body, 4);
                if ((x == 24 || x == 25) && (y == 18 || y == 19))
                    memcpy(pixel, eye, 4);
                if ((x == 38 || x == 39) && (y == 18 || y == 19))
                    memcpy(pixel, eye, 4);
                if (y < 16)
                    memcpy(pixel, hair, 4);
            }
            else if (inBody || inArm || inLeg)
            {
                memcpy(pixel, body, 4);
            }
        }
    }
}

void CreateCharacterTexture(void)
{
    const int width = 64;
    const int height = 64;
    static unsigned char imageData[64 * 64 * 4];

    FillCharacterImage(imageData, width, height);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateTextureFromFile(const char *path)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (!data)
    {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

unsigned int CreateStandartShader(void)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    int texLoc = glGetUniformLocation(shaderProgram, "uTexture");
    if (texLoc != -1)
        glUniform1i(texLoc, 0);

    colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    useTextureLocation = glGetUniformLocation(shaderProgram, "uUseTexture");
    useAlphaCutoffLocation = glGetUniformLocation(shaderProgram, "uUseAlphaCutoff");
    alphaThresholdLocation = glGetUniformLocation(shaderProgram, "uAlphaThreshold");

    if (useTextureLocation != -1)
        glUniform1i(useTextureLocation, useTexture);
    if (useAlphaCutoffLocation != -1)
        glUniform1i(useAlphaCutoffLocation, useAlphaCutoff);
    if (alphaThresholdLocation != -1)
        glUniform1f(alphaThresholdLocation, alphaThreshold);

    return shaderProgram;
}

void SetColors(float r, float g, float b, float a)
{
    glUseProgram(shaderProgram);
    if (colorLocation != -1)
        glUniform4f(colorLocation, r, g, b, a);
}

void SetTextureEnabled(int enabled)
{
    useTexture = enabled ? 1 : 0;
    glUseProgram(shaderProgram);
    if (useTextureLocation != -1)
        glUniform1i(useTextureLocation, useTexture);
}

void SetAlphaCutoffEnabled(int enabled)
{
    useAlphaCutoff = enabled ? 1 : 0;
    glUseProgram(shaderProgram);
    if (useAlphaCutoffLocation != -1)
        glUniform1i(useAlphaCutoffLocation, useAlphaCutoff);
}

void SetAlphaThreshold(float threshold)
{
    alphaThreshold = threshold;
    glUseProgram(shaderProgram);
    if (alphaThresholdLocation != -1)
        glUniform1f(alphaThresholdLocation, alphaThreshold);
}

void Draw(void)
{
    glUseProgram(shaderProgram);
    if (useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DeleteTrash(void)
{
    glDeleteTextures(1, &textureID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}