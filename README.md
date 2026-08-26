# Fractales

Explorateur de fractales 2D en temps réel, en C++ / OpenGL, avec zoom en double précision, export d'images et d'animations, et un mode Buddhabrot calculé sur CPU.

Ce projet a démarré comme un premier projet OpenGL/GLSL d'apprentissage, puis a été entièrement refactoré : architecture C++ moderne (RAII, `unique_ptr`, destructeurs virtuels), build CMake multiplateforme, shaders fusionnés (palette en uniform plutôt que dupliquée par fichier), zoom en double précision GLSL, et génération multi-threadée en tâche de fond pour le Nebulabrot.

## Fonctionnalités

- **10 fractales en temps réel** : Mandelbrot, Julia, Burning Ship, Tricorn, Multibrot, Celtic Mandelbrot, Phoenix, Nova, Lambda, Newton
- **Nebulabrot** (Buddhabrot coloré) : calculé sur CPU en tâche de fond (multi-thread, ne bloque pas l'interface), avec barre de progression et paramètres réglables (échantillons, bornes d'itération par canal RGB)
- **6 palettes de couleurs** : Original, Fire, Electric, Gold, Verdoyante, Perle
- **Zoom profond en double précision** (GLSL `dvec2`, GL 4.1 core) : zoom exploitable jusqu'à environ 10¹³-10¹⁴, contre ~10⁶-10⁷ en simple précision
- **Zoom vers le curseur** (molette) et navigation au clic-glisser
- **Zoom infini vers un point remarquable** (touche maintenue) par fractale
- **Export PNG** haute résolution (×4 la résolution de la fenêtre) de la vue actuelle
- **Export GIF animé** :
  - zoom progressif vers le point actuellement centré
  - "morphing" des paramètres (Julia, Phoenix, Nova, Multibrot) : les paramètres propres à chaque fractale varient sur un cycle
- Interface de réglages ImGui (itérations, plage de couleurs, lissage, palette, paramètres spécifiques à chaque fractale)

## Contrôles

| Action                               | Effet                                                |
| ------------------------------------ | ---------------------------------------------------- |
| Clic gauche + glisser                | Déplace la vue                                       |
| Molette                              | Zoom avant/arrière, centré sur le curseur            |
| Maintenir `W` (touche "Z" en AZERTY) | Zoom infini vers un point remarquable de la fractale |
| `R`                                  | Réinitialise le zoom et le centrage                  |
| `Échap`                              | Quitte                                               |

## Compilation

### Prérequis

- CMake ≥ 3.14
- Un compilateur C++17 (MSVC, GCC, Clang)
- Python 3 avec le module `jinja2` (utilisé par [glad2](https://github.com/Dav1dde/glad) pour générer le loader OpenGL au moment du build) :
  ```bash
  pip install jinja2
  ```
- Sur Linux, les paquets de développement X11/OpenGL, par exemple sur Debian/Ubuntu :
  ```bash
  sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
  ```

Toutes les autres dépendances (GLFW, glad2, Dear ImGui, stb_image_write, gif-h) sont récupérées automatiquement par CMake via `FetchContent` — rien à installer manuellement.

### Build

```bash
cmake -B build -S .
cmake --build build --config Release
```

L'exécutable `fractales` est généré dans `build/` (ou `build/Release/` sous Visual Studio). Le dossier `shaders/` est copié automatiquement à côté de l'exécutable à chaque build.

Sous Windows avec Visual Studio 2022, il suffit d'ouvrir le dossier du projet (`Fichier > Ouvrir > Dossier`) : les outils CMake intégrés à VS détectent `CMakeLists.txt` automatiquement. Pense à créer une configuration `x64-Release` (VS ne génère par défaut que `x64-Debug`) via le sélecteur de configuration en haut de l'IDE.

### Options CMake

- `FRACTALES_HIDE_CONSOLE` (`ON` par défaut sur Windows) : masque la fenêtre console au lancement. Utile de la passer à `OFF` temporairement pour voir les logs `std::cerr` pendant une session de debug.

## Le mode Nebulabrot

Contrairement aux 10 autres fractales (un calcul indépendant par pixel, refait à chaque frame dans un fragment shader), le Nebulabrot fonctionne par accumulation statistique : des millions de points aléatoires sont itérés, et pour ceux qui s'échappent de l'ensemble de Mandelbrot, chaque position visitée pendant leur trajectoire est accumulée dans un histogramme — l'image émerge de cette accumulation, pas d'un calcul par pixel. Trois histogrammes séparés (un par canal RGB, chacun avec sa propre borne d'itération) donnent le rendu "Nebulabrot" coloré.

Ce calcul se fait entièrement sur CPU (pas de compute shader, le projet cible OpenGL 4.1 core), réparti sur tous les cœurs disponibles via `std::thread`, et lancé en tâche de fond pour ne pas geler l'interface — une barre de progression suit l'avancement pendant que le reste de l'application (menu, autres fractales) reste utilisable. C'est donc un mode "génère puis affiche" plutôt qu'un mode navigable en direct comme les 10 autres fractales.

## Architecture

```
src/
├── main.cpp                    # Boucle principale, interaction souris/clavier, menu ImGui
├── core/
│   ├── Shader.h/.cpp            # Wrapper RAII pour un programme shader OpenGL (move-only)
│   ├── Fractale.h/.cpp          # Classe de base abstraite pour toutes les fractales
│   ├── Palette.h/.cpp           # Les 6 palettes de couleurs
│   ├── Application.h/.cpp       # RAII pour Dear ImGui + compteur FPS
│   ├── Screenshot.h/.cpp        # Export PNG haute résolution (framebuffer hors écran)
│   ├── GifExport.h/.cpp         # Export GIF animé (zoom, morphing de paramètres)
│   ├── NebulabrotGenerator.h/.cpp  # Calcul CPU multi-threadé du Nebulabrot
│   └── NebulabrotView.h/.cpp    # Génération asynchrone + affichage GPU du résultat
└── fractals/                    # Une classe par fractale (Mandelbrot, Julia, ...)
shaders/                         # Un fragment shader par fractale (palette en uniform)
```

Chaque fractale ne possède qu'**un seul shader** (la palette de couleurs est envoyée en `uniform vec3[8]` plutôt que dupliquée dans un fichier par palette), et la classe de base `Fractale` expose deux points d'extension optionnels : `animateParameters(t)` pour l'export GIF de "morphing", et `coordsForZoom()` pour le zoom infini vers un point remarquable.

## Stack technique

- **C++17**, CMake + `FetchContent` (aucune dépendance vendorée dans le repo)
- **OpenGL 4.1 Core** / GLSL 410 (double précision `dvec2`)
- [GLFW](https://www.glfw.org/) — fenêtrage et contexte OpenGL
- [glad2](https://github.com/Dav1dde/glad) — chargement des fonctions OpenGL
- [Dear ImGui](https://github.com/ocornut/imgui) — interface de réglages
- [stb_image_write](https://github.com/nothings/stb) — export PNG
- [gif-h](https://github.com/charlietangora/gif-h) — export GIF animé
- `std::thread` — génération multi-threadée du Nebulabrot

## Historique du projet

Le code original (Visual Studio, `.sln`/`.vcxproj`, shaders dupliqués par palette) reste consultable dans l'historique Git, sur la branche `main`, avant le merge de la branche `refactor/cmake-clean-architecture`.
