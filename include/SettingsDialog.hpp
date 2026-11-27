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

#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

// qt
#include <QDialog>

/**
 * @brief Valores con los que se abre el panel de configuración.
 */
struct SettingsValues
{
    int theme;           ///< Índice del tema: 0 oscuro, 1 claro, 2 automático.
    bool alwaysOnTop;    ///< La ventana principal se mantiene encima de las demás.
    bool bandVisible;    ///< Se muestra la franja de lectura.
    int bandPercent;     ///< Posición de la franja, como porcentaje del alto del guion.
    int columnChars;     ///< Ancho de la columna de lectura, en caracteres.
    bool bionic;         ///< Lectura biónica activada.
    bool timerOnPresent; ///< El cronómetro arranca al iniciar la presentación.
    bool vimKeys;        ///< Teclas de vim en lugar de WASD.
};

/**
 * @brief Panel de configuración.
 *
 * Cada cambio se aplica al instante por medio de sus señales, así que no tiene botón de guardar.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const SettingsValues &values, QWidget *parent = nullptr);

signals:
    void alwaysOnTopChanged(bool enabled);
    void bandPositionChanged(int percent);
    void bandVisibleChanged(bool visible);
    void bionicChanged(bool enabled);
    void columnCharsChanged(int chars);
    void themeChanged(int theme);
    void timerOnPresentChanged(bool enabled);
    void vimKeysChanged(bool enabled);
};

#endif
