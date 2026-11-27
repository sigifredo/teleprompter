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

#ifndef BIONICWIDGET_HPP
#define BIONICWIDGET_HPP

// qt
#include <QTextBlock>
#include <QTextEdit>

/**
 * @brief Visor del guion con lectura biónica, columna centrada y franja de lectura.
 *
 * La lectura biónica pone en negrita la primera mitad de cada palabra. El visor conserva el texto
 * original, así que se puede activar o desactivar sin volver a abrir el archivo.
 *
 * Desplazarse con scrollStep() mueve exactamente una línea visual y la deja en la franja de
 * lectura, tenga la altura que tenga. Para que la primera y la última línea también lleguen a la
 * franja, el documento lleva espacio arriba y abajo.
 */
class BionicWidget : public QTextEdit
{
    Q_OBJECT

public:
    /// Ancho de la columna de lectura, en caracteres: valor inicial y límites.
    static constexpr int DefaultColumnChars = 62;
    static constexpr int MinColumnChars = 40;
    static constexpr int MaxColumnChars = 100;

    /// Posición de la franja de lectura, como porcentaje del alto del visor: valor inicial y límites.
    static constexpr int DefaultBandPercent = 30;
    static constexpr int MinBandPercent = 15;
    static constexpr int MaxBandPercent = 60;

    explicit BionicWidget(QWidget *parent = nullptr);

    bool isBionicEnabled() const;
    bool isReadingBandVisible() const;
    int columnChars() const;
    int readingBandPosition() const;

public slots:
    /**
     * @brief Muestra un texto plano; los saltos de línea separan párrafos.
     */
    void setBionicText(const QString &text);

    /**
     * @brief Muestra un texto en Markdown, conservando encabezados, listas y énfasis.
     */
    void setBionicMarkdown(const QString &text);

    /**
     * @brief Mueve el guion una línea visual (dirección positiva baja, negativa sube).
     */
    void scrollStep(int direction);

    /**
     * @brief Mueve el guion una página, dejando algo de solapamiento.
     */
    void scrollPage(int direction);

    /**
     * @brief Lleva el guion al final, o al principio si `end` es false.
     */
    void scrollToEdge(bool end);

    void setFontSize(int fontSize);
    void setBionicEnabled(bool enabled);
    void setReadingBandVisible(bool visible);
    void setReadingBandPosition(int percent);
    void setColumnChars(int chars);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QRect bandRect() const;
    int lineHalfHeight(const QTextBlock &block, bool last) const;
    void render();
    void renderMarkdown();
    void renderText();
    void updateColumn();
    void updatePadding();

    bool bionic_ = true;
    bool bandVisible_ = false;
    bool loaded_ = false;
    bool markdown_ = false;
    int bandPercent_ = DefaultBandPercent;
    int columnChars_ = DefaultColumnChars;
    QString source_; ///< Texto original, sin la lectura biónica aplicada.
};

#endif
