# OpenGL Library Demo

Ця бібліотека представляє простий OpenGL-проєкт на C з модульною архітектурою для створення вікна, ініціалізації контексту, налаштування геометрії, шейдерів та текстур.

## Огляд

Проєкт побудовано на основі:
- `GLFW` — створення вікна та керування подіями.
- `GLAD` — завантаження функцій OpenGL.
- `stb_image.h` — завантаження PNG/текстур у пам'ять.

Ціль — надати базовий OpenGL-движок для рендеру квадрата, який можна відображати однотонним кольором або текстурою.

## Структура проекту

- `Makefile` — правила для збирання програми.
- `README.md` — документація.
- `LICENSE` — ліцензія проекту.
- `src/main.c` — приклад використання бібліотеки.
- `src/lib/win/window.h` — оголошення функцій роботи з вікном.
- `src/lib/win/window.c` — реалізація створення вікна та ініціалізації GLFW.
- `src/lib/render/render.h` — оголошення функцій рендеру, шейдерів та текстур.
- `src/lib/render/render.c` — реалізація API рендеру та завантаження текстур.
- `src/lib/render/stb_image.h` — вбудований заголовок для завантаження зображень.
- `src/glad/` — GLAD loader для OpenGL.
- `src/assets/` — приклад текстури, яка використовується у додатку.

## Компоненти бібліотеки

### Модуль вікна (`src/lib/win`)

Функції:

- `void Init(void);`
  - Ініціалізує GLFW.
  - Викликається перед створенням вікна.

- `Window *create_window(int w, int h, const char *t);`
  - Створює вікно розміром `w × h` з назвою `t`.
  - Повертає `GLFWwindow *` (тип `Window *`) або `NULL` у разі помилки.

- `void MakeContext(Window *window);`
  - Робить передане вікно контекстом OpenGL.

- `int errorGLAD(Window *window);`
  - Ініціалізує GLAD.
  - У разі помилки знищує вікно та завершує GLFW, повертає `-1`.
  - Повертає `0` при успішній ініціалізації.

- `void Destroy(Window *window);`
  - Звільняє ресурси вікна GLFW.

- `void Terminate(void);`
  - Завершує GLFW.

- Макрос `loop(window)`
  - Простий цикл рендеру, який виконується поки вікно не буде закрито.
  - Приклад:

```c
loop(win)
{
    // оновлення, малювання
}
```

### Модуль рендеру (`src/lib/render`)

Функції:

- `void ColorBG(float r, float g, float b, float a);`
  - Встановлює колір очищення екрану та виконує `glClear(GL_COLOR_BUFFER_BIT)`.

- `unsigned int InitEP(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize);`
  - Ініціалізує VAO, VBO та EBO для даних вершин та індексів.
  - Повертає ідентифікатор VAO.
  - Вершини очікуються у форматі: `x, y, z, u, v`.

- `unsigned int CreateStandartShader(void);`
  - Створює та лінкує стандартні вершинний і фрагментний шейдери.
  - Фрагментний шейдер підтримує:
    - `uTexture` — текстурний силаблер.
    - `uColor` — однотонний колір.
    - `uUseTexture` — перемикач текстури.
    - `uUseAlphaCutoff` — перемикач обрізання за прозорістю.
    - `uAlphaThreshold` — поріг альфа-каналу.
  - Повертає `shaderProgram`.

- `void CreateCharacterTexture(void);`
  - Генерує вбудовану 64×64 текстуру з простим персонажем.
  - Підходить для прикладів, коли файлова текстура не потрібна.

- `void CreateTextureFromFile(const char *path);`
  - Завантажує PNG/зображення за допомогою `stb_image.h`.
  - Завантажує текстуру в OpenGL і генерує MIP-мапи.
  - Якщо файл не завантажено, виводить помилку в `stderr`.

- `void SetColors(float r, float g, float b, float a);`
  - Встановлює значення `uColor` для фрагментного шейдера.

- `void SetTextureEnabled(int enabled);`
  - Увімкнення (`1`) або вимкнення (`0`) текстурного режиму.
  - Якщо вимкнено, об’єкт рендериться однотонним кольором.

- `void SetAlphaCutoffEnabled(int enabled);`
  - Увімкнення (`1`) або вимкнення (`0`) обрізання пікселів із прозорістю нижче заданого порогу.

- `void SetAlphaThreshold(float threshold);`
  - Встановлює поріг альфа-прозорості для обрізання.

- `void Draw(void);`
  - Встановлює поточну програму шейдера та текстуру (якщо увімкнено).
  - Прорисовує елементи як два трикутники за індексами.

- `void DeleteTrash(void);`
  - Звільняє `textureID`, `VAO`, `VBO`, `EBO` та `shaderProgram`.

## Приклад використання

`src/main.c` демонструє повний робочий сценарій:

```c
#include "lib/win/window.h"
#include "lib/render/render.h"

int main()
{
    Init();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    Window *win = create_window(600, 600, "Title");
    if (win == NULL)
        return -2;

    MakeContext(win);
    if (errorGLAD(win) != 0)
        return 1;

    InitEP(vertices, sizeof(vertices), indices, sizeof(indices));
    CreateStandartShader();
    CreateTextureFromFile("src/assets/sim.png");
    SetColors(0.0f, 0.0f, 1.0f, 1.0f);
    SetTextureEnabled(1);
    SetAlphaThreshold(0.1f);
    SetAlphaCutoffEnabled(1);

    loop(win)
    {
        ColorBG(1.0f, 0.0f, 0.0f, 1.0f);
        Draw();
    }

    DeleteTrash();
    Destroy(win);
    Terminate();
    return 0;
}
```

## Побудова та запуск

### Залежності

Потрібні системні бібліотеки:
- `glfw3-dev`
- `libgl1-mesa-dev` або еквівалент OpenGL development package
- `libglu1-mesa-dev`
- `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxinerama-dev`, `libxcursor-dev`
- `libasound2-dev`, `libpulse-dev`, `libudev-dev`, `libdbus-1-dev`
- `libdl-dev`
- `gcc`
- `make`

На Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libasound2-dev libpulse-dev libudev-dev libdbus-1-dev libdl-dev
```

### Збірка

```bash
make
```

### Запуск

```bash
./app
```

## Робота з текстурами

### Включити текстуру

```c
CreateTextureFromFile("src/assets/sim.png");
SetTextureEnabled(1);
```

### Використати однотонний колір

```c
SetTextureEnabled(0);
SetColors(0.0f, 1.0f, 0.0f, 1.0f);
```

### Генерація вбудованої текстури

```c
CreateCharacterTexture();
SetTextureEnabled(1);
```

### Використовувати поріг альфа

```c
SetAlphaThreshold(0.1f);
SetAlphaCutoffEnabled(1);
```

Якщо `uAlphaCutoff` увімкнено, пікселі з `color.a < alphaThreshold` відкидаються.

## Розширення та ідеї

- Додати підтримку декількох шейдерів та конструкції сцен.
- Замість статичних вершин використовувати матриці трансформації для обертання, масштабування та переміщення.
- Додати буфер для нормалей та рендеринг освітлення.
- Використати `stb_image_write.h` для експорту зображень або `stb_truetype.h` для рендерингу тексту.
- Додати обробку клавіш та введення миші через GLFW.

## Ліцензія

Проєкт наслідує ліцензію з файлу `LICENSE`.
