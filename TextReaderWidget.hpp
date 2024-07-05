#ifndef TEXTREADERWIDGET_HPP
#define TEXTREADERWIDGET_HPP

#include <QWidget>

class BionicWidget;
class QLabel;

class TextReaderWidget: public QWidget
{
    Q_OBJECT

public:
    TextReaderWidget(QWidget* pParent = nullptr);

public slots:
    void chooseFile();
    void fontSizeChanged(const int& iSize);
    void openFile(const QString& sPath);

private:
    BionicWidget* _pBionicWidget;
    QLabel* _pFontSizeLabel;
};

#endif
