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

#ifndef PRESENTATIONWINDOW_HPP
#define PRESENTATIONWINDOW_HPP

// qt
#include <QImage>
#include <QWidget>

class SlideDeck;

/**
 * @brief Ventana de presentación: la diapositiva actual, centrada sobre fondo negro y sin cursor.
 */
class PresentationWindow : public QWidget
{
    Q_OBJECT

public:
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
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void deckCurrentChanged(int index);

private:
    void updateSlide();

    SlideDeck *deck_;
    QImage slide_;
    bool blackout_ = false;
};

#endif
