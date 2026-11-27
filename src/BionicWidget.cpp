

// Own
#include <BionicWidget.hpp>

// Qt
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
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
}

void BionicWidget::scrollStep(int iDirection)
{
    QScrollBar *pBar = verticalScrollBar();

    pBar->setValue(pBar->value() + iDirection * 3 * fontMetrics().lineSpacing());
}

void BionicWidget::setFontSize(const int &iFontSize)
{
    QFont defaultFont = font();
    defaultFont.setPointSize(iFontSize);

    setFont(defaultFont);
    updateColumn();
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
    int iHeight = static_cast<int>(fontMetrics().lineSpacing() * 1.5);
    int iTop = viewport()->height() * 3 / 10;

    color.setAlpha(34);
    painter.fillRect(0, iTop, viewport()->width(), iHeight, color);
    color.setAlpha(200);
    painter.fillRect(0, iTop, 3, iHeight, color);
}

void BionicWidget::resizeEvent(QResizeEvent *pEvent)
{
    QTextEdit::resizeEvent(pEvent);
    updateColumn();
}

void BionicWidget::updateColumn()
{
    // Keep lines to a comfortable reading width and center them.
    int iColumn = fontMetrics().averageCharWidth() * COLUMN_CHARS;
    int iAvailable = width() - 2 * frameWidth() - (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0);
    int iMargin = qMax(16, (iAvailable - iColumn) / 2);

    setViewportMargins(iMargin, 0, iMargin, 0);
}
