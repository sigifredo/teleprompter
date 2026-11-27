

// own
#include <SlideStripWidget.hpp>
#include <SlideDeck.hpp>

// qt
#include <QPixmap>

namespace
{
    // Tamaño de las miniaturas, y alto que ocupan el número, los márgenes y la barra de desplazamiento.
    constexpr int ThumbnailWidth = 160;
    constexpr int ThumbnailHeight = 90;
    constexpr int ChromeHeight = 56;

    // Las miniaturas se dibujan al doble para que se vean nítidas en pantallas de alta densidad.
    constexpr int RenderScale = 2;
} // namespace

SlideStripWidget::SlideStripWidget(QWidget *parent)
    : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setFlow(QListView::LeftToRight);
    setWrapping(false);
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setIconSize(QSize(ThumbnailWidth, ThumbnailHeight));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setFixedHeight(ThumbnailHeight + ChromeHeight);

    connect(this, &QListWidget::currentRowChanged, this, &SlideStripWidget::rowActivated);
}

void SlideStripWidget::setDeck(SlideDeck *deck)
{
    deck_ = deck;

    blockSignals(true);
    clear();

    for (int i = 0; i < deck->count(); i++)
    {
        const QPixmap pixmap = QPixmap::fromImage(deck->render(i, QSize(ThumbnailWidth, ThumbnailHeight) * RenderScale));
        QIcon icon;

        // Con el mismo pixmap en el estado seleccionado, Qt no tiñe de gris la miniatura elegida.
        icon.addPixmap(pixmap, QIcon::Normal);
        icon.addPixmap(pixmap, QIcon::Selected);

        auto *item = new QListWidgetItem(icon, QString::number(i + 1), this);

        item->setTextAlignment(Qt::AlignHCenter);
    }

    blockSignals(false);

    connect(deck, &SlideDeck::currentChanged, this, &SlideStripWidget::deckCurrentChanged, Qt::UniqueConnection);
    deckCurrentChanged(deck->currentIndex());
}

void SlideStripWidget::deckCurrentChanged(int index)
{
    // Se bloquean las señales para no devolverle el cambio al SlideDeck.
    blockSignals(true);
    setCurrentRow(index);
    blockSignals(false);

    if (currentItem() != nullptr)
        scrollToItem(currentItem(), QAbstractItemView::PositionAtCenter);
}

void SlideStripWidget::rowActivated(int row)
{
    if (deck_ != nullptr && row >= 0)
        deck_->setCurrent(row);
}
