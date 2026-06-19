# Simple GL Demo Library

Цей репозиторій містить невеликий движок для рендерингу 2D-об'єктів на OpenGL з інтегрованим ImGui для налагодження. Проєкт підтримує збірку на Linux та Windows через CMake.

Швидкий огляд:
- **Мова:** C (ядро) + C++ (ImGui)
- **Зовнішні залежності:** GLFW, OpenGL (системна реалізація)
- **Включені:** GLAD, ImGui (локальні вихідні файли)

Документація:
- Проєкт: docs/PROJECT_DOC.md
- API (опис функцій): docs/API.md

Збірка (Linux):

```bash
mkdir -p build && cd build
cmake ..
cmake --build . --config Release
```

Збірка (Windows, з встановленим CMake + vcpkg/пакетами):

```powershell
mkdir build; cd build
cmake .. -A x64
cmake --build . --config Release
```

Після збірки виконуваний файл `app` з'явиться в директорії `build`.
