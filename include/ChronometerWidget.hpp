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

#ifndef CHRONOMETERWIDGET_HPP
#define CHRONOMETERWIDGET_HPP

// qt
#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;

/**
 * @brief Cronómetro con botones de inicio, pausa y parada.
 *
 * Muestra el tiempo como mm:ss, o como H:mm:ss pasada la hora. Mientras corre, la etiqueta
 * lleva la propiedad dinámica `active`, que la hoja de estilos usa para destacarla.
 */
class ChronometerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChronometerWidget(QWidget *parent = nullptr);

    /**
     * @brief Indica si el cronómetro está corriendo.
     */
    bool isRunning() const;

public slots:
    /**
     * @brief Inicia el cronómetro; si ya corría, lo pausa.
     */
    void start();

    /**
     * @brief Lo detiene y vuelve el tiempo a cero.
     */
    void stop();

private:
    QPushButton *startButton_;
    QLabel *timeLabel_;
    QTimer *timer_;
    int timeElapsed_ = 0;

    void setActiveStyle(bool active);

private slots:
    void updateTime();
};

#endif
