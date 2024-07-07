

// Own
#include <TextReaderWidget.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
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
    _pBionicWidget->setBionicText("Autor:\nSigifredo Escobar Gómez <sigifredo89@gmail.com>\nhttps://s9art.com/links/");

    ChronometerWidget *pChronometerWidget = new ChronometerWidget(this);

    QWidget *pFontBarWidget = new QWidget(this);
    {
        QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pFontBarWidget);

        QPushButton *pPlusButton = new QPushButton("&+", pFontBarWidget);
        _pFontSizeLabel = new QLabel("0", pFontBarWidget);
        QPushButton *pMinusButton = new QPushButton("&-", pFontBarWidget);

        _pFontSizeLabel->setAlignment(Qt::AlignCenter);
        _pFontSizeLabel->setStyleSheet("font: 18pt;");
        _pFontSizeLabel->setText(QString::number(_pBionicWidget->font().pointSize()));

        connect(pPlusButton, SIGNAL(clicked()), _pBionicWidget, SLOT(incrementFontSize()));
        connect(pMinusButton, SIGNAL(clicked()), _pBionicWidget, SLOT(decrementFontSize()));

        pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pLayout->addWidget(pPlusButton);
        pLayout->addWidget(_pFontSizeLabel);
        pLayout->addWidget(pMinusButton);
        pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    }

    connect(_pBionicWidget, SIGNAL(fontSizeChanged(int)), this, SLOT(fontSizeChanged(int)));

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
