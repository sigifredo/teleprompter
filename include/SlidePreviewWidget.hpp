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
 * @file SlidePreviewWidget.hpp
 * @brief Vista previa de una diapositiva.
 */

#ifndef SLIDEPREVIEWWIDGET_HPP
#define SLIDEPREVIEWWIDGET_HPP

// qt
#include <QImage>
#include <QWidget>

class SlideDeck;

/**
 * @brief Muestra la diapositiva que está a una distancia fija de la actual, con un título encima.
 *
 * Un desfase de 0 es la diapositiva actual y uno de 1, la siguiente. Fuera de rango, o sin
 * presentación, muestra un recuadro con el motivo.
 */
class SlidePreviewWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Crea la vista previa.
     *
     * @param deck Presentación de la que se toman las diapositivas; debe existir mientras exista la vista.
     * @param offset Desfase respecto de la diapositiva actual: 0 la actual y 1 la siguiente.
     * @param caption Título que se muestra encima de la diapositiva.
     * @param parent Widget padre.
     */
    SlidePreviewWidget(SlideDeck *deck, int offset, const QString &caption, QWidget *parent = nullptr);

    /**
     * @brief Tamaño mínimo: el del área de la diapositiva más el del título.
     *
     * @return El tamaño mínimo.
     */
    QSize minimumSizeHint() const override;

protected:
    /**
     * @brief Dibuja el título y la diapositiva con un borde.
     *
     * Sin diapositiva dibuja un recuadro discontinuo con el motivo: «No slides» o «End of slides».
     *
     * @param event Evento de pintado.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Vuelve a dibujar la diapositiva al nuevo tamaño.
     *
     * @param event Evento de cambio de tamaño.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    QString caption_; ///< Título mostrado encima de la diapositiva.
    SlideDeck *deck_; ///< Presentación de la que se toman las diapositivas.
    QImage image_;    ///< Diapositiva dibujada; nula si no hay ninguna que mostrar.
    int offset_;      ///< Desfase respecto de la diapositiva actual.

    /**
     * @brief Área bajo el título donde se dibuja la diapositiva.
     *
     * @return El rectángulo, en coordenadas del widget.
     */
    QRect slideArea() const;

    /**
     * @brief Dibuja la diapositiva que está al desfase de la actual, al tamaño del área y con la densidad de píxeles de la pantalla, y repinta.
     */
    void updateImage();

private slots:
    /**
     * @brief Vuelve a dibujar la diapositiva cuando cambia la diapositiva actual.
     *
     * @param index Índice de la nueva diapositiva actual.
     */
    void deckCurrentChanged(int index);
};

#endif
