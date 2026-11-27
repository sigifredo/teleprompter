

// Own
#include <Theme.hpp>

// Qt
#include <QApplication>
#include <QFile>
#include <QGuiApplication>
#include <QPalette>
#include <QSettings>
#include <QStyleHints>

namespace
{
struct Tokens
{
    QColor bg, surface, hover, pressed, border;
    QColor text, muted, accent, accentText;
    QColor script, scriptText, disabledBg, disabledText;
    QColor primary, primaryHover, primaryText;
};

// Neutral cool greys with a single blue accent, after GitHub's Primer palette. Text contrast is
// kept around 13:1 for the script instead of the maximum, because glare is what tires the eyes.
Tokens darkTokens()
{
    return {
        "#0d1117", "#151b23", "#212830", "#262c36", "#30363d",
        "#e6edf3", "#9198a1", "#4493f8", "#0d1117",
        "#0d1117", "#d1d9e0", "#151b23", "#656c76",
        "#1f6feb", "#1a65d8", "#ffffff"};
}

Tokens lightTokens()
{
    return {
        "#ffffff", "#f6f8fa", "#eff2f5", "#e6eaef", "#d1d9e0",
        "#1f2328", "#59636e", "#0969da", "#ffffff",
        "#ffffff", "#1f2328", "#eff2f5", "#818b98",
        "#0969da", "#0860ca", "#ffffff"};
}
}

Theme::Theme()
    : QObject(nullptr), _mode(Dark)
{
}

Theme &Theme::instance()
{
    static Theme theme;

    return theme;
}

Theme::Mode Theme::mode() const
{
    return _mode;
}

bool Theme::isDark() const
{
    if (_mode == Auto)
    {
        return QGuiApplication::styleHints()->colorScheme() != Qt::ColorScheme::Light;
    }

    return _mode == Dark;
}

QString Theme::modeName() const
{
    switch (_mode)
    {
    case Light:
        return "Light";
    case Auto:
        return "Auto";
    default:
        return "Dark";
    }
}

void Theme::load()
{
    int iMode = QSettings().value("ui/theme", Dark).toInt();

    _mode = (iMode >= Dark && iMode <= Auto) ? static_cast<Mode>(iMode) : Dark;

    // Auto follows the operating system, so repaint when it changes.
    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, [this]()
            {
        if (_mode == Auto)
        {
            apply();
        } });

    apply();
}

void Theme::setMode(Mode mode)
{
    _mode = mode;
    QSettings().setValue("ui/theme", static_cast<int>(mode));
    apply();
}

void Theme::cycle()
{
    setMode(static_cast<Mode>((_mode + 1) % 3));
}

void Theme::apply()
{
    Tokens t = isDark() ? darkTokens() : lightTokens();

    QPalette palette;
    palette.setColor(QPalette::Window, t.bg);
    palette.setColor(QPalette::WindowText, t.text);
    palette.setColor(QPalette::Base, t.script);
    palette.setColor(QPalette::AlternateBase, t.surface);
    palette.setColor(QPalette::Text, t.scriptText);
    palette.setColor(QPalette::Button, t.surface);
    palette.setColor(QPalette::ButtonText, t.text);
    palette.setColor(QPalette::Highlight, t.accent);
    palette.setColor(QPalette::HighlightedText, t.accentText);
    palette.setColor(QPalette::PlaceholderText, t.muted);
    palette.setColor(QPalette::ToolTipBase, t.surface);
    palette.setColor(QPalette::ToolTipText, t.text);
    palette.setColor(QPalette::Link, t.accent);
    palette.setColor(QPalette::Mid, t.border);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, t.disabledText);
    palette.setColor(QPalette::Disabled, QPalette::WindowText, t.disabledText);
    palette.setColor(QPalette::Disabled, QPalette::Text, t.disabledText);
    qApp->setPalette(palette);

    QFile file(":/styles.qss");
    QString sStyle;

    if (file.open(QFile::ReadOnly))
    {
        sStyle = QString::fromUtf8(file.readAll());
    }

    const QList<QPair<QString, QColor>> replacements = {
        {"@bg@", t.bg}, {"@surface@", t.surface}, {"@hover@", t.hover}, {"@pressed@", t.pressed}, {"@border@", t.border}, {"@text@", t.text}, {"@muted@", t.muted}, {"@accent@", t.accent}, {"@accentText@", t.accentText}, {"@script@", t.script}, {"@scriptText@", t.scriptText}, {"@disabledBg@", t.disabledBg}, {"@disabledText@", t.disabledText}, {"@primary@", t.primary}, {"@primaryHover@", t.primaryHover}, {"@primaryText@", t.primaryText}};

    for (const QPair<QString, QColor> &replacement : replacements)
    {
        sStyle.replace(replacement.first, replacement.second.name());
    }

    // The check mark must contrast with the accent: dark on the light blue, white on the dark blue.
    sStyle.replace("@check@", isDark() ? ":/icons/check-dark.png" : ":/icons/check-light.png");

    qApp->setStyleSheet(sStyle);
    emit changed();
}
