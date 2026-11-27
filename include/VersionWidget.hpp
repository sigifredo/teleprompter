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
 * @file VersionWidget.hpp
 * @brief Botón con la versión de la aplicación.
 */

#ifndef VERSIONWIDGET_HPP
#define VERSIONWIDGET_HPP

// qt
#include <QWidget>

/**
 * @brief Botón con la versión de la aplicación; abre el diálogo de créditos.
 */
class VersionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Crea el botón con la versión de la aplicación.
     *
     * @param parent Widget padre.
     */
    explicit VersionWidget(QWidget *parent = nullptr);

public slots:
    /**
     * @brief Abre el diálogo de créditos y espera a que se cierre.
     */
    void showCredits();
};

#endif
