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

#ifndef THEME_HPP
#define THEME_HPP

// Qt
#include <QObject>

class Theme : public QObject
{
    Q_OBJECT

public:
    enum Mode
    {
        Dark = 0,
        Light = 1,
        Auto = 2
    };

    static Theme &instance();

    Mode mode() const;
    bool isDark() const;
    QString modeName() const;

    void load();

public slots:
    void setMode(Mode mode);
    void cycle();

signals:
    void changed();

private:
    Theme();

    Mode _mode;

    void apply();
};

#endif
