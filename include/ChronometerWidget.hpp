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
 * @file ChronometerWidget.hpp
 * @brief Cronómetro con botones de inicio, pausa y parada.
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
    /**
     * @brief Crea el cronómetro, parado y en cero.
     *
     * @param parent Widget padre.
     */
    explicit ChronometerWidget(QWidget *parent = nullptr);

    /**
     * @brief Indica si el cronómetro está corriendo.
     *
     * @return true si corre.
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
    QPushButton *startButton_; ///< Botón de inicio y pausa.
    int timeElapsed_ = 0;      ///< Segundos transcurridos.
    QLabel *timeLabel_;        ///< Etiqueta con el tiempo transcurrido.
    QTimer *timer_;            ///< Temporizador que avanza el tiempo cada segundo.

    /**
     * @brief Marca la etiqueta del tiempo con la propiedad `active` y repinta su estilo.
     *
     * @param active true mientras el cronómetro corre.
     */
    void setActiveStyle(bool active);

private slots:
    /**
     * @brief Suma un segundo y actualiza la etiqueta: mm:ss, o H:mm:ss pasada la hora.
     */
    void updateTime();
};

#endif
