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
 * @file BionicWidget.hpp
 * @brief Visor del guion con lectura biónica.
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
    static constexpr int DefaultBandPercent = 30; ///< Posición inicial de la franja de lectura, como porcentaje del alto del visor.
    static constexpr int DefaultColumnChars = 62; ///< Ancho inicial de la columna de lectura, en caracteres.
    static constexpr int MaxBandPercent = 60;     ///< Posición más baja de la franja, como porcentaje del alto del visor.
    static constexpr int MaxColumnChars = 100;    ///< Ancho máximo de la columna de lectura, en caracteres.
    static constexpr int MinBandPercent = 15;     ///< Posición más alta de la franja, como porcentaje del alto del visor.
    static constexpr int MinColumnChars = 40;     ///< Ancho mínimo de la columna de lectura, en caracteres.

    /**
     * @brief Crea un visor vacío y de solo lectura, que no acepta archivos arrastrados.
     *
     * @param parent Widget padre.
     */
    explicit BionicWidget(QWidget *parent = nullptr);

    /**
     * @brief Ancho de la columna de lectura.
     *
     * @return El ancho, en caracteres.
     */
    int columnChars() const;

    /**
     * @brief Indica si la lectura biónica está activada.
     *
     * @return true si está activada.
     */
    bool isBionicEnabled() const;

    /**
     * @brief Indica si la franja de lectura está visible.
     *
     * @return true si está visible.
     */
    bool isReadingBandVisible() const;

    /**
     * @brief Posición de la franja de lectura.
     *
     * @return El porcentaje del alto del visor donde empieza la franja.
     */
    int readingBandPosition() const;

public slots:
    /**
     * @brief Mueve el guion una página: el 90 % del alto visible, para que quede algo de solapamiento.
     *
     * @param direction Positiva baja y negativa sube.
     */
    void scrollPage(int direction);

    /**
     * @brief Mueve el guion una línea visual y la deja en la franja de lectura.
     *
     * No hace nada si no hay más líneas en esa dirección.
     *
     * @param direction Positiva baja y negativa sube.
     */
    void scrollStep(int direction);

    /**
     * @brief Lleva el guion a un extremo.
     *
     * @param end true lo lleva al final y false, al principio.
     */
    void scrollToEdge(bool end);

    /**
     * @brief Activa o desactiva la lectura biónica, conservando la posición de lectura.
     *
     * @param enabled true la activa.
     */
    void setBionicEnabled(bool enabled);

    /**
     * @brief Muestra un texto en Markdown, conservando encabezados, listas y énfasis.
     *
     * @param text Texto en Markdown.
     */
    void setBionicMarkdown(const QString &text);

    /**
     * @brief Muestra un texto plano; los saltos de línea separan párrafos.
     *
     * @param text Texto plano. Se muestra tal cual, sin interpretar HTML.
     */
    void setBionicText(const QString &text);

    /**
     * @brief Fija el ancho de la columna de lectura.
     *
     * @param chars Ancho en caracteres; se limita al rango de MinColumnChars a MaxColumnChars.
     */
    void setColumnChars(int chars);

    /**
     * @brief Fija el tamaño de la letra del guion.
     *
     * @param fontSize Tamaño en puntos.
     */
    void setFontSize(int fontSize);

    /**
     * @brief Fija la posición de la franja de lectura.
     *
     * @param percent Porcentaje del alto del visor; se limita al rango de MinBandPercent a MaxBandPercent.
     */
    void setReadingBandPosition(int percent);

    /**
     * @brief Muestra u oculta la franja de lectura.
     *
     * Aunque esté oculta, scrollStep() sigue dejando cada línea en su posición.
     *
     * @param visible true la muestra.
     */
    void setReadingBandVisible(bool visible);

protected:
    /**
     * @brief Dibuja el texto y, si la franja está visible, un relleno suave con una barra de acento en el borde izquierdo.
     *
     * @param event Evento de pintado.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Recalcula la columna de lectura y el espacio sobre y bajo el texto.
     *
     * @param event Evento de cambio de tamaño.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    int bandPercent_ = DefaultBandPercent; ///< Posición de la franja, como porcentaje del alto del visor.
    bool bandVisible_ = false;             ///< La franja de lectura está visible.
    bool bionic_ = true;                   ///< La lectura biónica está activada.
    int columnChars_ = DefaultColumnChars; ///< Ancho de la columna de lectura, en caracteres.
    bool loaded_ = false;                  ///< Ya se cargó un texto.
    bool markdown_ = false;                ///< El texto cargado es Markdown y no texto plano.
    QString source_;                       ///< Texto original, sin la lectura biónica aplicada.

    /**
     * @brief Rectángulo de la franja de lectura.
     *
     * @return El rectángulo, en coordenadas del área visible.
     */
    QRect bandRect() const;

    /**
     * @brief Mitad de la altura de una línea de un bloque de texto.
     *
     * @param block Bloque del documento.
     * @param last true para la última línea del bloque y false para la primera.
     * @return La mitad de la altura de esa línea, en píxeles.
     */
    int lineHalfHeight(const QTextBlock &block, bool last) const;

    /**
     * @brief Muestra el texto cargado con las opciones actuales; no hace nada si aún no se cargó texto.
     */
    void render();

    /**
     * @brief Muestra el texto como Markdown y, con la lectura biónica activada, pone en negrita la primera mitad de cada palabra.
     */
    void renderMarkdown();

    /**
     * @brief Muestra el texto como párrafos planos y, con la lectura biónica activada, pone en negrita la primera mitad de cada palabra.
     */
    void renderText();

    /**
     * @brief Ajusta los márgenes laterales para centrar la columna de lectura, con un mínimo de 16 píxeles a cada lado.
     */
    void updateColumn();

    /**
     * @brief Ajusta el espacio sobre y bajo el documento para que la primera y la última línea lleguen a la franja.
     */
    void updatePadding();
};

#endif
