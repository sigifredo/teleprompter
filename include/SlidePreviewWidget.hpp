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

#ifndef SLIDEPREVIEWWIDGET_HPP
#define SLIDEPREVIEWWIDGET_HPP

// Qt
#include <QImage>
#include <QWidget>

class SlideDeck;

/**
 * Shows the slide at a fixed offset from the current one (0 = current, 1 = next).
 */
class SlidePreviewWidget : public QWidget
{
    Q_OBJECT

public:
    SlidePreviewWidget(SlideDeck *pDeck, int iOffset, const QString &sCaption, QWidget *pParent = nullptr);

    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *pEvent) override;
    void resizeEvent(QResizeEvent *pEvent) override;

private:
    SlideDeck *_pDeck;
    int _iOffset;
    QString _sCaption;
    QImage _image;

    QRect slideArea() const;
    void updateImage();

private slots:
    void deckCurrentChanged(int iIndex);
};

#endif
