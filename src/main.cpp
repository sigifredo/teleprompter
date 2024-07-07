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

// Own
#include <MainWindow.hpp>
#include <Version.hpp>

// Qt
#include <QApplication>
#include <QFile>
#include <QScreen>

// std
#include <iostream>

QRect centerGeometry();

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QFile styleFile(":/styles.qss");

    if (styleFile.open(QFile::ReadOnly))
    {
        app.setStyleSheet(QLatin1String(styleFile.readAll()));
    }
    else
    {
        std::cerr << "[Error] Could not open style file." << std::endl;
    }

    MainWindow mw;
    mw.setGeometry(centerGeometry());
    mw.setWindowTitle("Teleprompter v" PROJECT_VERSION);
    mw.show();

    return app.exec();
}

QRect centerGeometry()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int windowWidth = screenWidth * 0.3;
    int windowHeight = screenHeight / 2;

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    return QRect(posX, posY, windowWidth, windowHeight);
}
