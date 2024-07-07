

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
    void setFontSize(const int &iFontSize);
};

#endif
