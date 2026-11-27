

// Own
#include <MainWindow.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>
#include <FileWidget.hpp>
#include <FontBarWidget.hpp>
#include <PresentationWindow.hpp>
#include <SlideDeck.hpp>
#include <SlideStripWidget.hpp>
#include <VersionWidget.hpp>

// Qt
#include <QApplication>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QScreen>
#include <QFileInfo>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent), _pPresentation(nullptr)
{
    QWidget *pCentralWidget = new QWidget(this);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, pCentralWidget);

    _pFileWidget = new FileWidget("Open &script...", "Script (*.md *.txt)", pCentralWidget);
    _pSlidesFileWidget = new FileWidget("Open s&lides...", "Slides (*.pdf)", pCentralWidget);
    _pDeck = new SlideDeck(this);
    _pSlideStrip = new SlideStripWidget(pCentralWidget);
    _pSlideStrip->hide();

    QWidget *pSlideBar = new QWidget(pCentralWidget);
    {
        QBoxLayout *pBarLayout = new QBoxLayout(QBoxLayout::LeftToRight, pSlideBar);

        _pSlideCounterLabel = new QLabel("No slides", pSlideBar);
        _pPresentButton = new QPushButton("&Present (F5)", pSlideBar);
        _pPresentButton->setEnabled(false);

        pBarLayout->addWidget(_pSlideCounterLabel);
        pBarLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        pBarLayout->addWidget(_pPresentButton);
    }
    _pBionicWidget = new BionicWidget(pCentralWidget);
    ChronometerWidget *pChronometerWidget = new ChronometerWidget(pCentralWidget);
    FontBarWidget *pFontBarWidget = new FontBarWidget(pCentralWidget);
    VersionWidget *pVersionWidget = new VersionWidget(pCentralWidget);

    pLayout->setSpacing(0);
    _pBionicWidget->setBionicText("Hello world!");
    _pBionicWidget->setFontSize(pFontBarWidget->fontSize());

    connect(_pFileWidget, SIGNAL(fileSelected(QString)), this, SLOT(openFile(QString)));
    connect(_pSlidesFileWidget, SIGNAL(fileSelected(QString)), this, SLOT(openSlides(QString)));
    connect(_pDeck, SIGNAL(currentChanged(int)), this, SLOT(updateSlideCounter()));
    connect(_pPresentButton, SIGNAL(clicked()), this, SLOT(startPresentation()));
    connect(pFontBarWidget, SIGNAL(changeFontSize(int)), _pBionicWidget, SLOT(setFontSize(int)));

    pLayout->addWidget(_pFileWidget);
    pLayout->addWidget(_pSlidesFileWidget);
    pLayout->addWidget(_pBionicWidget, 2);
    pLayout->addWidget(_pSlideStrip);
    pLayout->addWidget(pSlideBar);
    pLayout->addWidget(pChronometerWidget);
    pLayout->addWidget(pFontBarWidget);
    pLayout->addWidget(pVersionWidget);

    setCentralWidget(pCentralWidget);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    qApp->installEventFilter(this);

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
            QString sText = in.readAll().trimmed();

            if (QFileInfo(file).suffix().compare("md", Qt::CaseInsensitive) == 0)
            {
                _pBionicWidget->setBionicMarkdown(sText);
            }
            else
            {
                _pBionicWidget->setBionicText(sText);
            }

            file.close();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not open the file: " + file.errorString());
        }
    }
}

void MainWindow::openSlides(const QString &sFilePath)
{
    if (sFilePath.isEmpty())
    {
        return;
    }

    if (_pDeck->load(sFilePath))
    {
        _pSlidesFileWidget->setFileName(QFileInfo(sFilePath).fileName());
        _pSlideStrip->setDeck(_pDeck);
        _pSlideStrip->show();
        _pPresentButton->setEnabled(true);
        updateSlideCounter();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not open the slides: " + sFilePath);
    }
}

void MainWindow::startPresentation()
{
    if (_pPresentation != nullptr || _pDeck->count() == 0)
    {
        return;
    }

    // On a single screen the admin window must not stay above the presentation.
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    show();

    _pPresentation = new PresentationWindow(_pDeck);
    _pPresentation->setAttribute(Qt::WA_DeleteOnClose);
    connect(_pPresentation, SIGNAL(closed()), this, SLOT(stopPresentation()));

    QScreen *pTarget = nullptr;

    for (QScreen *pScreen : QGuiApplication::screens())
    {
        if (pScreen != screen())
        {
            pTarget = pScreen;
            break;
        }
    }

    if (pTarget != nullptr)
    {
        _pPresentation->move(pTarget->geometry().topLeft());
        _pPresentation->showFullScreen();
    }
    else
    {
        _pPresentation->showMaximized();
    }

    activateWindow();
    raise();
}

void MainWindow::stopPresentation()
{
    if (_pPresentation == nullptr)
    {
        return;
    }

    PresentationWindow *pPresentation = _pPresentation;

    _pPresentation = nullptr;
    pPresentation->disconnect(this);
    pPresentation->close();

    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    show();
    activateWindow();
}

void MainWindow::updateSlideCounter()
{
    _pSlideCounterLabel->setText(QString("Slide %1 / %2").arg(_pDeck->currentIndex() + 1).arg(_pDeck->count()));
}

bool MainWindow::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress && _pDeck->count() > 0 && QApplication::activeModalWidget() == nullptr)
    {
        QKeyEvent *pKeyEvent = static_cast<QKeyEvent *>(pEvent);

        // Modifiers are left alone so the Alt+<letter> mnemonics keep working.
        if (pKeyEvent->modifiers() == Qt::NoModifier || pKeyEvent->modifiers() == Qt::KeypadModifier)
        {
            switch (pKeyEvent->key())
            {
            case Qt::Key_Left:
            case Qt::Key_H:
                _pDeck->previous();
                return true;
            case Qt::Key_Right:
            case Qt::Key_L:
                _pDeck->next();
                return true;
            case Qt::Key_Up:
            case Qt::Key_K:
                _pBionicWidget->scrollStep(-1);
                return true;
            case Qt::Key_Down:
            case Qt::Key_J:
                _pBionicWidget->scrollStep(1);
                return true;
            case Qt::Key_F5:
                startPresentation();
                return true;
            case Qt::Key_Escape:
            case Qt::Key_Q:
                if (_pPresentation != nullptr)
                {
                    stopPresentation();
                    return true;
                }
                break;
            default:
                break;
            }
        }
    }

    return QMainWindow::eventFilter(pObject, pEvent);
}
