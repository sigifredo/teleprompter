

// own
#include <Theme.hpp>

// qt
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPalette>
#include <QRegularExpression>
#include <QSettings>
#include <QStyleHints>

#define CLASS_NAME "Theme"
#define TELEPROMPTER_WARNING() qWarning() << "[" CLASS_NAME "]"

namespace
{
    QHash<QString, QString> readStrings(const QJsonObject &object)
    {
        QHash<QString, QString> out;

        for (auto it = object.begin(); it != object.end(); ++it)
            out.insert(it.key(), it.value().toString());

        return out;
    }
} // namespace

Theme::Theme()
{
    loadTokens();
    rebuildValues();
}

Theme &Theme::instance()
{
    static Theme theme;

    return theme;
}

void Theme::load()
{
    const int mode = QSettings().value("ui/theme", static_cast<int>(Mode::Dark)).toInt();

    mode_ = mode >= static_cast<int>(Mode::Dark) && mode <= static_cast<int>(Mode::Auto) ? static_cast<Mode>(mode) : Mode::Dark;

    // En modo automático el tema se vuelve a aplicar cuando el sistema cambia de esquema.
    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, [this]()
            {
        if (mode_ == Mode::Auto)
            apply(); });

    apply();
}

bool Theme::isDark() const
{
    if (mode_ == Mode::Auto)
        return QGuiApplication::styleHints()->colorScheme() != Qt::ColorScheme::Light;

    return mode_ == Mode::Dark;
}

QString Theme::variantName() const
{
    return isDark() ? QStringLiteral("dark") : QStringLiteral("light");
}

QString Theme::token(const QString &name) const
{
    return values_.value(name);
}

QColor Theme::color(const QString &name) const
{
    const QString value = values_.value(name);

    return value.startsWith('#') ? QColor(value) : QColor();
}

QString Theme::expand(const QString &templateText) const
{
    static const QRegularExpression placeholder(QStringLiteral("\\{\\{([a-z0-9-]+)\\}\\}"));

    QString out;
    qsizetype last = 0;

    for (auto it = placeholder.globalMatch(templateText); it.hasNext();)
    {
        const QRegularExpressionMatch match = it.next();
        const QString name = match.captured(1);

        if (!values_.contains(name))
            TELEPROMPTER_WARNING() << "Token desconocido en la hoja de estilos:" << name;

        out += QStringView(templateText).mid(last, match.capturedStart() - last);
        out += values_.value(name);
        last = match.capturedEnd();
    }

    out += QStringView(templateText).mid(last);

    return out;
}

QString Theme::styleSheet() const
{
    QFile file(QStringLiteral(":/teleprompter/qss/teleprompter.qss"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        TELEPROMPTER_WARNING() << "No se pudo abrir la hoja de estilos.";
        return {};
    }

    return expand(QString::fromUtf8(file.readAll()));
}

void Theme::setMode(Mode mode)
{
    mode_ = mode;
    QSettings().setValue("ui/theme", static_cast<int>(mode));
    apply();
}

void Theme::apply()
{
    rebuildValues();

    const QColor fg = color("fg");
    const QColor subtle = color("fg-subtle");
    QPalette palette;

    palette.setColor(QPalette::Window, color("bg"));
    palette.setColor(QPalette::WindowText, fg);
    palette.setColor(QPalette::Base, color("script-bg"));
    palette.setColor(QPalette::Text, color("script-fg"));
    palette.setColor(QPalette::AlternateBase, color("surface"));
    palette.setColor(QPalette::Button, color("surface"));
    palette.setColor(QPalette::ButtonText, fg);
    palette.setColor(QPalette::ToolTipBase, color("surface"));
    palette.setColor(QPalette::ToolTipText, fg);
    palette.setColor(QPalette::Highlight, color("accent"));
    palette.setColor(QPalette::HighlightedText, color("accent-text"));
    palette.setColor(QPalette::PlaceholderText, color("fg-muted"));
    palette.setColor(QPalette::Link, color("accent"));
    palette.setColor(QPalette::Mid, color("line"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, subtle);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, subtle);
    palette.setColor(QPalette::Disabled, QPalette::Text, subtle);

    qApp->setPalette(palette);
    qApp->setStyleSheet(styleSheet());

    emit changed();
}

void Theme::loadTokens()
{
    QFile file(QStringLiteral(":/teleprompter/tokens.json"));

    if (!file.open(QIODevice::ReadOnly))
    {
        TELEPROMPTER_WARNING() << "No se pudieron leer los tokens del tema.";
        return;
    }

    const QJsonObject themes = QJsonDocument::fromJson(file.readAll()).object().value("themes").toObject();

    themeDark_ = readStrings(themes.value("dark").toObject());
    themeLight_ = readStrings(themes.value("light").toObject());
}

void Theme::rebuildValues()
{
    values_ = isDark() ? themeDark_ : themeLight_;
    values_.insert("theme", variantName());
}
