# Changelog

Todos los cambios notables de este proyecto se documentan en este archivo.

El formato sigue las convenciones de [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/), y este proyecto adhiere a [Versionado Semántico](https://semver.org/lang/es/).

## [0.2.0] - 2026-11-26

### Añadido

- **diapositivas**: apertura de diapositivas en PDF con una tira de miniaturas que marca la diapositiva actual.
- **presentación**: una segunda ventana muestra la diapositiva actual, a pantalla completa en el segundo monitor si existe, mientras la ventana principal queda como panel del presentador. Oculta el cursor y admite pantalla en negro. Una opción hace que el cronómetro arranque al iniciarla, desactivada por defecto.
- **presentador**: vista con la diapositiva actual y la siguiente junto al guion.
- **guion**: Markdown (`.md`) que conserva encabezados, listas y énfasis con la lectura biónica aplicada. Columna de lectura centrada con una franja de lectura opcional, desactivada por defecto.
- **teclado**: `←`/`a` y `→`/`d` para las diapositivas, `↑`/`w` y `↓`/`s` para el guion, `PgUp`/`PgDn`, `Home`, `End`, número seguido de `Enter` para ir a una diapositiva, `Ctrl++`/`Ctrl+-` para el tamaño de letra, `F5` para presentar, `b` para la pantalla en negro, `Esc`/`q` para salir y `?` para la ayuda de atajos. Las teclas de vim (`h`, `j`, `k`, `l`, `g`, `G`) son una opción del panel de configuración, en lugar de WASD y desactivadas por defecto.
- **configuración**: panel (`Ctrl+,`) con aplicación inmediata de los cambios: tema, mantener la ventana encima, franja de lectura visible y su posición, ancho de la columna, lectura biónica, arranque del cronómetro al presentar y teclas de vim.
- **tema**: oscuro, claro y automático según el sistema, con el oscuro por defecto y una estética tipo GitHub de grises fríos con bordes finos y un único acento azul. El contraste del texto ronda 13:1 para cansar menos la vista.
- **design**: `design/teleprompter.tokens.json` es la fuente única de los colores, y `design/build_icons.py` regenera los iconos de la aplicación y las marcas de las casillas a partir de él.
- **SlideDeck**: avisa en el registro (`[SlideDeck]`) cuando un PDF no se puede abrir.
- **proyecto**: `.gitattributes` con los finales de línea en LF.
- **archivos**: arrastrar y soltar archivos `.md`, `.txt` y `.pdf` sobre la ventana. Se recuerdan los últimos archivos, el tamaño de letra, el tema, las opciones del panel de configuración y la posición de la ventana. Un estado vacío da instrucciones cuando no hay guion ni diapositivas.

### Cambiado

- **interfaz**: barra de controles única con los archivos, el cronómetro, el tamaño de letra y el botón de presentar, y una barra de estado con el contador de diapositivas, la ayuda, los ajustes y la versión.
- **estilo**: `Theme` lee los colores de los tokens y expande la hoja de estilos, una plantilla con `{{token}}` en `resources/qss/teleprompter.qss` compartida por toda la aplicación, incluidos los diálogos. Las variantes de estilo se eligen con propiedades dinámicas (`role`, `variant`, `active`) y las marcas de las casillas son SVG por tema. Los recursos se cargan con el prefijo `/teleprompter` desde `CMakeLists.txt`, sin archivo `.qrc`.
- **código**: se adopta el estilo de código de `plexus-cpp`. Los miembros llevan guion bajo al final y ningún identificador lleva prefijo de tipo; las constantes son `constexpr` en CamelCase en lugar de macros; `connect` usa punteros a miembro; los cuerpos de una sola sentencia no llevan llaves; los constructores de un argumento son `explicit`; los textos visibles pasan por `tr()`; y los comentarios y la documentación están en español.
- **ícono**: nuevo diseño con la paleta fría: una pantalla con la línea de lectura resaltada sobre fondo oscuro.
- **guion**: las teclas de subir y bajar (`↑`/`w` y `↓`/`s`, o `k` y `j` con vim) mueven el guion exactamente una línea visual y la dejan en la franja de lectura, sea cual sea su altura. El guion tiene espacio arriba y abajo para que la primera y la última línea también lleguen a la franja.
- **About**: el enlace del autor apunta a `sigifredo.dev`.
- **paquete**: el identificador es ahora `dev.sigifredo.teleprompter`.
- **compilación**: por defecto se compila optimizado (`Release`, con `-O3`, `-DNDEBUG` y LTO si el compilador lo soporta); `Debug` pasa a ser opcional con `-DCMAKE_BUILD_TYPE=Debug`.
- **compilación**: requiere Qt 6 con los módulos Pdf y Svg, un compilador con C++23 y CMake 3.20 o superior; se elimina el soporte de Qt 5 y Android. La configuración se reparte en `config/build.cmake`, `config/deps.cmake` y `config/platform.cmake`, y los archivos se listan en `HDRS` y `SRCS`.

### Corregido

- **atajos**: conflicto de `Alt+P` entre "Stop" y "Present".
- **macOS**: ruta incorrecta del ícono `.icns` en la configuración.
- **foco**: el foco del teclado era invisible en los botones.

### Eliminado

- **About**: el logo de s9art.

## [0.1.1] - 2024-07-08

### Añadido

- **lectura**: lectura biónica de archivos de texto, con tamaño de letra ajustable y cronómetro con inicio, pausa y parada.
- **About**: diálogo de créditos con la información de la aplicación, el autor y la licencia GNU GPL.
- **ícono**: de la aplicación en formatos png, ico, svg e icns.
- **versión**: indicación de la versión de depuración al compilar en modo debug.
- **compilación**: compilación en macOS.

### Corregido

- **cronómetro**: formato del tiempo.
