

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
};

Tokens darkTokens()
{
    return {
        "#121417", "#1c1f24", "#262a31", "#30353d", "#2c3139",
        "#e4e6ea", "#9aa1ad", "#e0a458", "#14161a",
        "#0e1013", "#eceef2", "#181b1f", "#5c6370"};
}

Tokens lightTokens()
{
    return {
        "#f6f7f9", "#e6e8ec", "#d9dce2", "#cbcfd6", "#cfd3da",
        "#1d2127", "#5a616d", "#a8590f", "#ffffff",
        "#ffffff", "#14161a", "#eceef1", "#9097a3"};
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
        {"@bg@", t.bg}, {"@surface@", t.surface}, {"@hover@", t.hover}, {"@pressed@", t.pressed}, {"@border@", t.border}, {"@text@", t.text}, {"@muted@", t.muted}, {"@accent@", t.accent}, {"@accentText@", t.accentText}, {"@script@", t.script}, {"@scriptText@", t.scriptText}, {"@disabledBg@", t.disabledBg}, {"@disabledText@", t.disabledText}};

    for (const QPair<QString, QColor> &replacement : replacements)
    {
        sStyle.replace(replacement.first, replacement.second.name());
    }

    qApp->setStyleSheet(sStyle);
    emit changed();
}
