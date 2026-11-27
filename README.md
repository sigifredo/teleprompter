# Teleprompter

 Teleprompter is an application designed to enhance reading efficiency and manage time effectively. Utilizing the concept of bionic reading, it aids users in improving their reading speed and comprehension by guiding the eyes through text with strategically highlighted focal points. Additionally, the app includes time control via a chronometer, allowing users to check the time while reading. This software is distributed under the terms of the GNU General Public License.

## Uso

Abre el guion con "Open script..." (archivos `.md` o `.txt`; el Markdown conserva encabezados, listas y énfasis con la lectura biónica aplicada) y las diapositivas con "Open slides..." (archivos `.pdf`; exporta tu presentación `.pptx` a PDF antes de abrirla). También puedes arrastrar los archivos sobre la ventana. La aplicación recuerda los últimos archivos, el tamaño de letra, el tema y la posición de la ventana.

La ventana principal es el panel del presentador: a la izquierda el guion, en una columna centrada con una franja de lectura; a la derecha la diapositiva actual y la siguiente; abajo la tira de miniaturas. Con "Present (F5)" se abre una segunda ventana con la diapositiva actual, a pantalla completa en el segundo monitor si existe, y arranca el cronómetro. El guion y las diapositivas son independientes.

El tema por defecto es oscuro; el botón "Theme" de la barra de estado alterna entre oscuro, claro y automático (según el sistema).

| Tecla | Acción |
| --- | --- |
| `←` / `h` | Diapositiva anterior |
| `→` / `l` | Diapositiva siguiente |
| `1-9` + `Enter` | Ir a una diapositiva por su número |
| `↑` / `k`, `↓` / `j` | Subir y bajar en el guion |
| `PgUp` / `PgDn` | Subir y bajar una página |
| `Home` / `g`, `End` / `G` | Inicio y final del guion |
| `Ctrl++` / `Ctrl+-` | Letra más grande o más pequeña |
| `F5` | Iniciar la presentación |
| `b` | Pantalla en negro durante la presentación |
| `Esc` / `q` | Salir de la presentación |
| `?` | Mostrar esta ayuda de atajos |

## Compilación

Requiere Qt 6 con los módulos Widgets y Pdf (en Debian: `qt6-base-dev` y `qt6-pdf-dev`). El identificador del paquete es `dev.sigifredo.teleprompter`.
