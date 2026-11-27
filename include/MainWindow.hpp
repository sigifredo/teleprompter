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
class ChronometerWidget;
class FileWidget;
class FontBarWidget;
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

    bool hasSavedGeometry() const;

public slots:
    void openFile(const QString &sFilePath);
    void openSlides(const QString &sFilePath);
    void startPresentation();
    void stopPresentation();

protected:
    void closeEvent(QCloseEvent *pEvent) override;
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;
    bool eventFilter(QObject *pObject, QEvent *pEvent) override;

private:
    BionicWidget *_pBionicWidget;
    ChronometerWidget *_pChronometer;
    FileWidget *_pFileWidget;
    FileWidget *_pSlidesFileWidget;
    FontBarWidget *_pFontBar;
    SlideDeck *_pDeck;
    SlideStripWidget *_pSlideStrip;
    QLabel *_pSlideCounterLabel;
    QPushButton *_pPresentButton;
    PresentationWindow *_pPresentation;
    QString _sGoToBuffer;
    bool _bAlwaysOnTop;
    bool _bStartTimerOnPresent;
    bool _bVimKeys;
    QString _sScriptPath;
    QString _sSlidesPath;

    int translateKey(int iKey) const;
    bool handleSlideKey(int iKey, bool bPlain);
    bool handleScriptKey(int iKey, Qt::KeyboardModifiers modifiers, const QString &sText);
    void loadSettings();
    void saveSettings();

private slots:
    void openSettings();
    void setAlwaysOnTop(bool bEnabled);
    void showHelp();
    void updateSlideCounter();
};

#endif
