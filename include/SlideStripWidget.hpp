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
 * @file SlideStripWidget.hpp
 * @brief Tira de miniaturas de las diapositivas.
 */

#ifndef SLIDESTRIPWIDGET_HPP
#define SLIDESTRIPWIDGET_HPP

// qt
#include <QListWidget>

class SlideDeck;

/**
 * @brief Tira horizontal de miniaturas que marca la diapositiva actual.
 *
 * Un clic en una miniatura cambia la diapositiva del SlideDeck. No toma el foco, para que
 * las teclas de dirección sigan llegando al resto de la ventana.
 */
class SlideStripWidget : public QListWidget
{
    Q_OBJECT

public:
    /**
     * @brief Crea la tira, vacía.
     *
     * @param parent Widget padre.
     */
    explicit SlideStripWidget(QWidget *parent = nullptr);

    /**
     * @brief Genera las miniaturas de una presentación y empieza a seguir su diapositiva actual.
     *
     * @param deck Presentación cuyas miniaturas se muestran; debe existir mientras exista la tira.
     */
    void setDeck(SlideDeck *deck);

private:
    SlideDeck *deck_ = nullptr; ///< Presentación que sigue la tira; nullptr hasta que se le asigna una.

private slots:
    /**
     * @brief Marca la miniatura de la diapositiva actual y la centra en la tira, sin devolver el cambio a la presentación.
     *
     * @param index Índice de la diapositiva actual.
     */
    void deckCurrentChanged(int index);

    /**
     * @brief Cambia la diapositiva actual de la presentación a la de la miniatura elegida.
     *
     * @param row Fila elegida; si es negativa no hace nada.
     */
    void rowActivated(int row);
};

#endif
