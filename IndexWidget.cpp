#include "IndexWidget.hpp"

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QPushButton>

IndexWidget::IndexWidget(QWidget *parent)
    : QWidget(parent)
{
    QBoxLayout* pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QPushButton* pOpenButton = new QPushButton("&Abrir archivo...", this);

    connect(pOpenButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

    pLayout->addWidget(pOpenButton);
}

void IndexWidget::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Archivo de texto (*.txt)"));

    if (dialog.exec() == QDialog::Accepted)
    {
    }

    /*
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    */
}
