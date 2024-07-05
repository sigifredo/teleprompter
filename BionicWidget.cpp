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

    for (const QString &sWord : words) {
        int iLen = sWord.length();
        int iBoldPart = iLen / 2;

        sResult.append("<b>" + sWord.left(iBoldPart) + "</b>" + sWord.mid(iBoldPart) + " ");
    }

    setHtml(sResult.trimmed());
}
