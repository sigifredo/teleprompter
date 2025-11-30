

// Own
#include <FileWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>

FileWidget::FileWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    _pFileNameLabel = new QLabel(this);
    QPushButton *pOpenFileButton = new QPushButton("&Open file...", this);

    connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

    pLayout->addWidget(_pFileNameLabel);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pLayout->addWidget(pOpenFileButton);
}

void FileWidget::setFileName(const QString &sFileName)
{
    _pFileNameLabel->setText(sFileName);
}

void FileWidget::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Markdown (*.md)"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();

        if (files.size() > 0)
        {
            emit fileSelected(files.first().trimmed());
        }
    }
}