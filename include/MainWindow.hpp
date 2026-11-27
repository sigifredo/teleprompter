/**
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
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Indica si hay una posición de ventana guardada de una sesión anterior.
     */
    bool hasSavedGeometry() const;

public slots:
    /**
     * @brief Abre un guion: Markdown si el archivo es `.md` y texto plano en otro caso.
     */
    void openFile(const QString &filePath);

    /**
     * @brief Abre unas diapositivas en PDF; avisa si no se pueden abrir.
     */
    void openSlides(const QString &filePath);

    /**
     * @brief Abre la ventana de presentación, en otra pantalla si hay una; sin diapositivas no hace nada.
     */
    void startPresentation();

    void stopPresentation();

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private slots:
    void openSettings();
    void setAlwaysOnTop(bool enabled);
    void showHelp();
    void updateSlideCounter();

private:
    BionicWidget *bionicWidget_;
    ChronometerWidget *chronometer_;
    FileWidget *fileWidget_;
    FileWidget *slidesFileWidget_;
    FontBarWidget *fontBar_;
    SlideDeck *deck_;
    SlideStripWidget *slideStrip_;
    QLabel *slideCounterLabel_;
    QPushButton *presentButton_;
    PresentationWindow *presentation_ = nullptr;
    QString goToBuffer_; ///< Dígitos tecleados para ir a una diapositiva por su número.
    QString scriptPath_;
    QString slidesPath_;
    bool alwaysOnTop_ = true;
    bool startTimerOnPresent_ = false;
    bool vimKeys_ = false;

    int translateKey(int key) const;
    bool handleSlideKey(int key, bool plain);
    bool handleScriptKey(int key, Qt::KeyboardModifiers modifiers, const QString &text);
    void loadSettings();
    void saveSettings();
};

#endif
