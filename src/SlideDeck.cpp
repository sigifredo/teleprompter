

// Own
#include <SlideDeck.hpp>

SlideDeck::SlideDeck(QObject *pParent)
    : QObject(pParent), _iCurrent(0)
{
    _pDocument = new QPdfDocument(this);
}

bool SlideDeck::load(const QString &sPath)
{
    if (_pDocument->load(sPath) != QPdfDocument::Error::None || _pDocument->pageCount() == 0)
    {
        return false;
    }

    _iCurrent = 0;
    emit currentChanged(_iCurrent);

    return true;
}

int SlideDeck::count() const
{
    return _pDocument->pageCount();
}

int SlideDeck::currentIndex() const
{
    return _iCurrent;
}

QImage SlideDeck::render(int iIndex, const QSize &size) const
{
    if (iIndex < 0 || iIndex >= count() || size.isEmpty())
    {
        return QImage();
    }

    // Keep the page aspect ratio inside the requested bounds.
    QSizeF pageSize = _pDocument->pagePointSize(iIndex);
    QSize target = pageSize.toSize().scaled(size, Qt::KeepAspectRatio);

    return _pDocument->render(iIndex, target);
}

void SlideDeck::next()
{
    setCurrent(_iCurrent + 1);
}

void SlideDeck::previous()
{
    setCurrent(_iCurrent - 1);
}

void SlideDeck::setCurrent(int iIndex)
{
    if (iIndex < 0 || iIndex >= count() || iIndex == _iCurrent)
    {
        return;
    }

    _iCurrent = iIndex;
    emit currentChanged(_iCurrent);
}
