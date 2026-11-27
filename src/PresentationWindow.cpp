

// own
#include <PresentationWindow.hpp>
#include <SlideDeck.hpp>

// qt
#include <QCloseEvent>
#include <QPainter>

PresentationWindow::PresentationWindow(SlideDeck *deck, QWidget *parent)
    : QWidget(parent, Qt::Window), deck_(deck)
{
    setObjectName("presentation");
    setAttribute(Qt::WA_OpaquePaintEvent);
    setWindowTitle(tr("Presentation"));
    setCursor(Qt::BlankCursor);

    connect(deck_, &SlideDeck::currentChanged, this, &PresentationWindow::deckCurrentChanged);
}

void PresentationWindow::toggleBlackout()
{
    blackout_ = !blackout_;
    update();
}

void PresentationWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closed();
}

void PresentationWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    if (blackout_ || slide_.isNull())
        return;

    const QSizeF logical = QSizeF(slide_.size()) / slide_.devicePixelRatio();

    painter.drawImage(QPointF((width() - logical.width()) / 2, (height() - logical.height()) / 2), slide_);
}

void PresentationWindow::resizeEvent(QResizeEvent *)
{
    updateSlide();
}

void PresentationWindow::deckCurrentChanged(int)
{
    updateSlide();
}

void PresentationWindow::updateSlide()
{
    const qreal ratio = devicePixelRatioF();

    slide_ = deck_->render(deck_->currentIndex(), size() * ratio);
    slide_.setDevicePixelRatio(ratio);
    update();
}
