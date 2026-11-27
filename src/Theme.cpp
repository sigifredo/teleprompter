

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

// Cool, low-chroma palettes designed in OKLCH (hue ~250). Text contrast is kept around
// 13:1 instead of the maximum, because glare is what tires the eyes in long readings.
Tokens darkTokens()
{
    return {
        "#0e141c", "#171f27", "#222b35", "#2d3843", "#29313a",
        "#d1dbe1", "#98a7b4", "#65d0dc", "#0e141c",
        "#090f15", "#cdd8de", "#13191f", "#606a74"};
}

Tokens lightTokens()
{
    return {
        "#eff4f9", "#e0e7ee", "#d5dee6", "#c8d3dc", "#cad2da",
        "#182230", "#4e5c6b", "#006e8e", "#ffffff",
        "#fafcfe", "#182230", "#e7ecf0", "#828e98"};
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
