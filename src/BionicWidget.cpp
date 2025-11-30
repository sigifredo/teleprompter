

// Own
#include <BionicWidget.hpp>

// Qt
#include <QTextBlock>

BionicWidget::BionicWidget(QWidget *pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);
}

void BionicWidget::setBionicText(const QString &sText)
{
    clear();
    setMarkdown(sText);

    QTextDocument *doc = document();
    QTextCursor cursor(doc);

    cursor.beginEditBlock();

    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next())
    {
        const QString text = block.text();
        const int blockPos = block.position(); // posición absoluta del bloque en el documento

        int i = 0;
        const int n = text.length();

        while (i < n)
        {
            while (i < n && text[i].isSpace())
                ++i;

            const int wordStart = i;

            while (i < n && !text[i].isSpace())
                ++i;

            const int wordEnd = i;

            if (wordEnd <= wordStart)
                continue;

            const int wordLen = wordEnd - wordStart;
            const int boldLen = wordLen / 2;

            if (boldLen <= 0)
                continue;

            QTextCharFormat fmt;
            fmt.setFontWeight(QFont::Bold);

            // Seleccionar y aplicar formato a la primera mitad de la palabra
            cursor.setPosition(blockPos + wordStart);
            cursor.setPosition(blockPos + wordStart + boldLen, QTextCursor::KeepAnchor);
            cursor.mergeCharFormat(fmt);
        }
    }

    cursor.endEditBlock();
}

void BionicWidget::setFontSize(const int &iFontSize)
{
    QFont defaultFont = font();
    defaultFont.setPointSize(iFontSize);

    setFont(defaultFont);
}
