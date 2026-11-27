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
 * @file PresentationWindow.hpp
 * @brief Ventana de presentación.
 */

#ifndef PRESENTATIONWINDOW_HPP
#define PRESENTATIONWINDOW_HPP

// qt
#include <QImage>
#include <QWidget>

class SlideDeck;

/**
 * @brief Ventana de presentación: la diapositiva actual, centrada sobre fondo negro y sin cursor.
 *
 * Sigue a la presentación: cuando cambia la diapositiva actual, la vuelve a dibujar.
 */
class PresentationWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Crea la ventana con la diapositiva actual de una presentación.
     *
     * @param deck Presentación que se muestra; debe existir mientras exista la ventana.
     * @param parent Widget padre.
     */
    explicit PresentationWindow(SlideDeck *deck, QWidget *parent = nullptr);

public slots:
    /**
     * @brief Alterna entre la diapositiva y la pantalla en negro.
     */
    void toggleBlackout();

signals:
    /**
     * @brief Se emite cuando la ventana se cierra, para que el panel salga del modo presentación.
     */
    void closed();

protected:
    /**
     * @brief Acepta el cierre y emite closed().
     *
     * @param event Evento de cierre.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Pinta el fondo negro y, salvo en la pantalla en negro, la diapositiva centrada.
     *
     * @param event Evento de pintado.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Vuelve a dibujar la diapositiva al nuevo tamaño de la ventana.
     *
     * @param event Evento de cambio de tamaño.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    bool blackout_ = false; ///< La pantalla en negro está activada.
    SlideDeck *deck_;       ///< Presentación de la que se toma la diapositiva.
    QImage slide_;          ///< Diapositiva actual, dibujada al tamaño de la ventana.

    /**
     * @brief Dibuja la diapositiva actual al tamaño de la ventana, con la densidad de píxeles de la pantalla, y repinta.
     */
    void updateSlide();

private slots:
    /**
     * @brief Vuelve a dibujar la diapositiva cuando cambia la diapositiva actual.
     *
     * @param index Índice de la nueva diapositiva actual.
     */
    void deckCurrentChanged(int index);
};

#endif
