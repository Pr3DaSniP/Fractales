# Build

## Prérequis

- CMake >= 3.14
- Un compilateur C++17 (MSVC, gcc, clang)
- Python 3 + le module `jinja2` (utilisé par glad2 pour générer le loader OpenGL) :
  ```bash
  pip install jinja2
  ```
- Sur Linux : les paquets de dev pour X11/OpenGL (ex. sur Debian/Ubuntu) :
  ```bash
  sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
  ```

## Compiler

```bash
cmake -B build -S .
cmake --build build --config Release
```

L'exécutable `fractales` est généré dans `build/` (ou `build/Release/` sous MSVC).
Le dossier `shaders/` est copié automatiquement à côté de l'exécutable après
chaque build.

## Ce que fait ce squelette pour l'instant

- Ouvre une fenêtre GLFW avec un contexte OpenGL 3.3 Core.
- Charge les fonctions OpenGL via glad2.
- Affiche la version OpenGL en console et efface l'écran en boucle.

Rien d'autre pour le moment : ImGui est compilé et lié, mais pas encore
initialisé dans `main.cpp`. C'est la prochaine étape une fois que ce
squelette compile et s'exécute correctement chez toi.
