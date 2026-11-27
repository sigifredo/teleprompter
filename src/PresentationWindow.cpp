

// Own
#include <PresentationWindow.hpp>
#include <SlideDeck.hpp>

// Qt
#include <QCloseEvent>
#include <QPainter>

PresentationWindow::PresentationWindow(SlideDeck *pDeck, QWidget *pParent)
    : QWidget(pParent, Qt::Window), _pDeck(pDeck), _bBlackout(false)
{
    setObjectName("presentation");
    setAttribute(Qt::WA_OpaquePaintEvent);
    setWindowTitle("Presentation");
    setCursor(Qt::BlankCursor);

    connect(_pDeck, SIGNAL(currentChanged(int)), this, SLOT(deckCurrentChanged(int)));
}

void PresentationWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
    emit closed();
}

void PresentationWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (!_bBlackout && !_slide.isNull())
    {
        QSizeF logical = QSizeF(_slide.size()) / _slide.devicePixelRatio();
        QPointF topLeft((width() - logical.width()) / 2, (height() - logical.height()) / 2);

        painter.drawImage(topLeft, _slide);
    }
}

void PresentationWindow::resizeEvent(QResizeEvent *)
{
    updateSlide();
}

void PresentationWindow::updateSlide()
{
    qreal dRatio = devicePixelRatioF();

    _slide = _pDeck->render(_pDeck->currentIndex(), size() * dRatio);
    _slide.setDevicePixelRatio(dRatio);
    update();
}

void PresentationWindow::toggleBlackout()
{
    _bBlackout = !_bBlackout;
    update();
}

void PresentationWindow::deckCurrentChanged(int)
{
    updateSlide();
}
