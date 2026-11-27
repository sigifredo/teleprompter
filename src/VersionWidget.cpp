

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
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    QPushButton *pVersionButton = new QPushButton("v" PROJECT_VERSION, this);

    pLayout->setContentsMargins(0, 0, 0, 0);
    pVersionButton->setObjectName("flat");
    pVersionButton->setToolTip("About");

    connect(pVersionButton, SIGNAL(clicked()), this, SLOT(showCredits()));

    pLayout->addWidget(pVersionButton);
}

void VersionWidget::showCredits()
{
    CreditsDialog().exec();
}
