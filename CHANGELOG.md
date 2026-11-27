# Changelog

Todos los cambios notables de este proyecto se documentan en este archivo.

El formato sigue [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/) y el proyecto usa [versionado semántico](https://semver.org/lang/es/).

## [0.2.0] - 2026-11-26

### Added

- Apertura de diapositivas en PDF con una tira de miniaturas que marca la diapositiva actual.
- Modo presentación: una segunda ventana muestra la diapositiva actual, a pantalla completa en el segundo monitor si existe, mientras la ventana principal queda como panel del presentador.
- Vista del presentador con la diapositiva actual y la siguiente junto al guion.
- Guion en Markdown (`.md`) que conserva encabezados, listas y énfasis con la lectura biónica aplicada.
- Navegación con teclado: `←`/`h` y `→`/`l` para las diapositivas, `↑`/`k` y `↓`/`j` para el guion, `PgUp`/`PgDn`, `Home`/`g`, `End`/`G`, número seguido de `Enter` para ir a una diapositiva, `Ctrl++`/`Ctrl+-` para el tamaño de letra, `F5` para presentar, `b` para la pantalla en negro, `Esc`/`q` para salir y `?` para la ayuda de atajos.
- Tema oscuro, claro y automático según el sistema, con el oscuro por defecto y una paleta de colores fríos de croma bajo, con el contraste del texto en torno a 13:1 para cansar menos la vista.
- Columna de lectura centrada con una franja de lectura resaltada en el guion.
- Arrastrar y soltar archivos `.md`, `.txt` y `.pdf` sobre la ventana.
- Persistencia de los últimos archivos, el tamaño de letra, el tema, la opción de mantener la ventana encima y la posición de la ventana.
- Opción para mantener la ventana encima de las demás, ahora conmutable.
- Estado vacío con instrucciones cuando no hay guion ni diapositivas.
- Ocultar el cursor y pantalla en negro en la ventana de presentación.
- El cronómetro arranca al iniciar la presentación.

### Changed

- La interfaz usa una barra de controles única con los archivos, el cronómetro, el tamaño de letra y el botón de presentar, y una barra de estado con el contador de diapositivas, la ayuda, el tema y la versión.
- Los colores se definen como tokens en una hoja de estilos compartida por toda la aplicación, incluidos los diálogos.
- Nuevo ícono de la aplicación, con la paleta fría: una pantalla con la línea de lectura resaltada sobre fondo oscuro.
- Las teclas `↑`/`k` y `↓`/`j` mueven el guion exactamente una línea visual y la dejan en la franja de lectura, sea cual sea su altura, y el guion tiene espacio arriba y abajo para que la primera y la última línea también lleguen a la franja.
- El enlace del autor en el diálogo About apunta a `sigifredo.dev`.
- El identificador del paquete es ahora `dev.sigifredo.teleprompter`.
- La compilación requiere Qt 6 con el módulo Pdf y CMake 3.16 o superior; se elimina el soporte de Qt 5 y Android.

### Fixed

- Conflicto del atajo `Alt+P` entre "Stop" y "Present".
- Ruta incorrecta del ícono `.icns` en la configuración de macOS.
- Foco de teclado invisible en los botones.

### Removed

- El logo de s9art del diálogo About.

## [0.1.1] - 2024-07-08

### Added

- Lectura biónica de archivos de texto, con tamaño de letra ajustable y cronómetro con inicio, pausa y parada.
- Diálogo de créditos con la información de la aplicación, el autor y la licencia GNU GPL.
- Ícono de la aplicación en formatos png, ico, svg e icns.
- Indicación de la versión de depuración al compilar en modo debug.
- Compilación en macOS.

### Fixed

- Formato del cronómetro.
