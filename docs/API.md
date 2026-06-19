# API: Опис функцій та використання

Нижче наведено опис усіх публічних функцій проекту та приклади використання. Файли з огляду: `src/lib/win/window.c`, `src/lib/render/render.c`, `src/lib/UI/ui.h` / `ui.cpp`, `src/main.c`.

**Створення та управління вікном**

- `Window *create_window(int w, int h, const char *t)`:
  - Опис: ініціалізує GLFW (якщо потрібно) і створює вікно розміром `w`×`h` з заголовком `t`.
  - Повертає: вказівник на `GLFWwindow` (тип `Window`), або `NULL` при помилці.
  - Приклад:
    ```c
    Window *win = create_window(1280, 720, "Demo");
    if (!win) return -1;
    ```

- `void MakeContext(Window *window)`:
  - Опис: робить передане вікно поточним OpenGL-контекстом через `glfwMakeContextCurrent`.

- `void Destroy(Window *window)` та `void Terminate(void)`:
  - Опис: відповідно знищують вікно та завершують роботу GLFW.

- `int errorGLAD(Window *window)`:
  - Опис: ініціалізує GLAD за допомогою `glfwGetProcAddress`, встановлює callback для зміни розміру буферу та повертає `0` при успіху або `-1` при помилці.

**Рендеринг і об'єкти**

- `unsigned int CreateStandartShader(void)`:
  - Опис: створює простий шейдер (вершинний + фрагментний) для рендерингу текстур або кольору. Повертає `GLuint` програми шейдера.

- `void DeleteObject(unsigned int id)`:
  - Опис: Видаляє об'єкт со сцени.

- `unsigned int CreateObject(float *vertices, size_t verticesSize, unsigned int *indices, size_t indicesSize, const char *texturePath, float r, float g, float b, float a)`:
  - Опис: створює об'єкт для рендерингу.
  - `vertices` очікує масив з кроком 5 (X, Y, Z, U, V) — позиція та UV координати.
  - `indices` — індексний буфер.
  - `texturePath` — шлях до файлу з текстурою (може бути `NULL`). Якщо задано, текстура завантажується через `stb_image`.
  - Повертає індекс об'єкта (unsigned int) або `(unsigned int)-1` при помилці.

- `void SetObjectPosition(unsigned int id, float x, float y)` — встановлює позицію об'єкта.

- `int MoveObject(unsigned int id, float dx, float dy)` — переміщує об'єкт на (dx, dy). Повертає:
  - `0` — нічого не зроблено (наприклад, помилка id),
  - `1` — переміщення виконано,
  - `2` — рух заблоковано колізією.

 - `void SetObjectCollisionEnabled(unsigned int id, int enable, float colliderW, float colliderH, float colliderOx, float colliderOy)` — вмикає/вимикає перевірку зіткнень для об'єкта. Якщо `enable` = 1, додаткові параметри задають розміри та офсет колайдера (ширина, висота, офсет X, офсет Y). Якщо `enable` = 0, колайдер скидається і об'єкт не блокується.

 - `void SetCollisionMargin(float margin)` — задає невеликий додатковий відступ, який додається до пів-вимірів колайдерів під час перевірки зіткнень. Це дозволяє уникнути ситуацій, коли об'єкти опиняються точно "в притик". Передавай `0.0f` щоб відключити; за замовчуванням margin = 0.02f.

 - `int CheckObjectCollision(unsigned int a, unsigned int b)` — перевіряє колізію між двома об'єктами, повертає ненульове значення при зіткненні.

- `void SetObjectAlphaCutoff(unsigned int id, float cutoff)` — встановлює поріг прозорості; пікселі з альфою нижче `cutoff` відкидаються (discard) у фрагментному шейдері.

- `void SetObjectRemoveBackground(unsigned int id, int enable)` — зручно вмикає `alphaCutoff` на значення `0.1f` якщо включено або `0.0f` якщо виключено.

 - `void SetObjectTriggerEnabled(unsigned int objectId, int enable, float triggerW, float triggerH, float triggerOx, float triggerOy)` — налаштовує тригер-зону для об'єкта. Тригер не блокує рух; використовується для сповіщень при входженні/виході інших об'єктів.

 - `int CheckObjectTrigger(unsigned int triggerId, unsigned int objectId)` — перевіряє, чи перетинає `objectId` тригер-зону `triggerId`; повертає ненульове значення при перекритті.

- `void ColorBG(float r, float g, float b, float a)` — встановлює колір фону і очищає буфер кольору.

- `void DrawAll(void)` — малює всі створені об'єкти використовуючи один шейдер і значення їх властивостей (позиція, текстури, колір).

- `void DeleteTrash(void)` — звільняє всі GPU-ресурси (VAO, VBO, EBO, текстури) та видаляє програму шейдера.

**Фізика / рух**

- `void update_body(unsigned int id, float g)` — проста інтеграція швидкостей тіла під впливом прискорення `g`. Оновлює `vy`, рахує дельти і викликає `MoveObject`.

**UI (ImGui wrapper)**

- `void UI_Init(struct GLFWwindow *window)` — ініціалізує ImGui з підтримкою GLFW + OpenGL3.
- `void UI_NewFrame(void)`, `void UI_Render(void)`, `void UI_Shutdown(void)` — стандартні циклові виклики ImGui.
- `void UI_Text(const char *fmt, ...)`, `int UI_Button(const char *text)` — допоміжні виклики для тексту і кнопок.
- `int UI_BeginWindow(const char *name)`, `void UI_EndWindow(void)` — початок/кінець вікна ImGui.
- `int UI_DrawOverlay(int wasHit)` — простий оверлей, що показує `wasHit` і кнопку "Reset object 1"; повертає 1 якщо натискали.
- `int UI_TextButton(const char *text)` - проста кнопка без фона тільки текст

**Поради по використанню**

- При додаванні нового файлу з C++ кодом, додайте його у `CMakeLists.txt` в секцію `SRCS_CPP`.
- Для відладки встановіть змінну середовища `DEBUG_OBJECTS=1`, тоді `DrawAll` друкуватиме інформацію про об'єкти.

**Приклад використання (з main.c):**

1. Створити вікно через `create_window`.
2. Викликати `MakeContext` і `errorGLAD`.
3. Ініціалізувати UI через `UI_Init`.
4. Створити шейдер `CreateStandartShader()` і кілька об'єктів `CreateObject(...)`.
5. У циклі головному викликати логіку руху, перевірки колізій, `ColorBG`, `DrawAll`, ImGui-кадри та swap-buffer.
