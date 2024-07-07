

// Own
#include <ChronometerWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QTimer>

ChronometerWidget::ChronometerWidget(QWidget *pParent)
    : QWidget(pParent), _iTimeElapsed(0)
{
    QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    _pTimer = new QTimer(this);

    _pStartButton = new QPushButton("St&art", this);
    _pTimeLabel = new QLabel("00:00", this);
    {
        _pTimeLabel->setAlignment(Qt::AlignCenter);
        _pTimeLabel->setStyleSheet("font: 24pt;");
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
        _pTimeLabel->setStyleSheet("font: 24pt;");
    }
    else
    {
        _pTimer->start(1000);
        _pStartButton->setText("P&ause");
        _pTimeLabel->setStyleSheet("font: 24pt; color: #a61d1d;");
    }
}

void ChronometerWidget::stop()
{
    if (_pTimer->isActive())
    {
        _pTimer->stop();
        _pTimeLabel->setStyleSheet("font: 24pt;");
    }

    _iTimeElapsed = 0;
    _pTimeLabel->setText("00:00");
}

void ChronometerWidget::updateTime()
{
    QTime time(0, 0);
    QString sFormat("mm:ss");

    _iTimeElapsed++;

    if (_iTimeElapsed >= 3600)
    {
        sFormat = "hh:mm:ss";
    }

    _pTimeLabel->setText(time.addSecs(_iTimeElapsed).toString(sFormat));
}
