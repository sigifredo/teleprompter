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
 * @file SlideDeck.hpp
 * @brief Presentación en PDF y su diapositiva actual.
 */

#ifndef SLIDEDECK_HPP
#define SLIDEDECK_HPP

// qt
#include <QImage>
#include <QObject>
#include <QPdfDocument>

/**
 * @brief Presentación en PDF y diapositiva actual.
 *
 * Es la única fuente de verdad del índice: el panel del presentador y la ventana de
 * presentación leen de aquí y reaccionan a currentChanged().
 */
class SlideDeck : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Crea una presentación vacía.
     *
     * @param parent Objeto padre.
     */
    explicit SlideDeck(QObject *parent = nullptr);

    /**
     * @brief Número de diapositivas.
     *
     * @return El número de páginas del PDF, o 0 si no hay ninguno abierto.
     */
    int count() const;

    /**
     * @brief Índice de la diapositiva actual, desde 0.
     *
     * @return El índice de la diapositiva actual.
     */
    int currentIndex() const;

    /**
     * @brief Abre un PDF y vuelve a la primera diapositiva, emitiendo currentChanged().
     *
     * @param path Ruta del PDF.
     * @return false si no se pudo abrir o no tiene páginas; en ese caso deja un aviso en el registro.
     */
    bool load(const QString &path);

    /**
     * @brief Dibuja una diapositiva dentro de un tamaño, conservando la proporción de la página.
     *
     * @param index Índice de la diapositiva, desde 0.
     * @param size Tamaño máximo de la imagen, en píxeles.
     * @return La imagen, o una imagen nula si el índice o el tamaño no son válidos.
     */
    QImage render(int index, const QSize &size) const;

public slots:
    /**
     * @brief Pasa a la diapositiva siguiente; no hace nada en la última.
     */
    void next();

    /**
     * @brief Pasa a la diapositiva anterior; no hace nada en la primera.
     */
    void previous();

    /**
     * @brief Cambia la diapositiva actual; ignora un índice fuera de rango o el mismo.
     *
     * @param index Índice de la diapositiva, desde 0.
     */
    void setCurrent(int index);

signals:
    /**
     * @brief Se emite cuando cambia la diapositiva actual y al abrir un PDF.
     *
     * @param index Índice de la diapositiva actual.
     */
    void currentChanged(int index);

private:
    int current_ = 0;        ///< Índice de la diapositiva actual.
    QPdfDocument *document_; ///< Documento PDF.
};

#endif
