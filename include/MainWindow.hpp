

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class BionicWidget;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *pParent = nullptr);

public slots:
    void chooseFile();
    void fontSizeChanged(const int &iSize);
    void openFile(const QString &sPath);

private:
    BionicWidget *_pBionicWidget;
    QLabel *_pFileNameLabel;
    QLabel *_pFontSizeLabel;
};

#endif
