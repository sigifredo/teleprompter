

// Own
#include <FontBarWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

FontBarWidget::FontBarWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    pLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *pPlusButton = new QPushButton("A+", this);
    _pFontSizeLabel = new QLabel("0", this);
    QPushButton *pMinusButton = new QPushButton("A-", this);

    _pFontSizeLabel->setObjectName("fontSizeLabel");
    _pFontSizeLabel->setText(QString::number(DEFAULT_FONT_SIZE));

    pPlusButton->setToolTip("Increase font size (Ctrl++)");
    pMinusButton->setToolTip("Decrease font size (Ctrl+-)");

    connect(pPlusButton, SIGNAL(clicked()), this, SLOT(incrementFontSize()));
    connect(pMinusButton, SIGNAL(clicked()), this, SLOT(decrementFontSize()));

    pLayout->addWidget(pMinusButton);
    pLayout->addWidget(_pFontSizeLabel);
    pLayout->addWidget(pPlusButton);
}

int FontBarWidget::fontSize() const
{
    return _pFontSizeLabel->text().toInt();
}

void FontBarWidget::setFontSize(int iFontSize)
{
    _pFontSizeLabel->setText(QString::number(iFontSize));
    emit changeFontSize(iFontSize);
}

void FontBarWidget::decrementFontSize()
{
    int iNewFontSize = _pFontSizeLabel->text().toInt();

    if (iNewFontSize > 2)
    {
        iNewFontSize -= 2;
    }

    _pFontSizeLabel->setText(QString::number(iNewFontSize));
    emit changeFontSize(iNewFontSize);
}

void FontBarWidget::incrementFontSize()
{
    int iNewFontSize = _pFontSizeLabel->text().toInt() + 2;
    _pFontSizeLabel->setText(QString::number(iNewFontSize));
    emit changeFontSize(iNewFontSize);
}
