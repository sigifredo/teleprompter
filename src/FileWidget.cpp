

// Own
#include <FileWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QFontMetrics>
#include <QPushButton>
#include <QStyle>

FileWidget::FileWidget(const QString &sButtonText, const QString &sFilter, QWidget *pParent)
    : QWidget(pParent), _sFilter(sFilter)
{
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    pLayout->setContentsMargins(0, 0, 0, 0);

    _pFileNameLabel = new QLabel("No file", this);
    _pFileNameLabel->setMaximumWidth(110);
    QPushButton *pOpenFileButton = new QPushButton(sButtonText, this);

    connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

    pLayout->addWidget(_pFileNameLabel);
    pLayout->addWidget(pOpenFileButton);
}

void FileWidget::setFileName(const QString &sFileName)
{
    _pFileNameLabel->setObjectName("fileName");
    _pFileNameLabel->setText(_pFileNameLabel->fontMetrics().elidedText(sFileName, Qt::ElideMiddle, 110));
    _pFileNameLabel->setToolTip(sFileName);
    style()->unpolish(_pFileNameLabel);
    style()->polish(_pFileNameLabel);
}

void FileWidget::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(_sFilter);

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();

        if (files.size() > 0)
        {
            emit fileSelected(files.first().trimmed());
        }
    }
}