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

#ifndef CHRONOMETERWIDGET_HPP
#define CHRONOMETERWIDGET_HPP

// Qt
#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;

class ChronometerWidget : public QWidget
{
    Q_OBJECT

public:
    ChronometerWidget(QWidget *pParent = nullptr);

public slots:
    void start();
    void stop();

private:
    QPushButton *_pStartButton;
    QLabel *_pTimeLabel;
    QTimer *_pTimer;
    int _iTimeElapsed;

    void setTimerActiveStyle(const bool &active);

private slots:
    void updateTime();
};

#endif
