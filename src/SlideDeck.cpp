

// own
#include <SlideDeck.hpp>

// qt
#include <QDebug>

#define CLASS_NAME "SlideDeck"
#define TELEPROMPTER_WARNING() qWarning() << "[" CLASS_NAME "]"

SlideDeck::SlideDeck(QObject *parent)
    : QObject(parent)
{
    document_ = new QPdfDocument(this);
}

bool SlideDeck::load(const QString &path)
{
    const QPdfDocument::Error error = document_->load(path);

    if (error != QPdfDocument::Error::None || document_->pageCount() == 0)
    {
        TELEPROMPTER_WARNING() << "No se pudo abrir" << path << "(error" << static_cast<int>(error) << ")";
        return false;
    }

    current_ = 0;
    emit currentChanged(current_);

    return true;
}

int SlideDeck::count() const
{
    return document_->pageCount();
}

int SlideDeck::currentIndex() const
{
    return current_;
}

QImage SlideDeck::render(int index, const QSize &size) const
{
    if (index < 0 || index >= count() || size.isEmpty())
        return {};

    // Se conserva la proporción de la página dentro del tamaño pedido.
    const QSize page = document_->pagePointSize(index).toSize();

    return document_->render(index, page.scaled(size, Qt::KeepAspectRatio));
}

void SlideDeck::next()
{
    setCurrent(current_ + 1);
}

void SlideDeck::previous()
{
    setCurrent(current_ - 1);
}

void SlideDeck::setCurrent(int index)
{
    if (index < 0 || index >= count() || index == current_)
        return;

    current_ = index;
    emit currentChanged(current_);
}
