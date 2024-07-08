

// Own
#include <MainWindow.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>
#include <FileWidget.hpp>
#include <FontBarWidget.hpp>
#include <VersionWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QFileInfo>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent)
{
    QWidget *pCentralWidget = new QWidget(this);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, pCentralWidget);

    _pFileWidget = new FileWidget(pCentralWidget);
    _pBionicWidget = new BionicWidget(pCentralWidget);
    ChronometerWidget *pChronometerWidget = new ChronometerWidget(pCentralWidget);
    FontBarWidget *pFontBarWidget = new FontBarWidget(pCentralWidget);
    VersionWidget *pVersionWidget = new VersionWidget(pCentralWidget);

    pLayout->setSpacing(0);
    _pBionicWidget->setBionicText("Hello world!");
    _pBionicWidget->setFontSize(pFontBarWidget->fontSize());

    connect(_pFileWidget, SIGNAL(fileSelected(QString)), this, SLOT(openFile(QString)));
    connect(pFontBarWidget, SIGNAL(changeFontSize(int)), _pBionicWidget, SLOT(setFontSize(int)));

    pLayout->addWidget(_pFileWidget);
    pLayout->addWidget(_pBionicWidget);
    pLayout->addWidget(pChronometerWidget);
    pLayout->addWidget(pFontBarWidget);
    pLayout->addWidget(pVersionWidget);

    setCentralWidget(pCentralWidget);
    setWindowFlag(Qt::WindowStaysOnTopHint);

#if defined(Q_OS_WIN)
    setWindowIcon(QIcon(":/app-icons/teleprompter.ico"));
#elif defined(Q_OS_MACOS)
    setWindowIcon(QIcon(":/app-icons/teleprompter.icns"));
#else
    setWindowIcon(QIcon(":/app-icons/teleprompter.png"));
#endif
}

void MainWindow::fontSizeChanged(const int &iSize)
{
    _pFontSizeLabel->setText(QString::number(iSize));
}

void MainWindow::openFile(const QString &sFilePath)
{
    if (!sFilePath.isEmpty())
    {
        QFile file(sFilePath);

        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);

            _pFileWidget->setFileName(QFileInfo(file).fileName());
            _pBionicWidget->setBionicText(in.readAll().trimmed());

            file.close();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not open the file: " + file.errorString());
        }
    }
}
