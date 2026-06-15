#include <stdio.h>
#include <glad/glad.h>
#include "window.h"

Window *create_window(int w, int h, const char *t)
{
    if (!glfwInit())
        return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return glfwCreateWindow(w, h, t, NULL, NULL);
}

void MakeContext(Window *window)
{
    glfwMakeContextCurrent(window);
}

void Destroy(Window *window)
{
    glfwDestroyWindow(window);
}

void Terminate(void)
{
    glfwTerminate();
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

int errorGLAD(Window *window)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return 0;
}
