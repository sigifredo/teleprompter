

// Own
#include <MainWindow.hpp>
#include <BionicWidget.hpp>
#include <ChronometerWidget.hpp>
#include <FileWidget.hpp>
#include <FontBarWidget.hpp>
#include <PresentationWindow.hpp>
#include <SettingsDialog.hpp>
#include <SlideDeck.hpp>
#include <SlidePreviewWidget.hpp>
#include <SlideStripWidget.hpp>
#include <Theme.hpp>
#include <VersionWidget.hpp>

// Qt
#include <QApplication>
#include <QBoxLayout>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QScreen>
#include <QSettings>
#include <QStatusBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent), _pPresentation(nullptr), _bAlwaysOnTop(true), _bStartTimerOnPresent(false), _bVimKeys(false)
{
    QWidget *pCentralWidget = new QWidget(this);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, pCentralWidget);

    _pDeck = new SlideDeck(this);
    _pFileWidget = new FileWidget("Open &script...", "Script (*.md *.txt)", pCentralWidget);
    _pSlidesFileWidget = new FileWidget("Open s&lides...", "Slides (*.pdf)", pCentralWidget);
    _pChronometer = new ChronometerWidget(pCentralWidget);
    _pFontBar = new FontBarWidget(pCentralWidget);
    _pPresentButton = new QPushButton("Present (F5)", pCentralWidget);
    _pBionicWidget = new BionicWidget(pCentralWidget);
    _pSlideStrip = new SlideStripWidget(pCentralWidget);

    // Single control bar: files, timer, font size and presentation.
    QWidget *pBar = new QWidget(pCentralWidget);
    {
        QBoxLayout *pBarLayout = new QBoxLayout(QBoxLayout::LeftToRight, pBar);

        pBar->setObjectName("topbar");
        pBarLayout->setContentsMargins(0, 0, 0, 10);
        pBarLayout->setSpacing(16);
        pBarLayout->addWidget(_pFileWidget);
        pBarLayout->addWidget(_pSlidesFileWidget);
        pBarLayout->addStretch(1);
        pBarLayout->addWidget(_pChronometer);
        pBarLayout->addWidget(_pFontBar);
        pBarLayout->addWidget(_pPresentButton);
    }

    // Script on the left, current and next slide on the right (presenter view).
    QWidget *pBody = new QWidget(pCentralWidget);
    {
        QBoxLayout *pBodyLayout = new QBoxLayout(QBoxLayout::LeftToRight, pBody);
        QBoxLayout *pPreviewLayout = new QBoxLayout(QBoxLayout::TopToBottom);

        pBodyLayout->setContentsMargins(0, 0, 0, 0);
        pBodyLayout->setSpacing(12);
        pPreviewLayout->setSpacing(8);
        pPreviewLayout->addWidget(new SlidePreviewWidget(_pDeck, 0, "Current", pBody), 3);
        pPreviewLayout->addWidget(new SlidePreviewWidget(_pDeck, 1, "Next", pBody), 2);

        pBodyLayout->addWidget(_pBionicWidget, 3);
        pBodyLayout->addLayout(pPreviewLayout, 2);
    }

    _pPresentButton->setObjectName("primary");
    _pPresentButton->setEnabled(false);
    _pPresentButton->setToolTip("Start the presentation (F5)");
    _pBionicWidget->setPlaceholderText("Open a script (.md / .txt)\nand slides (.pdf)\n\nPress ? for shortcuts");

    pLayout->setSpacing(10);
    pLayout->addWidget(pBar);
    pLayout->addWidget(pBody, 1);
    pLayout->addWidget(_pSlideStrip);
    setCentralWidget(pCentralWidget);

    // Status bar: slide counter and view options.
    _pSlideCounterLabel = new QLabel("No slides", this);
    QPushButton *pSettingsButton = new QPushButton("Settings", this);
    QPushButton *pHelpButton = new QPushButton("?", this);

    pSettingsButton->setObjectName("flat");
    pSettingsButton->setToolTip("Settings (Ctrl+,)");
    pHelpButton->setObjectName("flat");
    pHelpButton->setToolTip("Keyboard shortcuts (?)");

    statusBar()->setSizeGripEnabled(false);
    statusBar()->addWidget(_pSlideCounterLabel);
    statusBar()->addPermanentWidget(pHelpButton);
    statusBar()->addPermanentWidget(pSettingsButton);
    statusBar()->addPermanentWidget(new VersionWidget(this));

    // Buttons take focus with Tab only, so a mouse click never moves the keyboard focus away from the script.
    for (QPushButton *pButton : findChildren<QPushButton *>())
    {
        pButton->setFocusPolicy(Qt::TabFocus);
    }

    connect(_pFileWidget, SIGNAL(fileSelected(QString)), this, SLOT(openFile(QString)));
    connect(_pSlidesFileWidget, SIGNAL(fileSelected(QString)), this, SLOT(openSlides(QString)));
    connect(_pFontBar, SIGNAL(changeFontSize(int)), _pBionicWidget, SLOT(setFontSize(int)));
    connect(_pDeck, SIGNAL(currentChanged(int)), this, SLOT(updateSlideCounter()));
    connect(_pPresentButton, SIGNAL(clicked()), this, SLOT(startPresentation()));
    connect(pSettingsButton, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(pHelpButton, SIGNAL(clicked()), this, SLOT(showHelp()));

#if defined(Q_OS_WIN)
    setWindowIcon(QIcon(":/app-icons/teleprompter.ico"));
#elif defined(Q_OS_MACOS)
    setWindowIcon(QIcon(":/app-icons/teleprompter.icns"));
#else
    setWindowIcon(QIcon(":/app-icons/teleprompter.png"));
#endif

    setAcceptDrops(true);
    qApp->installEventFilter(this);

    _pBionicWidget->setFontSize(_pFontBar->fontSize());
    loadSettings();

    // Keys go to the script from the start; buttons are reached with Tab.
    _pBionicWidget->setFocus();
}

bool MainWindow::hasSavedGeometry() const
{
    return QSettings().contains("window/geometry");
}

void MainWindow::loadSettings()
{
    QSettings settings;

    restoreGeometry(settings.value("window/geometry").toByteArray());
    _pFontBar->setFontSize(settings.value("ui/fontSize", _pFontBar->fontSize()).toInt());

    setAlwaysOnTop(settings.value("ui/alwaysOnTop", true).toBool());
    _bStartTimerOnPresent = settings.value("ui/timerOnPresent", false).toBool();
    _bVimKeys = settings.value("ui/vimKeys", false).toBool();
    _pBionicWidget->setBionicEnabled(settings.value("ui/bionic", true).toBool());
    _pBionicWidget->setReadingBandVisible(settings.value("ui/bandVisible", false).toBool());
    _pBionicWidget->setReadingBandPosition(settings.value("ui/bandPercent", _pBionicWidget->readingBandPosition()).toInt());
    _pBionicWidget->setColumnChars(settings.value("ui/columnChars", _pBionicWidget->columnChars()).toInt());

    QString sScript = settings.value("files/script").toString();
    QString sSlides = settings.value("files/slides").toString();

    if (!sScript.isEmpty() && QFileInfo::exists(sScript))
    {
        openFile(sScript);
    }

    if (!sSlides.isEmpty() && QFileInfo::exists(sSlides))
    {
        openSlides(sSlides);
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.setValue("window/geometry", saveGeometry());
    settings.setValue("ui/fontSize", _pFontBar->fontSize());
    settings.setValue("ui/alwaysOnTop", _bAlwaysOnTop);
    settings.setValue("ui/timerOnPresent", _bStartTimerOnPresent);
    settings.setValue("ui/vimKeys", _bVimKeys);
    settings.setValue("ui/bionic", _pBionicWidget->isBionicEnabled());
    settings.setValue("ui/bandVisible", _pBionicWidget->isReadingBandVisible());
    settings.setValue("ui/bandPercent", _pBionicWidget->readingBandPosition());
    settings.setValue("ui/columnChars", _pBionicWidget->columnChars());
    settings.setValue("files/script", _sScriptPath);
    settings.setValue("files/slides", _sSlidesPath);
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    saveSettings();

    if (_pPresentation != nullptr)
    {
        _pPresentation->disconnect(this);
        _pPresentation->close();
        _pPresentation = nullptr;
    }

    QMainWindow::closeEvent(pEvent);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *pEvent)
{
    if (pEvent->mimeData()->hasUrls())
    {
        pEvent->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *pEvent)
{
    for (const QUrl &url : pEvent->mimeData()->urls())
    {
        QString sPath = url.toLocalFile();
        QString sSuffix = QFileInfo(sPath).suffix().toLower();

        if (sSuffix == "pdf")
        {
            openSlides(sPath);
        }
        else if (sSuffix == "md" || sSuffix == "txt")
        {
            openFile(sPath);
        }
    }

    pEvent->acceptProposedAction();
}

void MainWindow::openFile(const QString &sFilePath)
{
    if (!sFilePath.isEmpty())
    {
        QFile file(sFilePath);

        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);
            QString sText = in.readAll().trimmed();

            _sScriptPath = sFilePath;
            _pFileWidget->setFileName(QFileInfo(file).fileName());

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
        _sSlidesPath = sFilePath;
        _pSlidesFileWidget->setFileName(QFileInfo(sFilePath).fileName());
        _pSlideStrip->setDeck(_pDeck);
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

    if (_bStartTimerOnPresent && !_pChronometer->isRunning())
    {
        _pChronometer->start();
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

    setWindowFlag(Qt::WindowStaysOnTopHint, _bAlwaysOnTop);
    show();
    activateWindow();
}

void MainWindow::openSettings()
{
    SettingsValues values = {
        static_cast<int>(Theme::instance().mode()),
        _bAlwaysOnTop,
        _pBionicWidget->isReadingBandVisible(),
        _pBionicWidget->readingBandPosition(),
        _pBionicWidget->columnChars(),
        _pBionicWidget->isBionicEnabled(),
        _bStartTimerOnPresent,
        _bVimKeys};
    SettingsDialog dialog(values, this);

    // Changes are applied as they are made; the dialog has nothing to confirm.
    connect(&dialog, &SettingsDialog::themeChanged, this, [](int iTheme)
            { Theme::instance().setMode(static_cast<Theme::Mode>(iTheme)); });
    connect(&dialog, SIGNAL(alwaysOnTopChanged(bool)), this, SLOT(setAlwaysOnTop(bool)));
    connect(&dialog, SIGNAL(bandVisibleChanged(bool)), _pBionicWidget, SLOT(setReadingBandVisible(bool)));
    connect(&dialog, SIGNAL(bandPositionChanged(int)), _pBionicWidget, SLOT(setReadingBandPosition(int)));
    connect(&dialog, SIGNAL(columnCharsChanged(int)), _pBionicWidget, SLOT(setColumnChars(int)));
    connect(&dialog, SIGNAL(bionicChanged(bool)), _pBionicWidget, SLOT(setBionicEnabled(bool)));
    connect(&dialog, &SettingsDialog::timerOnPresentChanged, this, [this](bool bEnabled)
            { _bStartTimerOnPresent = bEnabled; });
    connect(&dialog, &SettingsDialog::vimKeysChanged, this, [this](bool bEnabled)
            { _bVimKeys = bEnabled; });

    dialog.exec();
}

void MainWindow::setAlwaysOnTop(bool bEnabled)
{
    _bAlwaysOnTop = bEnabled;

    // While presenting, the flag stays off; stopPresentation restores it.
    if (_pPresentation == nullptr)
    {
        bool bVisible = isVisible();

        setWindowFlag(Qt::WindowStaysOnTopHint, bEnabled);

        if (bVisible)
        {
            show();
        }
    }
}

void MainWindow::showHelp()
{
    QDialog dialog(this);
    QBoxLayout *pLayout = new QBoxLayout(QBoxLayout::TopToBottom, &dialog);
    QLabel *pLabel = new QLabel(&dialog);
    QDialogButtonBox *pButtons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);

    QString sPrevious = _bVimKeys ? "&larr; / h" : "&larr; / a";
    QString sNext = _bVimKeys ? "&rarr; / l" : "&rarr; / d";
    QString sScroll = _bVimKeys ? "&uarr; / k, &darr; / j" : "&uarr; / w, &darr; / s";
    QString sEdges = _bVimKeys ? "Home / g, End / G" : "Home, End";

    pLabel->setTextFormat(Qt::RichText);
    pLabel->setText(
        "<h3>Keyboard shortcuts</h3>"
        "<table cellspacing='6'>"
        "<tr><td><b>" + sPrevious + "</b></td><td>Previous slide</td></tr>"
        "<tr><td><b>" + sNext + "</b></td><td>Next slide</td></tr>"
        "<tr><td><b>1-9 + Enter</b></td><td>Go to slide number</td></tr>"
        "<tr><td><b>" + sScroll + "</b></td><td>Scroll the script</td></tr>"
        "<tr><td><b>PgUp / PgDn</b></td><td>Scroll a page</td></tr>"
        "<tr><td><b>" + sEdges + "</b></td><td>Top / bottom of the script</td></tr>"
        "<tr><td><b>Ctrl++ / Ctrl+-</b></td><td>Bigger / smaller font</td></tr>"
        "<tr><td><b>Ctrl+,</b></td><td>Open the settings</td></tr>"
        "<tr><td><b>F5</b></td><td>Start the presentation</td></tr>"
        "<tr><td><b>b</b></td><td>Black out the presentation</td></tr>"
        "<tr><td><b>Esc / q</b></td><td>End the presentation</td></tr>"
        "</table>");

    connect(pButtons, SIGNAL(rejected()), &dialog, SLOT(reject()));

    pLayout->addWidget(pLabel);
    pLayout->addWidget(pButtons);

    dialog.setWindowTitle("Shortcuts");
    dialog.exec();
}

void MainWindow::updateSlideCounter()
{
    if (_pDeck->count() == 0)
    {
        _pSlideCounterLabel->setText("No slides");
    }
    else
    {
        _pSlideCounterLabel->setText(QString("Slide %1 / %2").arg(_pDeck->currentIndex() + 1).arg(_pDeck->count()));
    }
}

int MainWindow::translateKey(int iKey) const
{
    // The letter keys stand for the arrows: WASD by default, hjkl when the vim keys are enabled.
    if (_bVimKeys)
    {
        switch (iKey)
        {
        case Qt::Key_H:
            return Qt::Key_Left;
        case Qt::Key_L:
            return Qt::Key_Right;
        case Qt::Key_K:
            return Qt::Key_Up;
        case Qt::Key_J:
            return Qt::Key_Down;
        default:
            break;
        }
    }
    else
    {
        switch (iKey)
        {
        case Qt::Key_A:
            return Qt::Key_Left;
        case Qt::Key_D:
            return Qt::Key_Right;
        case Qt::Key_W:
            return Qt::Key_Up;
        case Qt::Key_S:
            return Qt::Key_Down;
        default:
            break;
        }
    }

    return iKey;
}

bool MainWindow::handleSlideKey(int iKey, bool bPlain)
{
    if (_pDeck->count() == 0 || !bPlain)
    {
        return false;
    }

    if (iKey >= Qt::Key_0 && iKey <= Qt::Key_9)
    {
        _sGoToBuffer.append(QChar('0' + (iKey - Qt::Key_0)));
        statusBar()->showMessage("Go to slide: " + _sGoToBuffer + " (Enter)");
        return true;
    }

    if ((iKey == Qt::Key_Return || iKey == Qt::Key_Enter) && !_sGoToBuffer.isEmpty())
    {
        _pDeck->setCurrent(_sGoToBuffer.toInt() - 1);
        _sGoToBuffer.clear();
        statusBar()->clearMessage();
        return true;
    }

    if (iKey == Qt::Key_Escape && !_sGoToBuffer.isEmpty())
    {
        _sGoToBuffer.clear();
        statusBar()->clearMessage();
        return true;
    }

    switch (iKey)
    {
    case Qt::Key_Left:
        _pDeck->previous();
        return true;
    case Qt::Key_Right:
        _pDeck->next();
        return true;
    case Qt::Key_F5:
        startPresentation();
        return true;
    case Qt::Key_B:
        if (_pPresentation != nullptr)
        {
            _pPresentation->toggleBlackout();
            return true;
        }
        break;
    default:
        break;
    }

    return false;
}

bool MainWindow::handleScriptKey(int iKey, Qt::KeyboardModifiers modifiers, const QString &sText)
{
    bool bPlain = modifiers == Qt::NoModifier;
    bool bShift = modifiers == Qt::ShiftModifier;
    bool bCtrl = (modifiers & ~Qt::ShiftModifier) == Qt::ControlModifier;

    if (bCtrl)
    {
        if (iKey == Qt::Key_Comma)
        {
            openSettings();
            return true;
        }

        if (iKey == Qt::Key_Plus || iKey == Qt::Key_Equal)
        {
            _pFontBar->setFontSize(_pFontBar->fontSize() + 2);
            return true;
        }

        if (iKey == Qt::Key_Minus && _pFontBar->fontSize() > 2)
        {
            _pFontBar->setFontSize(_pFontBar->fontSize() - 2);
            return true;
        }

        return false;
    }

    if (sText == "?" && (bPlain || bShift))
    {
        showHelp();
        return true;
    }

    if (_bVimKeys && bShift && iKey == Qt::Key_G)
    {
        _pBionicWidget->scrollToEdge(true);
        return true;
    }

    if (!bPlain)
    {
        return false;
    }

    if (_bVimKeys && iKey == Qt::Key_G)
    {
        _pBionicWidget->scrollToEdge(false);
        return true;
    }

    switch (iKey)
    {
    case Qt::Key_Up:
        _pBionicWidget->scrollStep(-1);
        return true;
    case Qt::Key_Down:
        _pBionicWidget->scrollStep(1);
        return true;
    case Qt::Key_PageUp:
        _pBionicWidget->scrollPage(-1);
        return true;
    case Qt::Key_PageDown:
        _pBionicWidget->scrollPage(1);
        return true;
    case Qt::Key_Home:
        _pBionicWidget->scrollToEdge(false);
        return true;
    case Qt::Key_End:
        _pBionicWidget->scrollToEdge(true);
        return true;
    default:
        break;
    }

    return false;
}

bool MainWindow::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress && QApplication::activeModalWidget() == nullptr)
    {
        QKeyEvent *pKeyEvent = static_cast<QKeyEvent *>(pEvent);
        Qt::KeyboardModifiers modifiers = pKeyEvent->modifiers() & ~Qt::KeypadModifier;
        int iKey = pKeyEvent->key();

        // Only a bare letter is translated, so Ctrl and Alt combinations keep their meaning.
        if (modifiers == Qt::NoModifier)
        {
            iKey = translateKey(iKey);
        }

        // Alt and Meta combinations are left alone so the mnemonics keep working.
        if (!(modifiers & (Qt::AltModifier | Qt::MetaModifier)))
        {
            if ((iKey == Qt::Key_Escape || iKey == Qt::Key_Q) && modifiers == Qt::NoModifier && _sGoToBuffer.isEmpty() && _pPresentation != nullptr)
            {
                stopPresentation();
                return true;
            }

            if (handleSlideKey(iKey, modifiers == Qt::NoModifier) || handleScriptKey(iKey, modifiers, pKeyEvent->text()))
            {
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(pObject, pEvent);
}
