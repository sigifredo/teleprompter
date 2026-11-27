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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class BionicWidget;
class FileWidget;
class PresentationWindow;
class QLabel;
class QPushButton;
class SlideDeck;
class SlideStripWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *pParent = nullptr);

public slots:
    void fontSizeChanged(const int &iSize);
    void openFile(const QString &sFilePath);
    void openSlides(const QString &sFilePath);
    void startPresentation();
    void stopPresentation();

protected:
    bool eventFilter(QObject *pObject, QEvent *pEvent) override;

private:
    BionicWidget *_pBionicWidget;
    QLabel *_pFontSizeLabel;
    FileWidget *_pFileWidget;
    FileWidget *_pSlidesFileWidget;
    SlideDeck *_pDeck;
    SlideStripWidget *_pSlideStrip;
    QLabel *_pSlideCounterLabel;
    QPushButton *_pPresentButton;
    PresentationWindow *_pPresentation;

private slots:
    void updateSlideCounter();
};

#endif
