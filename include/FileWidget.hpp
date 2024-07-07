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

#ifndef FILEWIDGET_HPP
#define FILEWIDGET_HPP

// Qt
#include <QWidget>

class QLabel;

class FileWidget : public QWidget
{
    Q_OBJECT

public:
    FileWidget(QWidget *pParent = nullptr);

    void setFileName(const QString &sFileName);

signals:
    void fileSelected(const QString &sFilePath);

public slots:
    void chooseFile();

private:
    QLabel *_pFileNameLabel;
};

#endif
