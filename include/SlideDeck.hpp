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

#ifndef SLIDEDECK_HPP
#define SLIDEDECK_HPP

// Qt
#include <QImage>
#include <QObject>
#include <QPdfDocument>

class SlideDeck : public QObject
{
    Q_OBJECT

public:
    SlideDeck(QObject *pParent = nullptr);

    bool load(const QString &sPath);
    int count() const;
    int currentIndex() const;
    QImage render(int iIndex, const QSize &size) const;

signals:
    void currentChanged(int iIndex);

public slots:
    void next();
    void previous();
    void setCurrent(int iIndex);

private:
    QPdfDocument *_pDocument;
    int _iCurrent;
};

#endif
