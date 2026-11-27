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
 * @file SettingsDialog.hpp
 * @brief Panel de configuración.
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
    bool alwaysOnTop;    ///< La ventana principal se mantiene encima de las demás.
    int bandPercent;     ///< Posición de la franja, como porcentaje del alto del guion.
    bool bandVisible;    ///< Se muestra la franja de lectura.
    bool bionic;         ///< Lectura biónica activada.
    int columnChars;     ///< Ancho de la columna de lectura, en caracteres.
    int theme;           ///< Índice del tema: 0 oscuro, 1 claro, 2 automático.
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
    /**
     * @brief Crea el panel con los valores actuales.
     *
     * @param values Valores con los que se abre.
     * @param parent Widget padre.
     */
    explicit SettingsDialog(const SettingsValues &values, QWidget *parent = nullptr);

signals:
    /**
     * @brief Se emite al cambiar si la ventana principal se mantiene encima de las demás.
     *
     * @param enabled true si se mantiene encima.
     */
    void alwaysOnTopChanged(bool enabled);

    /**
     * @brief Se emite al mover la posición de la franja de lectura.
     *
     * @param percent Porcentaje del alto del guion, de BionicWidget::MinBandPercent a BionicWidget::MaxBandPercent.
     */
    void bandPositionChanged(int percent);

    /**
     * @brief Se emite al mostrar u ocultar la franja de lectura.
     *
     * @param visible true si se muestra.
     */
    void bandVisibleChanged(bool visible);

    /**
     * @brief Se emite al activar o desactivar la lectura biónica.
     *
     * @param enabled true si está activada.
     */
    void bionicChanged(bool enabled);

    /**
     * @brief Se emite al cambiar el ancho de la columna de lectura.
     *
     * @param chars Ancho en caracteres, de BionicWidget::MinColumnChars a BionicWidget::MaxColumnChars.
     */
    void columnCharsChanged(int chars);

    /**
     * @brief Se emite al cambiar el tema.
     *
     * @param theme Índice del tema: 0 oscuro, 1 claro, 2 automático.
     */
    void themeChanged(int theme);

    /**
     * @brief Se emite al cambiar si el cronómetro arranca al iniciar la presentación.
     *
     * @param enabled true si arranca.
     */
    void timerOnPresentChanged(bool enabled);

    /**
     * @brief Se emite al cambiar entre las teclas de vim y WASD.
     *
     * @param enabled true para las teclas de vim.
     */
    void vimKeysChanged(bool enabled);
};

#endif
