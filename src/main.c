#include <stdio.h>

#include "lib/UI/ui.h"
#include "lib/win/window.h"
#include "lib/render/render.h"


int main(void)
{
    Window *win = create_window(1920, 1080, "Title");

    if (!win)
        return -1;

    MakeContext(win);

    if (errorGLAD(win) != 0)
        return -1;

    UI_Init(win);
    CreateStandartShader();

    float vertices[] = {
        -0.12f, -0.24f, 0.0f, 0.0f, 0.0f,
         0.12f, -0.24f, 0.0f, 1.0f, 0.0f,
         0.12f,  0.24f, 0.0f, 1.0f, 1.0f,
        -0.12f,  0.24f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

// Тепер кожна вершина має 5 значень: X, Y, Z, U, V
    float floorVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Нижня ліва
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Нижня права
         1.0f, -0.8f, 0.0f, 1.0f, 1.0f,  // Верхня права
        -1.0f, -0.8f, 0.0f, 0.0f, 1.0f   // Верхня ліва
    };

    unsigned int floorIndices[] = {
        0, 1, 2,
        2, 3, 0
    };


    unsigned int obj1 =
        CreateObject(vertices, sizeof(vertices),
                     indices, sizeof(indices),
                     NULL,
                     1.0f, 1.0f, 0.0f, 1.0f);

    unsigned int obj2 =
        CreateObject(vertices, sizeof(vertices),
                     indices, sizeof(indices),
                     NULL,
                     0.0f, 0.0f, 1.0f, 1.0f);
    unsigned int floorObj = CreateObject(floorVertices, sizeof(floorVertices),
                                        floorIndices, sizeof(floorIndices),
                                    NULL, 0.0f, 1.0f, 0.0f, 1.0f);

    if (obj1 == (unsigned int)-1 || obj2 == (unsigned int)-1) {
        fprintf(stderr, "Failed to create object(s). Check texture paths.\n");
        Destroy(win);
        Terminate();
        return -1;
    }


    SetObjectCollisionEnabled(obj2, 1);
    SetObjectCollisionEnabled(obj1, 1);
    SetObjectCollisionEnabled(floorObj, 1);

    SetObjectPosition(obj1, 0.0f, 0.0f);
    SetObjectPosition(obj2, 0.8f, 0.0f);

    SetObjectRemoveBackground(obj1, 1);
    SetObjectRemoveBackground(obj2, 1);

    int wasHit = 0;

    loop(win)
    {
        float dx = 0.0f;
        float dy = 0.0f;
        float dx2 = 0.0f;
        float dy2 = 0.0f;

        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            dx -= 0.01f;

        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            dx += 0.01f;

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            dy += 0.01f;

        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            dy -= 0.01f;

        update_body(obj1, -0.5f);
        update_body(obj2, -0.5f);
        
        int res1 = MoveObject(obj1, dx, dy);
        int res2 = MoveObject(obj2, dx2, dy2);

        if (res1 == 2 || res2 == 2 || CheckObjectCollision(obj1, obj2)) {
            if (!wasHit) {
                printf("Hit\n");
                wasHit = 1;
            }
        } else {
            wasHit = 0;
        }

        ColorBG(0.0f, 0.0f, 0.0f, 1.0f);
        DrawAll();

        UI_NewFrame();
        if (UI_BeginWindow("Debug")) {
            UI_Text("Was hit: %d", wasHit);
            if (UI_Button("Reset object 1")) {
                SetObjectPosition(obj1, 0.0f, 0.0f);
            }
        }
        UI_EndWindow();
        UI_Render();
    }

    UI_Shutdown();
    DeleteTrash();

    Destroy(win);
    Terminate();

    return 0;
}