

// own
#include <SlidePreviewWidget.hpp>
#include <SlideDeck.hpp>

// qt
#include <QPainter>

namespace
{
    // Alto del título y tamaño mínimo que debe tener el área de la diapositiva.
    constexpr int CaptionHeight = 22;
    constexpr int MinSlideWidth = 200;
    constexpr int MinSlideHeight = 112;
    constexpr int BorderRadius = 6;
} // namespace

SlidePreviewWidget::SlidePreviewWidget(SlideDeck *deck, int offset, const QString &caption, QWidget *parent)
    : QWidget(parent), deck_(deck), offset_(offset), caption_(caption)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(deck_, &SlideDeck::currentChanged, this, &SlidePreviewWidget::deckCurrentChanged);
}

QSize SlidePreviewWidget::minimumSizeHint() const
{
    return QSize(MinSlideWidth, MinSlideHeight + CaptionHeight);
}

void SlidePreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const QRect area = slideArea();

    QFont captionFont = font();

    captionFont.setBold(true);
    painter.setFont(captionFont);
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(QRect(0, 0, width(), CaptionHeight), Qt::AlignLeft | Qt::AlignVCenter, caption_);
    painter.setFont(font());

    if (image_.isNull())
    {
        painter.setPen(QPen(palette().color(QPalette::Mid), 1, Qt::DashLine));
        painter.drawRoundedRect(area.adjusted(0, 0, -1, -1), BorderRadius, BorderRadius);
        painter.setPen(palette().color(QPalette::PlaceholderText));
        painter.drawText(area, Qt::AlignCenter, deck_->count() == 0 ? tr("No slides") : tr("End of slides"));

        return;
    }

    const QSizeF logical = QSizeF(image_.size()) / image_.devicePixelRatio();
    const QPointF topLeft(area.x() + (area.width() - logical.width()) / 2, area.y() + (area.height() - logical.height()) / 2);

    painter.drawImage(topLeft, image_);
    painter.setPen(palette().color(QPalette::Mid));
    painter.drawRect(QRectF(topLeft, logical).adjusted(-0.5, -0.5, 0.5, 0.5));
}

void SlidePreviewWidget::resizeEvent(QResizeEvent *)
{
    updateImage();
}

void SlidePreviewWidget::deckCurrentChanged(int)
{
    updateImage();
}

QRect SlidePreviewWidget::slideArea() const
{
    return QRect(0, CaptionHeight, width(), height() - CaptionHeight);
}

void SlidePreviewWidget::updateImage()
{
    const qreal ratio = devicePixelRatioF();

    image_ = deck_->render(deck_->currentIndex() + offset_, slideArea().size() * ratio);

    if (!image_.isNull())
        image_.setDevicePixelRatio(ratio);

    update();
}
