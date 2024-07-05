#ifndef TEXTREADERWIDGET_HPP
#define TEXTREADERWIDGET_HPP

#include <QWidget>

class BionicWidget;

class TextReaderWidget: public QWidget
{
    Q_OBJECT

public:
    TextReaderWidget(QWidget* pParent = nullptr);

public slots:
    void chooseFile();
    void openFile(const QString& sPath);

private:
    BionicWidget* _pBionicWidget;
};

#endif
