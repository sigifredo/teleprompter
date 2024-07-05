

// Own
#include <BionicWidget.hpp>

BionicWidget::BionicWidget(QWidget *pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);

    QFont defaultFont = font();
    defaultFont.setPointSize(DEFAULT_FONT_SIZE);

    setFont(defaultFont);
}

void BionicWidget::decrementFontSize()
{
    QFont defaultFont = font();
    int fontSize = defaultFont.pointSize() - 2;

    defaultFont.setPointSize(fontSize);

    setFont(defaultFont);
    emit fontSizeChanged(fontSize);
}

void BionicWidget::incrementFontSize()
{
    QFont defaultFont = font();
    int fontSize = defaultFont.pointSize() + 2;

    defaultFont.setPointSize(fontSize);

    setFont(defaultFont);
    emit fontSizeChanged(fontSize);
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
            QStringList words = paragraph.split(" ");

            for (const QString &sWord : words)
            {
                QString sFixedWord = sWord.trimmed();

                if (!sFixedWord.isEmpty())
                {
                    int iLen = sFixedWord.length();
                    int iBoldPart = iLen / 2;

                    sResult.append("<strong>" + sFixedWord.left(iBoldPart) + "</strong>" + sFixedWord.mid(iBoldPart) + " ");
                }
            }

            sResult.append("<br><br>");
        }
    }

    setHtml(sResult.trimmed());
}
