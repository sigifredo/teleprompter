#include "TextReaderWidget.hpp"
#include "BionicWidget.hpp"

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>

TextReaderWidget::TextReaderWidget(QWidget *parent)
    : QWidget(parent)
{
    QBoxLayout* pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QWidget* pFileWidget = new QWidget(this);
    {
        QLayout* pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pFileWidget);

        QPushButton* pOpenFileButton = new QPushButton("&Abrir archivo...", pFileWidget);

        connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

        pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pLayout->addWidget(pOpenFileButton);
    }

    _pBionicWidget = new BionicWidget(this);

    QWidget* pToolBarWidget = new QWidget(this);
    {
        QLayout* pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pToolBarWidget);

        QPushButton* pPlusButton = new QPushButton("+", pToolBarWidget);
        QPushButton* pMinusButton = new QPushButton("-", pToolBarWidget);

        connect(pPlusButton, SIGNAL(clicked()), _pBionicWidget, SLOT(incrementFontSize()));
        connect(pMinusButton, SIGNAL(clicked()), _pBionicWidget, SLOT(decrementFontSize()));

        pLayout->addWidget(pPlusButton);
        pLayout->addWidget(pMinusButton);
    }

    pLayout->addWidget(pFileWidget);
    pLayout->addWidget(_pBionicWidget);
    pLayout->addWidget(pToolBarWidget);
}

void TextReaderWidget::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Archivo de texto (*.txt)"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();

        if (files.size() > 0)
        {
            openFile(files.first().trimmed());
        }
    }
}

void TextReaderWidget::openFile(const QString& sPath)
{
    if (!sPath.isEmpty())
    {
        QFile file(sPath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            _pBionicWidget->setBionicText(in.readAll().trimmed());

            file.close();
        }
        else
        {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo: " + file.errorString());
        }
    }
}
