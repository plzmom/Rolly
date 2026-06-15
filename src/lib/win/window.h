#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef GLFWwindow Window;

#define loop(window)                                              \
    while (!glfwWindowShouldClose(window))                        \
        for (int _once = 1; _once;                                \
             glfwSwapBuffers(window), glfwPollEvents(), _once = 0)

Window *create_window(int w, int h, const char *t);

void MakeContext(Window *window);

void Destroy(Window *window);

void Terminate(void);

int errorGLAD(Window *window);

#endif