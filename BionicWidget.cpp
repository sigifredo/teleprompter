// Own
#include "BionicWidget.hpp"


BionicWidget::BionicWidget(QWidget* pParent)
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

void BionicWidget::setBionicText(const QString& sText)
{
    QString sResult;
    QStringList words = sText.split(" ");

    for (const QString &sWord : words)
    {
        int iLen = sWord.length();
        int iBoldPart = iLen / 2;

        sResult.append("<strong>" + sWord.left(iBoldPart) + "</strong>" + sWord.mid(iBoldPart) + " ");
    }

    sResult.replace("\r\n", "<br>");
    sResult.replace("\n", "<br>");

    setHtml(sResult.trimmed());
}
