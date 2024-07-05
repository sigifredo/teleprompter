#ifndef BIONICWIDGET_HPP
#define BIONICWIDGET_HPP

#include <QTextEdit>

class BionicWidget: public QTextEdit
{
    Q_OBJECT

public:
    BionicWidget(QWidget* pParent = nullptr);

public slots:
    void decrementFontSize();
    void incrementFontSize();
    void setBionicText(const QString& sText);

private:
    QString _sRawText;
};

#endif
