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

#ifndef FONTBARWIDGET_HPP
#define FONTBARWIDGET_HPP

// Own
#define DEFAULT_FONT_SIZE 31

// Qt
#include <QWidget>

class QLabel;

class FontBarWidget : public QWidget
{
    Q_OBJECT

public:
    FontBarWidget(QWidget *pParent = nullptr);

    int fontSize() const;

signals:
    void changeFontSize(const int &iFontSize);

protected slots:
    void decrementFontSize();
    void incrementFontSize();

private:
    QLabel *_pFontSizeLabel;
};

#endif
