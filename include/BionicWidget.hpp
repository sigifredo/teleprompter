

#ifndef BIONICWIDGET_HPP
#define BIONICWIDGET_HPP

// Own
#define DEFAULT_FONT_SIZE 31

// Qt
#include <QTextEdit>

class BionicWidget : public QTextEdit
{
    Q_OBJECT

public:
    BionicWidget(QWidget *pParent = nullptr);

public slots:
    void decrementFontSize();
    void incrementFontSize();
    void setBionicText(const QString &sText);

signals:
    void fontSizeChanged(const int &iSize);
};

#endif
