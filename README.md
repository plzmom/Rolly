# Simple GL Demo Library

Цей репозиторій містить невеликий движок для рендерингу 2D-об'єктів на OpenGL з інтегрованим ImGui для налагодження. Проєкт підтримує збірку на Linux та Windows через CMake.

Швидкий огляд:
- **Мова:** C (ядро) + C++ (ImGui)
- **Зовнішні залежності:** GLFW, OpenGL (системна реалізація)
- **Включені:** GLAD, ImGui (локальні вихідні файли)

Документація:
- Проєкт: docs/PROJECT_DOC.md
- API (опис функцій): docs/API.md

## Як збирати

### 1. Збірка на Linux

```bash
mkdir -p build && cd build
cmake .. -DBUILD_APP=ON -DAPP_ENTRY_FILE=src/main.c
cmake --build . --config Release
```

### 2. Збірка на Windows

```powershell
mkdir build; cd build
cmake .. -A x64 -DBUILD_APP=ON -DAPP_ENTRY_FILE=src\main.c -DGLFW_FROM_SOURCE=ON
cmake --build . --config Release
```

> Якщо на Windows не знайдено системну бібліотеку GLFW, постав `-DGLFW_FROM_SOURCE=ON`.

### 3. Збірка конкретного файлу

Щоб компілювати інший `.c` файл як точку входу, вкажи `APP_ENTRY_FILE`:

```bash
cmake -S . -B build -DBUILD_APP=ON -DAPP_ENTRY_FILE=src/hsgdf.c
cmake --build build
```

### 4. Збірка тільки бібліотеки

Якщо ти хочеш отримати тільки бібліотеку без виконуваного файлу, вимкни `BUILD_APP`:

```bash
cmake -S . -B build -DBUILD_APP=OFF
cmake --build build
```

## Опції CMake

- `BUILD_APP` — створювати виконуваний файл `app` чи тільки бібліотеку.
- `APP_ENTRY_FILE` — шлях до C-файлу, який збирається як точка входу для `app`.
- `GLFW_FROM_SOURCE` — завантажити GLFW із вихідників замість використання системного пакету.

## Результат збірки

- На Linux виконавчий файл буде `build/app`.
- На Windows виконавчий файл буде `build/Release/app.exe` або `build/app.exe`, залежно від конфігурації.

## Використання як бібліотеки

Проєкт створює статичну бібліотеку `libapp` та CMake-ціль `game::libapp`.
Інші проєкти можуть підключати її і використовувати API функцій з `src/lib/UI/ui.h` та інших заголовків.
