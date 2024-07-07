

// Own
#include <TextReaderWidget.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>
#include <FontBarWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

TextReaderWidget::TextReaderWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QWidget *pFileWidget = new QWidget(this);
    {
        QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pFileWidget);

        _pFileNameLabel = new QLabel(this);
        QPushButton *pOpenFileButton = new QPushButton("&Open file...", pFileWidget);

        connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

        pLayout->addWidget(_pFileNameLabel);
        pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pLayout->addWidget(pOpenFileButton);
    }

    _pBionicWidget = new BionicWidget(this);
    ChronometerWidget *pChronometerWidget = new ChronometerWidget(this);
    FontBarWidget *pFontBarWidget = new FontBarWidget(this);

    _pBionicWidget->setBionicText("Autor:\nSigifredo Escobar Gómez <sigifredo89@gmail.com>\nhttps://s9art.com/links/");
    _pBionicWidget->setFontSize(pFontBarWidget->fontSize());

    connect(pFontBarWidget, SIGNAL(changeFontSize(int)), _pBionicWidget, SLOT(setFontSize(int)));

    pLayout->addWidget(pFileWidget);
    pLayout->addWidget(_pBionicWidget);
    pLayout->addWidget(pChronometerWidget);
    pLayout->addWidget(pFontBarWidget);
}

void TextReaderWidget::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Text file (*.txt)"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();

        if (files.size() > 0)
        {
            openFile(files.first().trimmed());
        }
    }
}

void TextReaderWidget::fontSizeChanged(const int &iSize)
{
    _pFontSizeLabel->setText(QString::number(iSize));
}

void TextReaderWidget::openFile(const QString &sPath)
{
    if (!sPath.isEmpty())
    {
        QFile file(sPath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            _pFileNameLabel->setText(QFileInfo(file).fileName());
            _pBionicWidget->setBionicText(in.readAll().trimmed());

            file.close();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not open the file: " + file.errorString());
        }
    }
}
