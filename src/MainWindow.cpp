

// own
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

// qt
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

namespace
{
    // Separaciones del diseño, en píxeles.
    constexpr int LayoutSpacing = 10;
    constexpr int BarSpacing = 16;
    constexpr int BarBottomMargin = 10;
    constexpr int BodySpacing = 12;
    constexpr int PreviewSpacing = 8;

    // Reparto del espacio: guion y vistas a un lado, diapositiva actual y siguiente en el otro.
    constexpr int ScriptStretch = 3;
    constexpr int PreviewStretch = 2;
    constexpr int CurrentSlideStretch = 3;
    constexpr int NextSlideStretch = 2;
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    auto *layout = new QBoxLayout(QBoxLayout::TopToBottom, centralWidget);

    deck_ = new SlideDeck(this);
    fileWidget_ = new FileWidget(tr("Open &script..."), tr("Script (*.md *.txt)"), centralWidget);
    slidesFileWidget_ = new FileWidget(tr("Open s&lides..."), tr("Slides (*.pdf)"), centralWidget);
    chronometer_ = new ChronometerWidget(centralWidget);
    fontBar_ = new FontBarWidget(centralWidget);
    presentButton_ = new QPushButton(tr("Present (F5)"), centralWidget);
    bionicWidget_ = new BionicWidget(centralWidget);
    slideStrip_ = new SlideStripWidget(centralWidget);

    // Barra única de controles: archivos, cronómetro, tamaño de letra y presentación.
    auto *bar = new QWidget(centralWidget);
    auto *barLayout = new QBoxLayout(QBoxLayout::LeftToRight, bar);

    bar->setProperty("role", "toolbar");
    barLayout->setContentsMargins(0, 0, 0, BarBottomMargin);
    barLayout->setSpacing(BarSpacing);
    barLayout->addWidget(fileWidget_);
    barLayout->addWidget(slidesFileWidget_);
    barLayout->addStretch(1);
    barLayout->addWidget(chronometer_);
    barLayout->addWidget(fontBar_);
    barLayout->addWidget(presentButton_);

    // El guion a la izquierda y, a la derecha, la diapositiva actual y la siguiente (vista del presentador).
    auto *body = new QWidget(centralWidget);
    auto *bodyLayout = new QBoxLayout(QBoxLayout::LeftToRight, body);
    auto *previewLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(BodySpacing);
    previewLayout->setSpacing(PreviewSpacing);
    previewLayout->addWidget(new SlidePreviewWidget(deck_, 0, tr("Current"), body), CurrentSlideStretch);
    previewLayout->addWidget(new SlidePreviewWidget(deck_, 1, tr("Next"), body), NextSlideStretch);

    bodyLayout->addWidget(bionicWidget_, ScriptStretch);
    bodyLayout->addLayout(previewLayout, PreviewStretch);

    presentButton_->setProperty("variant", "primary");
    presentButton_->setEnabled(false);
    presentButton_->setToolTip(tr("Start the presentation (F5)"));
    bionicWidget_->setPlaceholderText(tr("Open a script (.md / .txt)\nand slides (.pdf)\n\nPress ? for shortcuts"));

    layout->setSpacing(LayoutSpacing);
    layout->addWidget(bar);
    layout->addWidget(body, 1);
    layout->addWidget(slideStrip_);
    setCentralWidget(centralWidget);

    // Barra de estado: contador de diapositivas y opciones de la vista.
    slideCounterLabel_ = new QLabel(tr("No slides"), this);

    auto *settingsButton = new QPushButton(tr("Settings"), this);
    auto *helpButton = new QPushButton(QStringLiteral("?"), this);

    settingsButton->setProperty("variant", "ghost");
    settingsButton->setToolTip(tr("Settings (Ctrl+,)"));
    helpButton->setProperty("variant", "ghost");
    helpButton->setToolTip(tr("Keyboard shortcuts (?)"));

    statusBar()->setSizeGripEnabled(false);
    statusBar()->addWidget(slideCounterLabel_);
    statusBar()->addPermanentWidget(helpButton);
    statusBar()->addPermanentWidget(settingsButton);
    statusBar()->addPermanentWidget(new VersionWidget(this));

    // Los botones solo toman el foco con Tab: un clic no le quita el teclado al guion.
    for (QPushButton *button : findChildren<QPushButton *>())
        button->setFocusPolicy(Qt::TabFocus);

    connect(fileWidget_, &FileWidget::fileSelected, this, &MainWindow::openFile);
    connect(slidesFileWidget_, &FileWidget::fileSelected, this, &MainWindow::openSlides);
    connect(fontBar_, &FontBarWidget::changeFontSize, bionicWidget_, &BionicWidget::setFontSize);
    connect(deck_, &SlideDeck::currentChanged, this, &MainWindow::updateSlideCounter);
    connect(presentButton_, &QPushButton::clicked, this, &MainWindow::startPresentation);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);

#if defined(Q_OS_WIN)
    setWindowIcon(QIcon(":/teleprompter/app-icons/teleprompter.ico"));
#elif defined(Q_OS_MACOS)
    setWindowIcon(QIcon(":/teleprompter/app-icons/teleprompter.icns"));
#else
    setWindowIcon(QIcon(":/teleprompter/app-icons/teleprompter.png"));
#endif

    setAcceptDrops(true);
    qApp->installEventFilter(this);

    bionicWidget_->setFontSize(fontBar_->fontSize());
    loadSettings();

    // Las teclas van al guion desde el principio; a los botones se llega con Tab.
    bionicWidget_->setFocus();
}

bool MainWindow::hasSavedGeometry() const
{
    return QSettings().contains("window/geometry");
}

void MainWindow::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Error"), tr("Could not open the file: %1").arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    const QString text = in.readAll().trimmed();
    const QFileInfo info(file);

    scriptPath_ = filePath;
    fileWidget_->setFileName(info.fileName());

    if (info.suffix().compare("md", Qt::CaseInsensitive) == 0)
        bionicWidget_->setBionicMarkdown(text);
    else
        bionicWidget_->setBionicText(text);
}

void MainWindow::openSlides(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    if (!deck_->load(filePath))
    {
        QMessageBox::warning(this, tr("Error"), tr("Could not open the slides: %1").arg(filePath));
        return;
    }

    slidesPath_ = filePath;
    slidesFileWidget_->setFileName(QFileInfo(filePath).fileName());
    slideStrip_->setDeck(deck_);
    presentButton_->setEnabled(true);
    updateSlideCounter();
}

void MainWindow::startPresentation()
{
    if (presentation_ != nullptr || deck_->count() == 0)
        return;

    // Con una sola pantalla, esta ventana no puede quedar por encima de la presentación.
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    show();

    presentation_ = new PresentationWindow(deck_);
    presentation_->setAttribute(Qt::WA_DeleteOnClose);
    connect(presentation_, &PresentationWindow::closed, this, &MainWindow::stopPresentation);

    QScreen *target = nullptr;

    for (QScreen *candidate : QGuiApplication::screens())
    {
        if (candidate != screen())
        {
            target = candidate;
            break;
        }
    }

    if (target != nullptr)
    {
        presentation_->move(target->geometry().topLeft());
        presentation_->showFullScreen();
    }
    else
    {
        presentation_->showMaximized();
    }

    if (startTimerOnPresent_ && !chronometer_->isRunning())
        chronometer_->start();

    activateWindow();
    raise();
}

void MainWindow::stopPresentation()
{
    if (presentation_ == nullptr)
        return;

    PresentationWindow *presentation = presentation_;

    presentation_ = nullptr;
    presentation->disconnect(this);
    presentation->close();

    setWindowFlag(Qt::WindowStaysOnTopHint, alwaysOnTop_);
    show();
    activateWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();

    if (presentation_ != nullptr)
    {
        presentation_->disconnect(this);
        presentation_->close();
        presentation_ = nullptr;
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    for (const QUrl &url : event->mimeData()->urls())
    {
        const QString path = url.toLocalFile();
        const QString suffix = QFileInfo(path).suffix().toLower();

        if (suffix == "pdf")
            openSlides(path);
        else if (suffix == "md" || suffix == "txt")
            openFile(path);
    }

    event->acceptProposedAction();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() != QEvent::KeyPress || QApplication::activeModalWidget() != nullptr)
        return QMainWindow::eventFilter(object, event);

    const auto *keyEvent = static_cast<QKeyEvent *>(event);
    const Qt::KeyboardModifiers modifiers = keyEvent->modifiers() & ~Qt::KeypadModifier;
    int key = keyEvent->key();

    // Las combinaciones con Alt o Meta se dejan pasar para que sigan funcionando los mnemónicos.
    if (modifiers & (Qt::AltModifier | Qt::MetaModifier))
        return QMainWindow::eventFilter(object, event);

    // Solo se traduce una letra sola, así Ctrl y Shift conservan su significado.
    if (modifiers == Qt::NoModifier)
        key = translateKey(key);

    if ((key == Qt::Key_Escape || key == Qt::Key_Q) && modifiers == Qt::NoModifier && goToBuffer_.isEmpty() && presentation_ != nullptr)
    {
        stopPresentation();
        return true;
    }

    if (handleSlideKey(key, modifiers == Qt::NoModifier) || handleScriptKey(key, modifiers, keyEvent->text()))
        return true;

    return QMainWindow::eventFilter(object, event);
}

bool MainWindow::handleScriptKey(int key, Qt::KeyboardModifiers modifiers, const QString &text)
{
    const bool plain = modifiers == Qt::NoModifier;
    const bool shift = modifiers == Qt::ShiftModifier;
    const bool ctrl = (modifiers & ~Qt::ShiftModifier) == Qt::ControlModifier;

    if (ctrl)
    {
        if (key == Qt::Key_Comma)
        {
            openSettings();
            return true;
        }

        if (key == Qt::Key_Plus || key == Qt::Key_Equal)
        {
            fontBar_->incrementFontSize();
            return true;
        }

        if (key == Qt::Key_Minus)
        {
            fontBar_->decrementFontSize();
            return true;
        }

        return false;
    }

    if (text == "?" && (plain || shift))
    {
        showHelp();
        return true;
    }

    if (vimKeys_ && shift && key == Qt::Key_G)
    {
        bionicWidget_->scrollToEdge(true);
        return true;
    }

    if (!plain)
        return false;

    if (vimKeys_ && key == Qt::Key_G)
    {
        bionicWidget_->scrollToEdge(false);
        return true;
    }

    switch (key)
    {
    case Qt::Key_Up:
        bionicWidget_->scrollStep(-1);
        return true;
    case Qt::Key_Down:
        bionicWidget_->scrollStep(1);
        return true;
    case Qt::Key_PageUp:
        bionicWidget_->scrollPage(-1);
        return true;
    case Qt::Key_PageDown:
        bionicWidget_->scrollPage(1);
        return true;
    case Qt::Key_Home:
        bionicWidget_->scrollToEdge(false);
        return true;
    case Qt::Key_End:
        bionicWidget_->scrollToEdge(true);
        return true;
    default:
        return false;
    }
}

bool MainWindow::handleSlideKey(int key, bool plain)
{
    if (deck_->count() == 0 || !plain)
        return false;

    if (key >= Qt::Key_0 && key <= Qt::Key_9)
    {
        goToBuffer_.append(QChar('0' + (key - Qt::Key_0)));
        statusBar()->showMessage(tr("Go to slide: %1 (Enter)").arg(goToBuffer_));
        return true;
    }

    if ((key == Qt::Key_Return || key == Qt::Key_Enter) && !goToBuffer_.isEmpty())
    {
        deck_->setCurrent(goToBuffer_.toInt() - 1);
        goToBuffer_.clear();
        statusBar()->clearMessage();
        return true;
    }

    if (key == Qt::Key_Escape && !goToBuffer_.isEmpty())
    {
        goToBuffer_.clear();
        statusBar()->clearMessage();
        return true;
    }

    switch (key)
    {
    case Qt::Key_Left:
        deck_->previous();
        return true;
    case Qt::Key_Right:
        deck_->next();
        return true;
    case Qt::Key_F5:
        startPresentation();
        return true;
    case Qt::Key_B:
        if (presentation_ == nullptr)
            return false;

        presentation_->toggleBlackout();
        return true;
    default:
        return false;
    }
}

void MainWindow::loadSettings()
{
    QSettings settings;

    restoreGeometry(settings.value("window/geometry").toByteArray());
    fontBar_->setFontSize(settings.value("ui/fontSize", fontBar_->fontSize()).toInt());

    setAlwaysOnTop(settings.value("ui/alwaysOnTop", true).toBool());
    startTimerOnPresent_ = settings.value("ui/timerOnPresent", false).toBool();
    vimKeys_ = settings.value("ui/vimKeys", false).toBool();
    bionicWidget_->setBionicEnabled(settings.value("ui/bionic", true).toBool());
    bionicWidget_->setReadingBandVisible(settings.value("ui/bandVisible", false).toBool());
    bionicWidget_->setReadingBandPosition(settings.value("ui/bandPercent", bionicWidget_->readingBandPosition()).toInt());
    bionicWidget_->setColumnChars(settings.value("ui/columnChars", bionicWidget_->columnChars()).toInt());

    const QString script = settings.value("files/script").toString();
    const QString slides = settings.value("files/slides").toString();

    if (!script.isEmpty() && QFileInfo::exists(script))
        openFile(script);

    if (!slides.isEmpty() && QFileInfo::exists(slides))
        openSlides(slides);
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.setValue("window/geometry", saveGeometry());
    settings.setValue("ui/fontSize", fontBar_->fontSize());
    settings.setValue("ui/alwaysOnTop", alwaysOnTop_);
    settings.setValue("ui/timerOnPresent", startTimerOnPresent_);
    settings.setValue("ui/vimKeys", vimKeys_);
    settings.setValue("ui/bionic", bionicWidget_->isBionicEnabled());
    settings.setValue("ui/bandVisible", bionicWidget_->isReadingBandVisible());
    settings.setValue("ui/bandPercent", bionicWidget_->readingBandPosition());
    settings.setValue("ui/columnChars", bionicWidget_->columnChars());
    settings.setValue("files/script", scriptPath_);
    settings.setValue("files/slides", slidesPath_);
}

int MainWindow::translateKey(int key) const
{
    // Las letras hacen de flechas: WASD por defecto, hjkl con las teclas de vim.
    if (vimKeys_)
    {
        switch (key)
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
            return key;
        }
    }

    switch (key)
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
        return key;
    }
}

void MainWindow::openSettings()
{
    const SettingsValues values = {
        .alwaysOnTop = alwaysOnTop_,
        .bandPercent = bionicWidget_->readingBandPosition(),
        .bandVisible = bionicWidget_->isReadingBandVisible(),
        .bionic = bionicWidget_->isBionicEnabled(),
        .columnChars = bionicWidget_->columnChars(),
        .theme = static_cast<int>(Theme::instance().mode()),
        .timerOnPresent = startTimerOnPresent_,
        .vimKeys = vimKeys_};
    SettingsDialog dialog(values, this);

    // Los cambios se aplican al hacerlos: el diálogo no tiene nada que confirmar.
    connect(&dialog, &SettingsDialog::themeChanged, this, [](int theme)
            { Theme::instance().setMode(static_cast<Theme::Mode>(theme)); });
    connect(&dialog, &SettingsDialog::alwaysOnTopChanged, this, &MainWindow::setAlwaysOnTop);
    connect(&dialog, &SettingsDialog::bandVisibleChanged, bionicWidget_, &BionicWidget::setReadingBandVisible);
    connect(&dialog, &SettingsDialog::bandPositionChanged, bionicWidget_, &BionicWidget::setReadingBandPosition);
    connect(&dialog, &SettingsDialog::columnCharsChanged, bionicWidget_, &BionicWidget::setColumnChars);
    connect(&dialog, &SettingsDialog::bionicChanged, bionicWidget_, &BionicWidget::setBionicEnabled);
    connect(&dialog, &SettingsDialog::timerOnPresentChanged, this, [this](bool enabled)
            { startTimerOnPresent_ = enabled; });
    connect(&dialog, &SettingsDialog::vimKeysChanged, this, [this](bool enabled)
            { vimKeys_ = enabled; });

    dialog.exec();
}

void MainWindow::setAlwaysOnTop(bool enabled)
{
    alwaysOnTop_ = enabled;

    // Durante la presentación la marca queda apagada; stopPresentation() la restablece.
    if (presentation_ != nullptr)
        return;

    const bool visible = isVisible();

    setWindowFlag(Qt::WindowStaysOnTopHint, enabled);

    // Cambiar una marca de ventana la oculta: si estaba a la vista, se vuelve a mostrar.
    if (visible)
        show();
}

void MainWindow::showHelp()
{
    QDialog dialog(this);
    auto *layout = new QBoxLayout(QBoxLayout::TopToBottom, &dialog);
    auto *label = new QLabel(&dialog);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);

    const auto row = [](const QString &keys, const QString &action)
    { return "<tr><td><b>" + keys + "</b></td><td>" + action + "</td></tr>"; };

    const QString previous = vimKeys_ ? "&larr; / h" : "&larr; / a";
    const QString next = vimKeys_ ? "&rarr; / l" : "&rarr; / d";
    const QString scroll = vimKeys_ ? "&uarr; / k, &darr; / j" : "&uarr; / w, &darr; / s";
    const QString edges = vimKeys_ ? "Home / g, End / G" : "Home, End";

    label->setTextFormat(Qt::RichText);
    label->setText("<h3>" + tr("Keyboard shortcuts") + "</h3><table cellspacing='6'>" +
                   row(previous, tr("Previous slide")) +
                   row(next, tr("Next slide")) +
                   row("1-9 + Enter", tr("Go to slide number")) +
                   row(scroll, tr("Scroll the script")) +
                   row("PgUp / PgDn", tr("Scroll a page")) +
                   row(edges, tr("Top / bottom of the script")) +
                   row("Ctrl++ / Ctrl+-", tr("Bigger / smaller font")) +
                   row("Ctrl+,", tr("Open the settings")) +
                   row("F5", tr("Start the presentation")) +
                   row("b", tr("Black out the presentation")) +
                   row("Esc / q", tr("End the presentation")) +
                   "</table>");

    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    layout->addWidget(label);
    layout->addWidget(buttons);

    dialog.setWindowTitle(tr("Shortcuts"));
    dialog.exec();
}

void MainWindow::updateSlideCounter()
{
    if (deck_->count() == 0)
        slideCounterLabel_->setText(tr("No slides"));
    else
        slideCounterLabel_->setText(tr("Slide %1 / %2").arg(deck_->currentIndex() + 1).arg(deck_->count()));
}
