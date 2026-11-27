

// Own
#include <BionicWidget.hpp>

// Qt
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextCursor>
#include <QTextFrame>
#include <QTextFragment>

#define COLUMN_CHARS 62

BionicWidget::BionicWidget(QWidget *pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);
    setAcceptDrops(false);
    viewport()->setAcceptDrops(false);
}

void BionicWidget::setBionicText(const QString &sText)
{
    QString sResult;
    QString sFixedText(sText);

    sFixedText
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\r", "\n")
        .replace("\f", "\n")
        .replace("\v", "\n")
        .replace("\t", " ");

    QStringList paragraphs = sFixedText.split("\n");

    for (const QString &paragraph : paragraphs)
    {
        if (!paragraph.isEmpty())
        {
            QString sNewParagraph;
            QStringList words = paragraph.split(" ");

            for (const QString &sWord : words)
            {
                QString sFixedWord = sWord.trimmed();

                if (!sFixedWord.isEmpty())
                {
                    int iLen = sFixedWord.length();
                    int iBoldPart = iLen / 2;

                    sNewParagraph.append("<strong>" + sFixedWord.left(iBoldPart) + "</strong>" + sFixedWord.mid(iBoldPart) + " ");
                }
            }

            sResult.append(sNewParagraph.trimmed().append("<br><br>"));
        }
    }

    setHtml(sResult);
    updatePadding();
}

void BionicWidget::setBionicMarkdown(const QString &sText)
{
    document()->setMarkdown(sText);

    // Collect the ranges first: changing formats while iterating invalidates fragments.
    QList<QPair<int, int>> ranges;
    QRegularExpression wordRegex("[^\\s]+");

    for (QTextBlock block = document()->begin(); block.isValid(); block = block.next())
    {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it)
        {
            QTextFragment fragment = it.fragment();
            QRegularExpressionMatchIterator matches = wordRegex.globalMatch(fragment.text());

            while (matches.hasNext())
            {
                QRegularExpressionMatch match = matches.next();
                int iBoldPart = match.capturedLength() / 2;

                if (iBoldPart > 0)
                {
                    ranges.append(qMakePair(fragment.position() + match.capturedStart(), iBoldPart));
                }
            }
        }
    }

    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);

    QTextCursor cursor(document());
    cursor.beginEditBlock();

    for (const QPair<int, int> &range : ranges)
    {
        cursor.setPosition(range.first);
        cursor.setPosition(range.first + range.second, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(boldFormat);
    }

    cursor.endEditBlock();
    updatePadding();
}

void BionicWidget::scrollStep(int iDirection)
{
    // Move exactly one visual line and put it in the reading band, whatever its height
    // (headings, lists and paragraph gaps included), so no line is ever skipped.
    int iBandCenter = bandRect().center().y();
    QTextCursor cursor = cursorForPosition(QPoint(viewport()->width() / 2, iBandCenter));

    if (!cursor.movePosition(iDirection > 0 ? QTextCursor::Down : QTextCursor::Up))
    {
        return;
    }

    QScrollBar *pBar = verticalScrollBar();

    pBar->setValue(pBar->value() + cursorRect(cursor).center().y() - iBandCenter);
}

void BionicWidget::setFontSize(const int &iFontSize)
{
    QFont defaultFont = font();
    defaultFont.setPointSize(iFontSize);

    setFont(defaultFont);
    updateColumn();
    updatePadding();
}

void BionicWidget::scrollPage(int iDirection)
{
    QScrollBar *pBar = verticalScrollBar();

    pBar->setValue(pBar->value() + iDirection * pBar->pageStep() * 9 / 10);
}

void BionicWidget::scrollToEdge(bool bEnd)
{
    QScrollBar *pBar = verticalScrollBar();

    pBar->setValue(bEnd ? pBar->maximum() : pBar->minimum());
}

void BionicWidget::paintEvent(QPaintEvent *pEvent)
{
    QTextEdit::paintEvent(pEvent);

    // Reading guide: a soft band where the eyes should rest while the text flows through it.
    QPainter painter(viewport());
    QColor color = palette().color(QPalette::Highlight);
    QRect band = bandRect();

    color.setAlpha(34);
    painter.fillRect(band, color);
    color.setAlpha(200);
    painter.fillRect(band.x(), band.y(), 3, band.height(), color);
}

QRect BionicWidget::bandRect() const
{
    int iHeight = static_cast<int>(fontMetrics().lineSpacing() * 1.5);

    return QRect(0, viewport()->height() * 3 / 10, viewport()->width(), iHeight);
}

void BionicWidget::resizeEvent(QResizeEvent *pEvent)
{
    QTextEdit::resizeEvent(pEvent);
    updateColumn();
    updatePadding();
}

void BionicWidget::updateColumn()
{
    // Keep lines to a comfortable reading width and center them.
    int iColumn = fontMetrics().averageCharWidth() * COLUMN_CHARS;
    int iAvailable = width() - 2 * frameWidth() - (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0);
    int iMargin = qMax(16, (iAvailable - iColumn) / 2);

    setViewportMargins(iMargin, 0, iMargin, 0);
}

void BionicWidget::updatePadding()
{
    // Space above and below the text lets the first and the last line reach the reading band.
    // Its size depends on the height of those lines (a heading is taller than a paragraph line).
    QTextFrame *pRoot = document()->rootFrame();
    QTextFrameFormat format = pRoot->frameFormat();
    int iTop = 0;
    int iBottom = 0;

    if (!document()->isEmpty())
    {
        int iCenter = bandRect().center().y();

        iTop = qMax(0, iCenter - lineHalfHeight(document()->begin(), false));
        iBottom = qMax(0, viewport()->height() - iCenter - lineHalfHeight(document()->lastBlock(), true));
    }

    if (format.topMargin() != iTop || format.bottomMargin() != iBottom)
    {
        format.setTopMargin(iTop);
        format.setBottomMargin(iBottom);
        pRoot->setFrameFormat(format);
    }
}

int BionicWidget::lineHalfHeight(const QTextBlock &block, bool bLast) const
{
    // Asking for the block's rectangle makes the document lay it out, so its lines exist.
    document()->documentLayout()->blockBoundingRect(block);

    QTextLayout *pLayout = block.layout();

    if (pLayout == nullptr || pLayout->lineCount() == 0)
    {
        return fontMetrics().lineSpacing() / 2;
    }

    return static_cast<int>(pLayout->lineAt(bLast ? pLayout->lineCount() - 1 : 0).height() / 2);
}
