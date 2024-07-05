// Own
#include "BionicWidget.hpp"

// Qt
#include <QDebug>

BionicWidget::BionicWidget(QWidget* pParent)
    : QTextEdit(pParent)
{
    setReadOnly(true);
}

void BionicWidget::decrementFontSize()
{
}

void BionicWidget::incrementFontSize()
{
    // Obtener el cursor de texto y seleccionar todo el texto
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);

    // Obtener el formato del texto
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();

    // Aumentar el tamaño de la fuente
    font.setPointSize(font.pointSize() + 2);
    font.setBold(false);
    format.setFont(font);

    // Aplicar el nuevo formato al texto seleccionado
    cursor.setCharFormat(format);

    // setBionicText(_sRawText);
}

void BionicWidget::setBionicText(const QString& sText)
{
    _sRawText = sText;

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
