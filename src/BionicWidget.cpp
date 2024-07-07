

// Own
#include <BionicWidget.hpp>

BionicWidget::BionicWidget(QWidget *pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);
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

void BionicWidget::setFontSize(const int &iFontSize)
{
    QFont defaultFont = font();
    defaultFont.setPointSize(iFontSize);

    setFont(defaultFont);
}
