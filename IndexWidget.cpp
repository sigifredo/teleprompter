#include "IndexWidget.hpp"

// Qt
#include <QBoxLayout>
#include <QPushButton>

IndexWidget::IndexWidget(QWidget *parent)
    : QWidget(parent)
{
    QBoxLayout* pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QPushButton* pOpenButton = new QPushButton("&Abrir archivo...", this);

    pLayout->addWidget(pOpenButton);
}
