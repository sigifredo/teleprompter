

// Own
#include <VersionWidget.hpp>
#include <CreditsDialog.hpp>
#include <Version.hpp>

// Qt
#include <QBoxLayout>
#include <QPushButton>

VersionWidget::VersionWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QPushButton *pVersionButton = new QPushButton("Versión v" PROJECT_VERSION, this);

    connect(pVersionButton, SIGNAL(clicked()), this, SLOT(showCredits()));

    pLayout->addWidget(pVersionButton);
}

void VersionWidget::showCredits()
{
    CreditsDialog().exec();
}
