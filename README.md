# Teleprompter

Teleprompter es una aplicación para dar charlas y presentaciones: muestra guiones con lectura biónica, controla las diapositivas en un panel de presentador y mide el tiempo con un cronómetro. La lectura biónica guía la vista resaltando la primera parte de cada palabra, lo que ayuda a leer más rápido y con más comprensión. Este software se distribuye bajo los términos de la Licencia Pública General de GNU.

## Características

- Guion en Markdown (`.md`) o texto plano (`.txt`) con lectura biónica, en una columna centrada, con una franja de lectura opcional.
- Diapositivas en PDF con una tira de miniaturas y la vista de la diapositiva actual y la siguiente.
- Modo presentación en una ventana aparte, a pantalla completa en el segundo monitor si existe.
- Cronómetro con inicio, pausa y parada, que puede arrancar solo al presentar.
- Tema oscuro, claro o automático según el sistema, con grises fríos, bordes finos y un único acento azul.
- Panel de configuración cuyos cambios se aplican al instante.
- Atajos de teclado con WASD por defecto y las teclas de vim (`h`, `j`, `k`, `l`) como opción.
- Persistencia de los últimos archivos, el tamaño de letra, las opciones del panel de configuración y la posición de la ventana.

## Uso

El guion se abre con "Open script..." (archivos `.md` o `.txt`; el Markdown conserva encabezados, listas y énfasis con la lectura biónica aplicada) y las diapositivas con "Open slides..." (archivos `.pdf`; una presentación `.pptx` se exporta a PDF antes de abrirla). También se pueden arrastrar los archivos sobre la ventana.

La ventana principal es el panel del presentador: a la izquierda el guion, a la derecha la diapositiva actual y la siguiente, y abajo la tira de miniaturas. Con "Present (F5)" se abre una segunda ventana con la diapositiva actual: a pantalla completa en el segundo monitor si existe, o maximizada si solo hay una pantalla. Mientras dura la presentación, la ventana principal no se mantiene encima de las demás. El guion y las diapositivas son independientes: las teclas de dirección horizontales cambian de diapositiva y las verticales mueven el guion.

El botón "Settings" de la barra de estado (o `Ctrl+,`) abre el panel de configuración, donde los cambios se aplican al instante: el tema (oscuro por defecto, claro o automático según el sistema), mantener la ventana encima de las demás, mostrar la franja de lectura (desactivada por defecto) y su posición, el ancho de la columna, la lectura biónica, si el cronómetro arranca al presentar (desactivado por defecto) y las teclas de vim en lugar de WASD (desactivado por defecto).

La configuración se guarda con `QSettings` (en Linux, en `~/.config/sigifredo/teleprompter.conf`) al cerrar la ventana; el tema se guarda en cuanto se cambia.

### Atajos de teclado

| Tecla                | Acción                                    |
| -------------------- | ----------------------------------------- |
| `←` / `a`            | Diapositiva anterior                      |
| `→` / `d`            | Diapositiva siguiente                     |
| `1-9` + `Enter`      | Ir a una diapositiva por su número        |
| `↑` / `w`, `↓` / `s` | Subir y bajar en el guion                 |
| `PgUp` / `PgDn`      | Subir y bajar una página                  |
| `Home`, `End`        | Inicio y final del guion                  |
| `Ctrl++` / `Ctrl+-`  | Letra más grande o más pequeña            |
| `F5`                 | Iniciar la presentación                   |
| `b`                  | Pantalla en negro durante la presentación |
| `Esc` / `q`          | Salir de la presentación                  |
| `Ctrl+,`             | Abrir el panel de configuración           |
| `?`                  | Mostrar la ayuda de atajos                |

Con las teclas de vim activadas en el panel de configuración, `h`, `j`, `k` y `l` sustituyen a `a`, `s`, `w` y `d` (`h` y `l` para las diapositivas, `k` y `j` para el guion), y además `g` y `G` llevan al inicio y al final del guion. Las letras del modo que no está activo no hacen nada.

## Compilación

Requiere CMake 3.20 o superior, un compilador con C++23 y Qt 6 con los módulos Widgets, Pdf y Svg (en Debian: `qt6-base-dev`, `qt6-pdf-dev` y `qt6-svg-dev`).

```
cmake -S . -B build
cmake --build build -j 8
./build/bin/teleprompter
```

Sin un tipo explícito se compila optimizado (`Release`, con `-O3` y LTO si el compilador lo soporta). Para depurar, `-DCMAKE_BUILD_TYPE=Debug`, que además agrega `-debug` a la versión.

El identificador del paquete es `dev.sigifredo.teleprompter`.

## Estructura

- `include/` y `src/`: cabeceras y fuentes, un par por clase.
- `config/`: la configuración de CMake, repartida en compilación (`build.cmake`), dependencias (`deps.cmake`) y plataforma (`platform.cmake`).
- `design/`: `teleprompter.tokens.json` con los colores, de los que se derivan la paleta de Qt y la hoja de estilos, y `build_icons.py`, que genera los iconos a partir de ellos (requiere Pillow).
- `resources/`: la hoja de estilos (`qss/`), los iconos de la interfaz por tema (`ui/`) y los de la aplicación (`app-icons/`).

## Licencia

GNU General Public License, versión 3 o posterior.
