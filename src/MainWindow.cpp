

// Own
#include <MainWindow.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>
#include <FontBarWidget.hpp>
#include <VersionWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    QWidget *pCentralWidget = new QWidget(this);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, pCentralWidget);

    QWidget *pFileWidget = new QWidget(pCentralWidget);
    {
        QLayout *pLayout = new QBoxLayout(QBoxLayout::LeftToRight, pFileWidget);

        _pFileNameLabel = new QLabel(pFileWidget);
        QPushButton *pOpenFileButton = new QPushButton("&Open file...", pFileWidget);

        connect(pOpenFileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));

        pLayout->addWidget(_pFileNameLabel);
        pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pLayout->addWidget(pOpenFileButton);
    }

    _pBionicWidget = new BionicWidget(pCentralWidget);
    ChronometerWidget *pChronometerWidget = new ChronometerWidget(pCentralWidget);
    FontBarWidget *pFontBarWidget = new FontBarWidget(pCentralWidget);
    VersionWidget *pVersionWidget = new VersionWidget(pCentralWidget);

    _pBionicWidget->setBionicText("Hello world!");
    _pBionicWidget->setFontSize(pFontBarWidget->fontSize());

    connect(pFontBarWidget, SIGNAL(changeFontSize(int)), _pBionicWidget, SLOT(setFontSize(int)));

    pLayout->addWidget(pFileWidget);
    pLayout->addWidget(_pBionicWidget);
    pLayout->addWidget(pChronometerWidget);
    pLayout->addWidget(pFontBarWidget);
    pLayout->addWidget(pVersionWidget);

    setCentralWidget(pCentralWidget);
}

void MainWindow::chooseFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Text file (*.txt)"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();

        if (files.size() > 0)
        {
            openFile(files.first().trimmed());
        }
    }
}

void MainWindow::fontSizeChanged(const int &iSize)
{
    _pFontSizeLabel->setText(QString::number(iSize));
}

void MainWindow::openFile(const QString &sPath)
{
    if (!sPath.isEmpty())
    {
        QFile file(sPath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            _pFileNameLabel->setText(QFileInfo(file).fileName());
            _pBionicWidget->setBionicText(in.readAll().trimmed());

            file.close();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not open the file: " + file.errorString());
        }
    }
}
