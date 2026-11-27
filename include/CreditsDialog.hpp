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
 * @file CreditsDialog.hpp
 * @brief Diálogo «Acerca de».
 */

#ifndef CREDITSDIALOG_HPP
#define CREDITSDIALOG_HPP

// qt
#include <QDialog>

/**
 * @brief Diálogo «Acerca de»: versión, autor y licencia.
 *
 * Reparte la información en tres pestañas: About, Author y Licence.
 */
class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Crea el diálogo.
     *
     * @param parent Widget padre.
     */
    explicit CreditsDialog(QWidget *parent = nullptr);
};

#endif
