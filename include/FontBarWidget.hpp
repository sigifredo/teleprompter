

#ifndef FONTBARWIDGET_HPP
#define FONTBARWIDGET_HPP

// Own
#define DEFAULT_FONT_SIZE 31

// Qt
#include <QWidget>

class QLabel;

class FontBarWidget : public QWidget
{
    Q_OBJECT

public:
    FontBarWidget(QWidget *pParent = nullptr);

    int fontSize() const;

signals:
    void changeFontSize(const int &iFontSize);

protected slots:
    void decrementFontSize();
    void incrementFontSize();

private:
    QLabel *_pFontSizeLabel;
};

#endif
