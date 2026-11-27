

// own
#include <FontBarWidget.hpp>

// qt
#include <QBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QPushButton>

namespace
{
// Texto de un atajo con Ctrl; en macOS Qt lo asigna a la tecla Command y así se muestra.
QString shortcutText(int key)
{
    QString text = QKeySequence(Qt::CTRL | key).toString(QKeySequence::PortableText);
#ifdef Q_OS_MACOS
    text.replace(QStringLiteral("Ctrl"), QStringLiteral("Command"));
#endif
    return text;
}
} // namespace

namespace
{
    // Cuánto cambia el tamaño en cada pulsación; también es el tamaño mínimo.
    constexpr int FontStep = 2;
} // namespace

FontBarWidget::FontBarWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    layout->setContentsMargins(0, 0, 0, 0);

    auto *plusButton = new QPushButton(QStringLiteral("A+"), this);
    auto *minusButton = new QPushButton(QStringLiteral("A-"), this);

    fontSizeLabel_ = new QLabel(QString::number(DefaultFontSize), this);
    fontSizeLabel_->setProperty("role", "font-size");

    plusButton->setToolTip(tr("Increase font size (%1)").arg(shortcutText(Qt::Key_Plus)));
    minusButton->setToolTip(tr("Decrease font size (%1)").arg(shortcutText(Qt::Key_Minus)));

    connect(plusButton, &QPushButton::clicked, this, &FontBarWidget::incrementFontSize);
    connect(minusButton, &QPushButton::clicked, this, &FontBarWidget::decrementFontSize);

    layout->addWidget(minusButton);
    layout->addWidget(fontSizeLabel_);
    layout->addWidget(plusButton);
}

int FontBarWidget::fontSize() const
{
    return fontSizeLabel_->text().toInt();
}

void FontBarWidget::setFontSize(int fontSize)
{
    fontSizeLabel_->setText(QString::number(fontSize));
    emit changeFontSize(fontSize);
}

void FontBarWidget::decrementFontSize()
{
    int size = fontSize();

    if (size > FontStep)
        size -= FontStep;

    setFontSize(size);
}

void FontBarWidget::incrementFontSize()
{
    setFontSize(fontSize() + FontStep);
}
