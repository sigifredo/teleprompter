/**
 * Teleprompter: A bionic reading and time control application.
 * Copyright (C) 2024 Sigifredo Escobar Gómez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// own
#include <MainWindow.hpp>
#include <Theme.hpp>
#include <Version.hpp>

// qt
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QScreen>

namespace
{
    // Tamaño inicial de la ventana, como fracción de la pantalla.
    constexpr double WindowWidthFraction = 0.6;
    constexpr double WindowHeightFraction = 0.7;

    // Rectángulo centrado en la pantalla principal.
    QRect centerGeometry()
    {
        const QRect screen = QGuiApplication::primaryScreen()->geometry();
        const int width = static_cast<int>(screen.width() * WindowWidthFraction);
        const int height = static_cast<int>(screen.height() * WindowHeightFraction);

        return QRect((screen.width() - width) / 2, (screen.height() - height) / 2, width, height);
    }

    // Fuente del visor biónico: se empaqueta como recurso para que se vea igual en todas las plataformas.
    void loadBundledFonts()
    {
        for (const QString style : {"Regular", "Italic", "Bold", "BoldItalic"})
            QFontDatabase::addApplicationFont(":/teleprompter/fonts/AtkinsonHyperlegibleNext-" + style + ".ttf");
    }

    // Si la familia por defecto no existe en el sistema (p. ej. "Sans Serif"), Qt pierde tiempo resolviendo alias.
    void ensureDefaultFontFamily()
    {
        QFont font = QGuiApplication::font();

        if (QFontDatabase::hasFamily(font.family()))
            return;

        const QString family = QFontDatabase::systemFont(QFontDatabase::GeneralFont).family();

        if (QFontDatabase::hasFamily(family))
            font.setFamily(family);
        else if (const QStringList families = QFontDatabase::families(); !families.isEmpty())
            font.setFamily(families.first());
        else
            return;

        QGuiApplication::setFont(font);
    }
} // namespace

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("sigifredo");
    QCoreApplication::setOrganizationDomain("sigifredo.dev");
    QCoreApplication::setApplicationName("teleprompter");
    QGuiApplication::setDesktopFileName("dev.sigifredo.teleprompter");

    loadBundledFonts();
    ensureDefaultFontFamily();

    app.setStyle("Fusion");
    Theme::instance().load();

    MainWindow window;

    if (!window.hasSavedGeometry())
        window.setGeometry(centerGeometry());

    window.setWindowTitle(PROJECT_NAME " v" PROJECT_VERSION);
    window.show();

    return app.exec();
}
