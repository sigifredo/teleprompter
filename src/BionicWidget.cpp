

// own
#include <BionicWidget.hpp>

// qt
#include <QAbstractTextDocumentLayout>
#include <QFont>
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextFragment>
#include <QTextFrame>
#include <QTextLayout>

namespace
{
    // Margen mínimo a cada lado de la columna de lectura.
    constexpr int MinColumnMargin = 16;

    // Franja de lectura: alto en líneas, opacidad del relleno y ancho de la barra del borde.
    constexpr double BandLines = 1.5;
    constexpr int BandAlpha = 22;
    constexpr int BandBarWidth = 3;

    // Familia del texto del visor; sus archivos viajan como recurso y se registran al arrancar (ver main.cpp).
    const QString FontFamily = QStringLiteral("Atkinson Hyperlegible Next");

    // Cuánto de la página visible avanza scrollPage(), en porcentaje.
    constexpr int PageScrollPercent = 90;
} // namespace

BionicWidget::BionicWidget(QWidget *parent)
    : QTextEdit(parent)
{
    setFont(QFont(FontFamily));
    setReadOnly(true);
    setAcceptDrops(false);
    viewport()->setAcceptDrops(false);
}

int BionicWidget::columnChars() const
{
    return columnChars_;
}

bool BionicWidget::isBionicEnabled() const
{
    return bionic_;
}

bool BionicWidget::isReadingBandVisible() const
{
    return bandVisible_;
}

int BionicWidget::readingBandPosition() const
{
    return bandPercent_;
}

void BionicWidget::scrollPage(int direction)
{
    QScrollBar *bar = verticalScrollBar();

    bar->setValue(bar->value() + direction * bar->pageStep() * PageScrollPercent / 100);
}

void BionicWidget::scrollStep(int direction)
{
    // Se mueve exactamente una línea visual y se deja en la franja, sea cual sea su altura
    // (encabezados, listas y huecos entre párrafos incluidos), así no se salta ninguna.
    const int bandCenter = bandRect().center().y();
    QTextCursor cursor = cursorForPosition(QPoint(viewport()->width() / 2, bandCenter));

    if (!cursor.movePosition(direction > 0 ? QTextCursor::Down : QTextCursor::Up))
        return;

    QScrollBar *bar = verticalScrollBar();

    bar->setValue(bar->value() + cursorRect(cursor).center().y() - bandCenter);
}

void BionicWidget::scrollToEdge(bool end)
{
    QScrollBar *bar = verticalScrollBar();

    bar->setValue(end ? bar->maximum() : bar->minimum());
}

void BionicWidget::setBionicEnabled(bool enabled)
{
    if (enabled == bionic_)
        return;

    bionic_ = enabled;

    // Al volver a pintar el texto el desplazamiento se reinicia, así que se conserva la posición.
    const int value = verticalScrollBar()->value();

    render();
    verticalScrollBar()->setValue(value);
}

void BionicWidget::setBionicMarkdown(const QString &text)
{
    source_ = text;
    markdown_ = true;
    loaded_ = true;
    render();
}

void BionicWidget::setBionicText(const QString &text)
{
    source_ = text;
    markdown_ = false;
    loaded_ = true;
    render();
}

void BionicWidget::setColumnChars(int chars)
{
    columnChars_ = qBound(MinColumnChars, chars, MaxColumnChars);
    updateColumn();
}

void BionicWidget::setFontSize(int fontSize)
{
    QFont newFont = font();

    newFont.setPointSize(fontSize);
    setFont(newFont);
    updateColumn();
    updatePadding();
}

void BionicWidget::setReadingBandPosition(int percent)
{
    bandPercent_ = qBound(MinBandPercent, percent, MaxBandPercent);
    updatePadding();
    viewport()->update();
}

void BionicWidget::setReadingBandVisible(bool visible)
{
    bandVisible_ = visible;
    viewport()->update();
}

void BionicWidget::paintEvent(QPaintEvent *event)
{
    QTextEdit::paintEvent(event);

    if (!bandVisible_)
        return;

    // Guía de lectura: una franja suave por donde pasa el texto, con una barra de acento en el borde.
    QPainter painter(viewport());
    QColor fill = palette().color(QPalette::Text);
    const QRect band = bandRect();

    fill.setAlpha(BandAlpha);
    painter.fillRect(band, fill);
    painter.fillRect(band.x(), band.y(), BandBarWidth, band.height(), palette().color(QPalette::Highlight));
}

void BionicWidget::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    updateColumn();
    updatePadding();
}

QRect BionicWidget::bandRect() const
{
    const int height = static_cast<int>(fontMetrics().lineSpacing() * BandLines);

    return QRect(0, viewport()->height() * bandPercent_ / 100, viewport()->width(), height);
}

int BionicWidget::lineHalfHeight(const QTextBlock &block, bool last) const
{
    // Pedir el rectángulo del bloque hace que el documento lo componga, y así existen sus líneas.
    document()->documentLayout()->blockBoundingRect(block);

    const QTextLayout *layout = block.layout();

    if (layout == nullptr || layout->lineCount() == 0)
        return fontMetrics().lineSpacing() / 2;

    return static_cast<int>(layout->lineAt(last ? layout->lineCount() - 1 : 0).height() / 2);
}

void BionicWidget::render()
{
    if (!loaded_)
        return;

    if (markdown_)
        renderMarkdown();
    else
        renderText();

    updatePadding();
}

void BionicWidget::renderMarkdown()
{
    document()->setMarkdown(source_);

    if (!bionic_)
        return;

    // Primero se reúnen los rangos: cambiar formatos mientras se recorre invalida los fragmentos.
    QList<QPair<int, int>> ranges;
    const QRegularExpression wordRegex("[^\\s]+");

    for (QTextBlock block = document()->begin(); block.isValid(); block = block.next())
    {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it)
        {
            const QTextFragment fragment = it.fragment();
            QRegularExpressionMatchIterator matches = wordRegex.globalMatch(fragment.text());

            while (matches.hasNext())
            {
                const QRegularExpressionMatch match = matches.next();
                const int boldPart = match.capturedLength() / 2;

                if (boldPart > 0)
                    ranges.append(qMakePair(fragment.position() + match.capturedStart(), boldPart));
            }
        }
    }

    QTextCharFormat boldFormat;
    QTextCursor cursor(document());

    boldFormat.setFontWeight(QFont::Bold);
    cursor.beginEditBlock();

    for (const QPair<int, int> &range : ranges)
    {
        cursor.setPosition(range.first);
        cursor.setPosition(range.first + range.second, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(boldFormat);
    }

    cursor.endEditBlock();
}

void BionicWidget::renderText()
{
    QString result;
    QString fixedText(source_);

    fixedText
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\r", "\n")
        .replace("\f", "\n")
        .replace("\v", "\n")
        .replace("\t", " ");

    for (const QString &paragraph : fixedText.split("\n"))
    {
        if (paragraph.isEmpty())
            continue;

        QString newParagraph;

        for (const QString &word : paragraph.split(" "))
        {
            const QString fixedWord = word.trimmed();

            if (fixedWord.isEmpty())
                continue;

            const int boldPart = bionic_ ? fixedWord.length() / 2 : 0;

            if (boldPart > 0)
                newParagraph.append("<strong>" + fixedWord.left(boldPart) + "</strong>" + fixedWord.mid(boldPart) + " ");
            else
                newParagraph.append(fixedWord + " ");
        }

        result.append(newParagraph.trimmed().append("<br><br>"));
    }

    setHtml(result);
}

void BionicWidget::updateColumn()
{
    // Las líneas se mantienen en un ancho cómodo de lectura y se centran.
    const int column = fontMetrics().averageCharWidth() * columnChars_;
    const int available = width() - 2 * frameWidth() - (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0);
    const int margin = qMax(MinColumnMargin, (available - column) / 2);

    setViewportMargins(margin, 0, margin, 0);
}

void BionicWidget::updatePadding()
{
    // El espacio sobre y bajo el texto deja que la primera y la última línea lleguen a la franja.
    // Su tamaño depende de la altura de esas líneas: un encabezado es más alto que una línea de párrafo.
    QTextFrame *root = document()->rootFrame();
    QTextFrameFormat format = root->frameFormat();
    int top = 0;
    int bottom = 0;

    if (!document()->isEmpty())
    {
        const int center = bandRect().center().y();

        top = qMax(0, center - lineHalfHeight(document()->begin(), false));
        bottom = qMax(0, viewport()->height() - center - lineHalfHeight(document()->lastBlock(), true));
    }

    if (format.topMargin() == top && format.bottomMargin() == bottom)
        return;

    format.setTopMargin(top);
    format.setBottomMargin(bottom);
    root->setFrameFormat(format);
}
