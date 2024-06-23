#include "IndexWidget.hpp"

// Qt
#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

IndexWidget::IndexWidget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* pLayout = new QGridLayout(this);
    QPushButton* pOpenButton = new QPushButton("&Abrir archivo...", this);

    connect(pOpenButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 1);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 0);
    pLayout->addWidget(pOpenButton, 1, 1);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 2);
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 1);
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
