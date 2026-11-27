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
    explicit SlideStripWidget(QWidget *parent = nullptr);

    /**
     * @brief Genera las miniaturas de una presentación y empieza a seguir su diapositiva actual.
     */
    void setDeck(SlideDeck *deck);

private:
    SlideDeck *deck_ = nullptr;

private slots:
    void deckCurrentChanged(int index);
    void rowActivated(int row);
};

#endif
