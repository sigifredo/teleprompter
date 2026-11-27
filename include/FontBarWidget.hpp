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
 * @file FontBarWidget.hpp
 * @brief Barra para cambiar el tamaño de la letra.
 */

#ifndef FONTBARWIDGET_HPP
#define FONTBARWIDGET_HPP

// qt
#include <QWidget>

class QLabel;

/**
 * @brief Barra con los botones para agrandar y achicar la letra del guion.
 *
 * Cada pulsación cambia el tamaño en 2 puntos, y el tamaño no baja de 2.
 */
class FontBarWidget : public QWidget
{
    Q_OBJECT

public:
    static constexpr int DefaultFontSize = 31; ///< Tamaño de letra inicial, en puntos.

    /**
     * @brief Crea la barra con el tamaño inicial DefaultFontSize.
     *
     * @param parent Widget padre.
     */
    explicit FontBarWidget(QWidget *parent = nullptr);

    /**
     * @brief Tamaño de letra actual.
     *
     * @return El tamaño, en puntos.
     */
    int fontSize() const;

    /**
     * @brief Fija el tamaño de letra y emite changeFontSize().
     *
     * @param fontSize Tamaño en puntos.
     */
    void setFontSize(int fontSize);

public slots:
    /**
     * @brief Reduce el tamaño de letra en 2 puntos, sin bajar de 2.
     */
    void decrementFontSize();

    /**
     * @brief Aumenta el tamaño de letra en 2 puntos.
     */
    void incrementFontSize();

signals:
    /**
     * @brief Se emite cuando se fija un tamaño de letra.
     *
     * @param fontSize Tamaño en puntos.
     */
    void changeFontSize(int fontSize);

private:
    QLabel *fontSizeLabel_; ///< Etiqueta con el tamaño actual.
};

#endif
