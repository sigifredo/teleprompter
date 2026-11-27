# Teleprompter

 Teleprompter is an application designed to enhance reading efficiency and manage time effectively. Utilizing the concept of bionic reading, it aids users in improving their reading speed and comprehension by guiding the eyes through text with strategically highlighted focal points. Additionally, the app includes time control via a chronometer, allowing users to check the time while reading. This software is distributed under the terms of the GNU General Public License.

## Uso

Abre el guion con "Open script..." (archivos `.md` o `.txt`; el Markdown conserva encabezados, listas y énfasis con la lectura biónica aplicada) y las diapositivas con "Open slides..." (archivos `.pdf`; exporta tu presentación `.pptx` a PDF antes de abrirla). La tira inferior muestra las miniaturas de las diapositivas y marca la actual.

Con "Present (F5)" se abre una segunda ventana con la diapositiva actual, a pantalla completa en el segundo monitor si existe, mientras la ventana principal queda como panel de control con el guion y la tira de diapositivas. El guion y las diapositivas son independientes.

| Tecla | Acción |
| --- | --- |
| `←` / `h` | Diapositiva anterior |
| `→` / `l` | Diapositiva siguiente |
| `↑` / `k` | Subir en el guion |
| `↓` / `j` | Bajar en el guion |
| `F5` | Iniciar la presentación |
| `Esc` / `q` | Salir de la presentación |

## Compilación

Requiere Qt 6 con los módulos Widgets y Pdf (en Debian: `qt6-base-dev` y `qt6-pdf-dev`).
