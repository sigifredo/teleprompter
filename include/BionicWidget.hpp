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

#ifndef BIONICWIDGET_HPP
#define BIONICWIDGET_HPP

// Qt
#include <QTextEdit>

class BionicWidget : public QTextEdit
{
    Q_OBJECT

public:
    BionicWidget(QWidget *pParent = nullptr);

public slots:
    void setBionicText(const QString &sText);
    void setBionicMarkdown(const QString &sText);
    void scrollStep(int iDirection);
    void scrollPage(int iDirection);
    void scrollToEdge(bool bEnd);
    void setFontSize(const int &iFontSize);

protected:
    void paintEvent(QPaintEvent *pEvent) override;
    void resizeEvent(QResizeEvent *pEvent) override;

private:
    void updateColumn();
};

#endif
