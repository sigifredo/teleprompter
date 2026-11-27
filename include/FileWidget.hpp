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

#ifndef FILEWIDGET_HPP
#define FILEWIDGET_HPP

// qt
#include <QWidget>

class QLabel;

/**
 * @brief Fila con el nombre del archivo abierto y un botón para elegir otro.
 *
 * El filtro del diálogo se recibe al construirla, así que la misma clase sirve para el guion
 * y para las diapositivas.
 */
class FileWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @param buttonText Texto del botón; admite un mnemónico con `&`.
     * @param filter Filtro del diálogo de archivos, por ejemplo "Script (*.md *.txt)".
     */
    FileWidget(const QString &buttonText, const QString &filter, QWidget *parent = nullptr);

    /**
     * @brief Muestra el nombre del archivo abierto; el nombre completo queda en la ayuda emergente.
     */
    void setFileName(const QString &fileName);

signals:
    /**
     * @brief Se emite con la ruta que el usuario eligió.
     */
    void fileSelected(const QString &filePath);

public slots:
    /**
     * @brief Abre el diálogo para elegir un archivo.
     */
    void chooseFile();

private:
    QLabel *fileNameLabel_;
    QString filter_;
};

#endif
