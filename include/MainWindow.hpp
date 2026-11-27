/*
 * Teleprompter: A bionic reading and time control application.
 * Copyright (C) 2024 Sigifredo Escobar Gómez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file MainWindow.hpp
 * @brief Ventana principal: el panel del presentador.
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

// qt
#include <QMainWindow>

class BionicWidget;
class ChronometerWidget;
class FileWidget;
class FontBarWidget;
class PresentationWindow;
class QLabel;
class QPushButton;
class SlideDeck;
class SlideStripWidget;

/**
 * @brief Ventana principal: el panel del presentador.
 *
 * Reúne el guion, la diapositiva actual y la siguiente, la tira de miniaturas, el cronómetro y
 * los controles. Con la presentación en marcha, la diapositiva se muestra aparte en una
 * PresentationWindow y esta ventana sigue sirviendo de panel de control.
 *
 * Las teclas se atienden con un filtro de eventos de toda la aplicación, así que funcionan
 * desde cualquiera de las dos ventanas.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Crea la ventana, carga la configuración guardada y reabre los últimos archivos que existan.
     *
     * @param parent Widget padre.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Indica si hay una posición de ventana guardada de una sesión anterior.
     *
     * @return true si hay una posición guardada.
     */
    bool hasSavedGeometry() const;

public slots:
    /**
     * @brief Abre un guion: Markdown si el archivo es `.md` y texto plano en otro caso.
     *
     * Si no se puede leer, avisa con un cuadro de error.
     *
     * @param filePath Ruta del archivo; si está vacía no hace nada.
     */
    void openFile(const QString &filePath);

    /**
     * @brief Abre unas diapositivas en PDF; si no se pueden abrir, avisa con un cuadro de error.
     *
     * @param filePath Ruta del PDF; si está vacía no hace nada.
     */
    void openSlides(const QString &filePath);

    /**
     * @brief Abre la ventana de presentación; sin diapositivas no hace nada.
     *
     * Con más de una pantalla, la presentación ocupa otra pantalla completa; con una sola, se abre
     * maximizada. Mientras dura, esta ventana no se mantiene encima de las demás, y el cronómetro
     * arranca si esa opción está activada.
     */
    void startPresentation();

    /**
     * @brief Cierra la ventana de presentación y, si esa opción está activada, vuelve a mantener esta ventana encima de las demás.
     */
    void stopPresentation();

protected:
    /**
     * @brief Guarda la configuración y cierra la ventana de presentación, si la hay.
     *
     * @param event Evento de cierre.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Acepta que se arrastren archivos sobre la ventana.
     *
     * @param event Evento de arrastre.
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief Abre los archivos soltados: los `.pdf` como diapositivas y los `.md` y `.txt` como guion; ignora los demás.
     *
     * @param event Evento de soltar.
     */
    void dropEvent(QDropEvent *event) override;

    /**
     * @brief Atiende los atajos de teclado de toda la aplicación.
     *
     * No actúa mientras haya un diálogo modal abierto ni con combinaciones de Alt o Meta, que
     * quedan para los mnemónicos de los botones.
     *
     * @param object Objeto que recibe el evento.
     * @param event Evento recibido.
     * @return true si el evento se atendió como atajo y no debe seguir su camino.
     */
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    bool alwaysOnTop_ = true;                    ///< Esta ventana se mantiene encima de las demás.
    BionicWidget *bionicWidget_;                 ///< Visor del guion.
    ChronometerWidget *chronometer_;             ///< Cronómetro.
    SlideDeck *deck_;                            ///< Presentación abierta y su diapositiva actual.
    FileWidget *fileWidget_;                     ///< Fila para elegir el guion.
    FontBarWidget *fontBar_;                     ///< Barra del tamaño de letra.
    QString goToBuffer_;                         ///< Dígitos tecleados para ir a una diapositiva por su número.
    PresentationWindow *presentation_ = nullptr; ///< Ventana de presentación; nullptr si no hay una en marcha.
    QPushButton *presentButton_;                 ///< Botón que inicia la presentación.
    QString scriptPath_;                         ///< Ruta del guion abierto.
    QLabel *slideCounterLabel_;                  ///< Contador de diapositivas de la barra de estado.
    FileWidget *slidesFileWidget_;               ///< Fila para elegir las diapositivas.
    QString slidesPath_;                         ///< Ruta de las diapositivas abiertas.
    SlideStripWidget *slideStrip_;               ///< Tira de miniaturas.
    bool startTimerOnPresent_ = false;           ///< El cronómetro arranca al iniciar la presentación.
    bool vimKeys_ = false;                       ///< Teclas de vim en lugar de WASD.

    /**
     * @brief Atiende las teclas del guion: desplazamiento, tamaño de letra, configuración y ayuda.
     *
     * @param key Tecla, ya traducida por translateKey().
     * @param modifiers Modificadores de teclado.
     * @param text Texto que produce la tecla.
     * @return true si la tecla se atendió.
     */
    bool handleScriptKey(int key, Qt::KeyboardModifiers modifiers, const QString &text);

    /**
     * @brief Atiende las teclas de las diapositivas: cambiar de diapositiva, ir a una por su número, iniciar la presentación y la pantalla en negro.
     *
     * No hace nada si no hay diapositivas.
     *
     * @param key Tecla, ya traducida por translateKey().
     * @param plain true si la tecla se pulsó sin modificadores.
     * @return true si la tecla se atendió.
     */
    bool handleSlideKey(int key, bool plain);

    /**
     * @brief Lee la configuración guardada y reabre los últimos archivos que existan.
     */
    void loadSettings();

    /**
     * @brief Guarda la posición de la ventana, las opciones y las rutas de los últimos archivos.
     */
    void saveSettings();

    /**
     * @brief Traduce las letras a flechas: a, d, w y s con WASD, o h, l, k y j con las teclas de vim.
     *
     * @param key Tecla pulsada.
     * @return La flecha equivalente, o la misma tecla si no es una de esas letras.
     */
    int translateKey(int key) const;

private slots:
    /**
     * @brief Abre el panel de configuración, que aplica cada cambio al instante.
     */
    void openSettings();

    /**
     * @brief Fija si esta ventana se mantiene encima de las demás.
     *
     * Durante la presentación la marca queda apagada y se restablece al terminar.
     *
     * @param enabled true la mantiene encima.
     */
    void setAlwaysOnTop(bool enabled);

    /**
     * @brief Muestra los atajos de teclado del modo de teclas activo.
     */
    void showHelp();

    /**
     * @brief Actualiza el contador de diapositivas de la barra de estado.
     */
    void updateSlideCounter();
};

#endif
