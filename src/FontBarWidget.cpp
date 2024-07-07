

// Own
#include <FontBarWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

FontBarWidget::FontBarWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    QPushButton *pPlusButton = new QPushButton("&+", this);
    _pFontSizeLabel = new QLabel("0", this);
    QPushButton *pMinusButton = new QPushButton("&-", this);

    _pFontSizeLabel->setObjectName("fontSizeLabel");
    _pFontSizeLabel->setText(QString::number(DEFAULT_FONT_SIZE));

    connect(pPlusButton, SIGNAL(clicked()), this, SLOT(incrementFontSize()));
    connect(pMinusButton, SIGNAL(clicked()), this, SLOT(decrementFontSize()));

    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pLayout->addWidget(pPlusButton);
    pLayout->addWidget(_pFontSizeLabel);
    pLayout->addWidget(pMinusButton);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

int FontBarWidget::fontSize() const
{
    return _pFontSizeLabel->text().toInt();
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
