/*
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

/**
 * @file Theme.hpp
 * @brief Tema visual de la aplicación.
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
    /**
     * @brief Modo del tema.
     */
    enum class Mode
    {
        Dark,  ///< Tema oscuro.
        Light, ///< Tema claro.
        Auto   ///< Sigue el esquema de colores del sistema.
    };

    /**
     * @brief Color de un token.
     *
     * @param name Nombre del token.
     * @return El color, o un QColor inválido si el token no existe o no es un color.
     */
    QColor color(const QString &name) const;

    /**
     * @brief Sustituye cada `{{token}}` de una plantilla de hoja de estilos por su valor.
     *
     * Un token desconocido se sustituye por una cadena vacía y deja un aviso en el registro.
     *
     * @param templateText Plantilla de hoja de estilos.
     * @return La plantilla con los tokens sustituidos.
     */
    QString expand(const QString &templateText) const;

    /**
     * @brief Única instancia del tema.
     *
     * @return La instancia.
     */
    static Theme &instance();

    /**
     * @brief Indica si el tema aplicado es el oscuro, sea por el modo o por el sistema.
     *
     * @return true si el tema aplicado es el oscuro.
     */
    bool isDark() const;

    /**
     * @brief Lee el modo guardado, aplica el tema y, en modo automático, lo mantiene al día con el sistema.
     */
    void load();

    /**
     * @brief Modo elegido.
     *
     * No es lo mismo que el tema aplicado: en modo automático lo da isDark().
     *
     * @return El modo elegido.
     */
    Mode mode() const { return mode_; }

    /**
     * @brief Hoja de estilos base ya expandida.
     *
     * @return La hoja de estilos, o una cadena vacía si el recurso no se puede abrir.
     */
    QString styleSheet() const;

    /**
     * @brief Valor de un token del tema aplicado, tal como está en el JSON.
     *
     * @param name Nombre del token.
     * @return El valor, o una cadena vacía si no existe.
     */
    QString token(const QString &name) const;

    /**
     * @brief Nombre del tema aplicado en los tokens y en los recursos.
     *
     * @return "dark" o "light".
     */
    QString variantName() const;

public slots:
    /**
     * @brief Cambia el modo, lo guarda y aplica el tema.
     *
     * @param mode Modo elegido.
     */
    void setMode(Mode mode);

signals:
    /**
     * @brief Se emite cada vez que se aplica el tema.
     */
    void changed();

private:
    Mode mode_ = Mode::Dark;             ///< Modo elegido.
    QHash<QString, QString> themeDark_;  ///< Tokens del tema oscuro, leídos del JSON.
    QHash<QString, QString> themeLight_; ///< Tokens del tema claro, leídos del JSON.
    QHash<QString, QString> values_;     ///< Tokens del tema aplicado, más `theme` con su nombre.

    /**
     * @brief Carga los tokens del tema. Solo se llama desde instance().
     */
    Theme();

    /**
     * @brief Aplica el tema: recalcula los valores, asigna la paleta de Qt y la hoja de estilos a la aplicación y emite changed().
     */
    void apply();

    /**
     * @brief Lee los tokens del recurso `:/teleprompter/tokens.json`; si no se puede abrir, deja un aviso en el registro.
     */
    void loadTokens();

    /**
     * @brief Recalcula los valores con los tokens del tema aplicado y su nombre.
     */
    void rebuildValues();
};

#endif
