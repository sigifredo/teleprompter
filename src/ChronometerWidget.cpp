

// Own
#include <ChronometerWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QTime>
#include <QTimer>
#include <QVariant>

ChronometerWidget::ChronometerWidget(QWidget *pParent)
    : QWidget(pParent), _iTimeElapsed(0)
{
    QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    _pTimer = new QTimer(this);

    _pStartButton = new QPushButton("St&art", this);
    _pTimeLabel = new QLabel("00:00", this);
    {
        _pTimeLabel->setObjectName("timer");
        _pTimeLabel->setAlignment(Qt::AlignCenter);
    }
    QPushButton *pStopButton = new QPushButton("Sto&p", this);

    connect(_pTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(_pStartButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(pStopButton, SIGNAL(clicked()), this, SLOT(stop()));

    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pLayout->addWidget(_pStartButton);
    pLayout->addWidget(_pTimeLabel);
    pLayout->addWidget(pStopButton);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void ChronometerWidget::start()
{
    if (_pTimer->isActive())
    {
        _pTimer->stop();
        _pStartButton->setText("St&art");
        setTimerActiveStyle(false);
    }
    else
    {
        _pTimer->start(1000);
        _pStartButton->setText("P&ause");
        setTimerActiveStyle(true);
    }
}

void ChronometerWidget::stop()
{
    if (_pTimer->isActive())
    {
        _pTimer->stop();
        _pStartButton->setText("St&art");
        setTimerActiveStyle(false);
    }

    _iTimeElapsed = 0;
    _pTimeLabel->setText("00:00");
}

void ChronometerWidget::setTimerActiveStyle(const bool &active)
{
    QString value("false");

    if (active)
    {
        value = "true";
    }

    _pTimeLabel->setProperty("active", value);
    style()->unpolish(_pTimeLabel);
    style()->polish(_pTimeLabel);
}

void ChronometerWidget::updateTime()
{
    QTime time(0, 0);
    QString sFormat("mm:ss");

    _iTimeElapsed++;

    if (_iTimeElapsed >= 3600)
    {
        sFormat = "H:mm:ss";
    }

    _pTimeLabel->setText(time.addSecs(_iTimeElapsed).toString(sFormat));
}
