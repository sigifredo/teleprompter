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

#ifndef THEME_HPP
#define THEME_HPP

// qt
#include <QColor>
#include <QHash>
#include <QObject>
#include <QString>

/**
 * @brief Tema visual de Teleprompter: oscuro, claro o el del sistema.
 *
 * Deriva la paleta de Qt y la hoja de estilos de `design/teleprompter.tokens.json`, que viaja como
 * recurso. La hoja de estilos es una plantilla con `{{token}}`: se pasa por expand() y nunca lleva
 * colores literales.
 *
 * Uso, una vez por proceso y antes de mostrar una ventana:
 * @code
 *   Theme::instance().load();
 * @endcode
 */
class Theme : public QObject
{
    Q_OBJECT

public:
    enum class Mode
    {
        Dark,
        Light,
        Auto ///< Sigue el esquema de colores del sistema.
    };

    static Theme &instance();

    /**
     * @brief Lee el modo guardado, aplica el tema y, en modo automático, lo mantiene al día con el sistema.
     */
    void load();

    Mode mode() const { return mode_; }

    /**
     * @brief Indica si el tema aplicado es el oscuro, sea por el modo o por el sistema.
     */
    bool isDark() const;

    /**
     * @brief Nombre del tema aplicado en los tokens y en los recursos: "dark" o "light".
     */
    QString variantName() const;

    /**
     * @brief Valor de un token del tema aplicado, tal como está en el JSON.
     *
     * @return Una cadena vacía si no existe.
     */
    QString token(const QString &name) const;

    /**
     * @brief Color de un token. Si no existe devuelve un QColor inválido.
     */
    QColor color(const QString &name) const;

    /**
     * @brief Sustituye cada `{{token}}` de una plantilla de hoja de estilos por su valor.
     */
    QString expand(const QString &templateText) const;

    /**
     * @brief Hoja de estilos base ya expandida.
     */
    QString styleSheet() const;

public slots:
    /**
     * @brief Cambia el modo, lo guarda y aplica el tema.
     */
    void setMode(Mode mode);

signals:
    void changed();

private:
    Theme();

    void apply();
    void loadTokens();
    void rebuildValues();

    Mode mode_ = Mode::Dark;
    QHash<QString, QString> values_;
    QHash<QString, QString> themeDark_;
    QHash<QString, QString> themeLight_;
};

#endif
