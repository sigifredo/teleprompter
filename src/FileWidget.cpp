

// own
#include <FileWidget.hpp>

// qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QFontMetrics>
#include <QLabel>
#include <QPushButton>
#include <QStyle>

namespace
{
    // Ancho máximo de la etiqueta con el nombre del archivo.
    constexpr int MaxNameWidth = 110;
} // namespace

FileWidget::FileWidget(const QString &buttonText, const QString &filter, QWidget *parent)
    : QWidget(parent), filter_(filter)
{
    auto *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    layout->setContentsMargins(0, 0, 0, 0);

    fileNameLabel_ = new QLabel(tr("No file"), this);
    fileNameLabel_->setMaximumWidth(MaxNameWidth);

    auto *openFileButton = new QPushButton(buttonText, this);

    connect(openFileButton, &QPushButton::clicked, this, &FileWidget::chooseFile);

    layout->addWidget(fileNameLabel_);
    layout->addWidget(openFileButton);
}

void FileWidget::setFileName(const QString &fileName)
{
    // Con el nombre puesto, la hoja de estilos deja de pintar la etiqueta como texto de relleno.
    fileNameLabel_->setProperty("role", "file");
    fileNameLabel_->setText(fileNameLabel_->fontMetrics().elidedText(fileName, Qt::ElideMiddle, MaxNameWidth));
    fileNameLabel_->setToolTip(fileName);
    style()->unpolish(fileNameLabel_);
    style()->polish(fileNameLabel_);
}

void FileWidget::chooseFile()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(filter_);

    if (dialog.exec() != QDialog::Accepted)
        return;

    const QStringList files = dialog.selectedFiles();

    if (!files.isEmpty())
        emit fileSelected(files.first().trimmed());
}
