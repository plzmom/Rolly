# OpenGL Texture Demo

Це невеликий OpenGL-проєкт на C з рендером квадрата, який може працювати у двох режимах:
- однотонний колір;
- текстура (PNG-файл із каменем).

## Опис

Проєкт використовує GLFW для створення вікна та контексту OpenGL, GLAD для завантаження функцій OpenGL, та `stb_image.h` для завантаження PNG-текстури.

Квадрат рендериться як два трикутники. У вершинних даних присутні координати позиції та UV-координати для накладання текстури.

## Структура проєкту

- `Makefile` — правила збірки.
- `src/main.c` — головний файл, який ініціалізує движок, налаштовує буфери та запускає цикл відрисовки.
- `src/lib/win/window.c` — створення вікна GLFW, керування контекстом та завершення.
- `src/lib/win/window.h` — оголошення API роботи з вікном.
- `src/lib/render/render.c` — реалізація рендеру, шейдерів, буферів, створення текстур та накладання.
- `src/lib/render/render.h` — API рендеру.
- `src/lib/render/stb_image.h` — заголовок для завантаження PNG.
- `src/assets/stone.png` — приклад текстури каменю для накладання.
- `src/glad` — GLAD loader для OpenGL.

## Функції та API

### `src/main.c`

- `Init()` — ініціалізація GLFW.
- `create_window(...)` — створює вікно з заданими розмірами.
- `MakeContext(win)` — робить контекст активним.
- `errorGLAD(win)` — перевіряє GLAD.
- `InitEP(...)` — налаштовує VAO/VBO/EBO для квадрата.
- `CreateStandartShader()` — компілює шейдери.
- `CreateTextureFromFile("src/assets/stone.png")` — завантажує текстуру з PNG-файлу.
- `SetColors(...)` — встановлює колір, який використовується якщо текстура відключена.
- `SetTextureEnabled(1)` — вмикає режим текстури.
- `loop(win)` — головний цикл рендеру.
- `Draw()` — малює квадрат.
- `DeleteTrash()` — очищає виділені OpenGL-ресурси.
- `Destroy(win)` та `Terminate()` — закривають вікно і припиняють роботу GLFW.

### `src/lib/render/render.c`

- `CreateCharacterTexture()` — генерація вбудованої текстури персонажа.
- `CreateTextureFromFile(const char *path)` — завантаження зображення з файлу за допомогою `stb_image.h`.
- `SetTextureEnabled(int enabled)` — перемикає режим між кольором та текстурою.

## Запуск

### Встановлення залежностей

Потрібні бібліотеки:
- `glfw`
- `OpenGL`
- `libdl`
- `gcc`
- `make`

На Ubuntu це можна встановити так:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libgl-dev libasound2-dev libpulse-dev libudev-dev libdbus-1-dev libdl-dev
```

### Збірка

```bash
make
```

### Запуск

```bash
./app
```

## Текстури

### Поточний режим

Поточний `src/main.c` використовує:

```c
CreateTextureFromFile("src/assets/stone.png");
SetTextureEnabled(1);
```

Це завантажує файл `src/assets/stone.png` і відображає його на квадраті.

### Перехід на колір

Щоб рендерити однотонний колір замість текстури, змініть на:

```c
SetTextureEnabled(0);
```

### Як додати власну текстуру

Щоб замінити `stone.png` на власне зображення:
1. Покладіть PNG у `src/assets/`.
2. Замініть шлях у `CreateTextureFromFile(...)`.
3. Перекомпілюйте.

## Рекомендації

- Для підтримки інших форматів можна додати `stb_image_write.h` або аналогічну бібліотеку.
- Щоб зробити обертання або масштабування, додайте матричні шейдери та передачу матриць у вершинний шейдер.
- Для рендеру кількох об'єктів розширте `InitEP` та додайте кілька VAO/VBO.

## Ліцензія

Проєкт наслідує ліцензію з файлу `LICENSE`.
