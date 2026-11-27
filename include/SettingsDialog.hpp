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

// Qt
#include <QDialog>

struct SettingsValues
{
    int theme;
    bool alwaysOnTop;
    bool bandVisible;
    int bandPercent;
    int columnChars;
    bool bionic;
    bool timerOnPresent;
};

/**
 * Settings panel. Every change is applied right away through its signals, so it has no Save button.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(const SettingsValues &values, QWidget *pParent = nullptr);

signals:
    void themeChanged(int iTheme);
    void alwaysOnTopChanged(bool bEnabled);
    void bandVisibleChanged(bool bVisible);
    void bandPositionChanged(int iPercent);
    void columnCharsChanged(int iChars);
    void bionicChanged(bool bEnabled);
    void timerOnPresentChanged(bool bEnabled);
};

#endif
