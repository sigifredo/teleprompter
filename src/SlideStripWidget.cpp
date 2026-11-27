

// Own
#include <SlideStripWidget.hpp>
#include <SlideDeck.hpp>

// Qt
#include <QPixmap>

#define THUMBNAIL_WIDTH 160
#define THUMBNAIL_HEIGHT 90

SlideStripWidget::SlideStripWidget(QWidget *pParent)
    : QListWidget(pParent), _pDeck(nullptr)
{
    setViewMode(QListView::IconMode);
    setFlow(QListView::LeftToRight);
    setWrapping(false);
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setIconSize(QSize(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setFixedHeight(THUMBNAIL_HEIGHT + 56);

    connect(this, SIGNAL(currentRowChanged(int)), this, SLOT(rowActivated(int)));
}

void SlideStripWidget::setDeck(SlideDeck *pDeck)
{
    _pDeck = pDeck;

    blockSignals(true);
    clear();

    for (int i = 0; i < pDeck->count(); i++)
    {
        QImage image = pDeck->render(i, QSize(THUMBNAIL_WIDTH * 2, THUMBNAIL_HEIGHT * 2));
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(QPixmap::fromImage(image)), QString::number(i + 1), this);

        pItem->setTextAlignment(Qt::AlignHCenter);
    }

    blockSignals(false);

    connect(pDeck, SIGNAL(currentChanged(int)), this, SLOT(deckCurrentChanged(int)), Qt::UniqueConnection);
    deckCurrentChanged(pDeck->currentIndex());
}

void SlideStripWidget::deckCurrentChanged(int iIndex)
{
    // Avoid feeding the change back to the deck.
    blockSignals(true);
    setCurrentRow(iIndex);
    blockSignals(false);

    if (currentItem() != nullptr)
    {
        scrollToItem(currentItem(), QAbstractItemView::PositionAtCenter);
    }
}

void SlideStripWidget::rowActivated(int iRow)
{
    if (_pDeck != nullptr && iRow >= 0)
    {
        _pDeck->setCurrent(iRow);
    }
}
