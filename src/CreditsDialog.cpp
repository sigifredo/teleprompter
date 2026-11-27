

// own
#include <CreditsDialog.hpp>
#include <Version.hpp>

// qt
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>

CreditsDialog::CreditsDialog(QWidget *parent)
    : QDialog(parent)
{
    auto *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    auto *appInfoLabel = new QLabel(tr("<h2>%1</h2>Project version: %2<br>Qt version: %3").arg(PROJECT_NAME, PROJECT_VERSION, qVersion()), this);

    auto *tabs = new QTabWidget(this);

    auto *aboutTab = new QWidget(tabs);
    auto *aboutLayout = new QBoxLayout(QBoxLayout::LeftToRight, aboutTab);
    auto *descriptionLabel = new QLabel(tr("%1 is an application designed to enhance reading\nefficiency and manage time effectively. Utilizing the concept of\nbionic reading, it aids users in improving their reading speed and\ncomprehension by guiding the eyes through text with strategically\nhighlighted focal points. Additionally, the app includes time\ncontrol via a chronometer, allowing users to check the time while\nreading. This software is distributed under the terms of the GNU\nGeneral Public License.").arg(PROJECT_NAME), aboutTab);

    aboutLayout->addWidget(descriptionLabel);

    auto *authorTab = new QWidget(tabs);
    auto *authorLayout = new QBoxLayout(QBoxLayout::TopToBottom, authorTab);
    auto *authorLabel = new QLabel(tr("<strong>Sigifredo Escobar Gómez</strong><br><em>Developer</em><br><a href=\"https://sigifredo.dev\">sigifredo.dev</a>"), authorTab);

    authorLayout->addWidget(authorLabel);

    auto *licenceTab = new QWidget(tabs);
    auto *licenceLayout = new QBoxLayout(QBoxLayout::TopToBottom, licenceTab);
    auto *licenceTextEdit = new QTextEdit(licenceTab);

    licenceTextEdit->setReadOnly(true);
    licenceTextEdit->setHtml(tr("<strong>%1:</strong> A bionic reading and time control application.<br>Copyright (C) 2024 Sigifredo Escobar Gómez<br><br>This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.<br><br>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.<br><br>You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.<br><br><strong>Atkinson Hyperlegible Next</strong> (reading font): Copyright 2020-2024 The Atkinson Hyperlegible Next Project Authors, licensed under the SIL Open Font License 1.1 (<a href=\"https://openfontlicense.org\">openfontlicense.org</a>).").arg(PROJECT_NAME));
    licenceLayout->addWidget(licenceTextEdit);

    tabs->addTab(aboutTab, tr("About"));
    tabs->addTab(authorTab, tr("Author"));
    tabs->addTab(licenceTab, tr("Licence"));

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);

    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(appInfoLabel);
    layout->addWidget(tabs);
    layout->addWidget(buttons);

    setWindowTitle(tr("About %1").arg(PROJECT_NAME));
}
