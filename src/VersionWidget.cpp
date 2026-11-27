

// own
#include <VersionWidget.hpp>
#include <CreditsDialog.hpp>
#include <Version.hpp>

// qt
#include <QBoxLayout>
#include <QPushButton>

VersionWidget::VersionWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    auto *versionButton = new QPushButton("v" PROJECT_VERSION, this);

    layout->setContentsMargins(0, 0, 0, 0);
    versionButton->setProperty("variant", "ghost");
    versionButton->setToolTip(tr("About"));

    connect(versionButton, &QPushButton::clicked, this, &VersionWidget::showCredits);

    layout->addWidget(versionButton);
}

void VersionWidget::showCredits()
{
    CreditsDialog().exec();
}
