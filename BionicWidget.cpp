#include "BionicWidget.hpp"


BionicWidget::BionicWidget(QWidget* pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);
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
