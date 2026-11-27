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
    SlidePreviewWidget(SlideDeck *deck, int offset, const QString &caption, QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    SlideDeck *deck_;
    int offset_;
    QString caption_;
    QImage image_;

    QRect slideArea() const;
    void updateImage();

private slots:
    void deckCurrentChanged(int index);
};

#endif
