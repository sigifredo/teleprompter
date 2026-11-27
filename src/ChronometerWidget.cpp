

// own
#include <ChronometerWidget.hpp>

// qt
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QTime>
#include <QTimer>
#include <QVariant>

namespace
{
    // Intervalo del cronómetro y segundos a partir de los cuales se muestran las horas.
    constexpr int TickInterval = 1000;
    constexpr int SecondsPerHour = 3600;
} // namespace

ChronometerWidget::ChronometerWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    layout->setContentsMargins(0, 0, 0, 0);

    timer_ = new QTimer(this);
    startButton_ = new QPushButton(tr("St&art"), this);
    timeLabel_ = new QLabel(QStringLiteral("00:00"), this);
    timeLabel_->setProperty("role", "timer");
    timeLabel_->setAlignment(Qt::AlignCenter);

    auto *stopButton = new QPushButton(tr("Sto&p"), this);

    connect(timer_, &QTimer::timeout, this, &ChronometerWidget::updateTime);
    connect(startButton_, &QPushButton::clicked, this, &ChronometerWidget::start);
    connect(stopButton, &QPushButton::clicked, this, &ChronometerWidget::stop);

    layout->addWidget(startButton_);
    layout->addWidget(timeLabel_);
    layout->addWidget(stopButton);
}

bool ChronometerWidget::isRunning() const
{
    return timer_->isActive();
}

void ChronometerWidget::start()
{
    if (timer_->isActive())
    {
        timer_->stop();
        startButton_->setText(tr("St&art"));
        setActiveStyle(false);
    }
    else
    {
        timer_->start(TickInterval);
        startButton_->setText(tr("P&ause"));
        setActiveStyle(true);
    }
}

void ChronometerWidget::stop()
{
    if (timer_->isActive())
    {
        timer_->stop();
        startButton_->setText(tr("St&art"));
        setActiveStyle(false);
    }

    timeElapsed_ = 0;
    timeLabel_->setText(QStringLiteral("00:00"));
}

void ChronometerWidget::setActiveStyle(bool active)
{
    timeLabel_->setProperty("active", active ? "true" : "false");
    style()->unpolish(timeLabel_);
    style()->polish(timeLabel_);
}

void ChronometerWidget::updateTime()
{
    const QTime zero(0, 0);

    timeElapsed_++;

    const QString format = timeElapsed_ >= SecondsPerHour ? QStringLiteral("H:mm:ss") : QStringLiteral("mm:ss");

    timeLabel_->setText(zero.addSecs(timeElapsed_).toString(format));
}
