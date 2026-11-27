

// Own
#include <SlidePreviewWidget.hpp>
#include <SlideDeck.hpp>

// Qt
#include <QPainter>

#define CAPTION_HEIGHT 22

SlidePreviewWidget::SlidePreviewWidget(SlideDeck *pDeck, int iOffset, const QString &sCaption, QWidget *pParent)
    : QWidget(pParent), _pDeck(pDeck), _iOffset(iOffset), _sCaption(sCaption)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(_pDeck, SIGNAL(currentChanged(int)), this, SLOT(deckCurrentChanged(int)));
}

QSize SlidePreviewWidget::minimumSizeHint() const
{
    return QSize(200, 112 + CAPTION_HEIGHT);
}

QRect SlidePreviewWidget::slideArea() const
{
    return QRect(0, CAPTION_HEIGHT, width(), height() - CAPTION_HEIGHT);
}

void SlidePreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor muted = palette().color(QPalette::PlaceholderText);
    QRect area = slideArea();

    QFont captionFont = font();

    captionFont.setBold(true);
    painter.setFont(captionFont);
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(QRect(0, 0, width(), CAPTION_HEIGHT), Qt::AlignLeft | Qt::AlignVCenter, _sCaption);
    painter.setFont(font());

    if (_image.isNull())
    {
        painter.setPen(QPen(palette().color(QPalette::Mid), 1, Qt::DashLine));
        painter.drawRoundedRect(area.adjusted(0, 0, -1, -1), 6, 6);
        painter.setPen(muted);
        painter.drawText(area, Qt::AlignCenter, _pDeck->count() == 0 ? "No slides" : "End of slides");

        return;
    }

    QSizeF logical = QSizeF(_image.size()) / _image.devicePixelRatio();
    QPointF topLeft(area.x() + (area.width() - logical.width()) / 2, area.y() + (area.height() - logical.height()) / 2);

    painter.drawImage(topLeft, _image);
    painter.setPen(palette().color(QPalette::Mid));
    painter.drawRect(QRectF(topLeft, logical).adjusted(-0.5, -0.5, 0.5, 0.5));
}

void SlidePreviewWidget::resizeEvent(QResizeEvent *)
{
    updateImage();
}

void SlidePreviewWidget::updateImage()
{
    qreal dRatio = devicePixelRatioF();

    _image = _pDeck->render(_pDeck->currentIndex() + _iOffset, slideArea().size() * dRatio);

    if (!_image.isNull())
    {
        _image.setDevicePixelRatio(dRatio);
    }

    update();
}

void SlidePreviewWidget::deckCurrentChanged(int)
{
    updateImage();
}
