/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <QFileDialog>

#include "pixmapdialog.h"

PixmapDialog::PixmapDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::Dialog()
{
	setupUi(this);
	QGraphicsScene * scene = new QGraphicsScene(this);
	scene->clear();
	graphicsView->setScene(scene);
}

void PixmapDialog::setPixmap(const QPixmap & pixmap)
{
	m_pixmap = pixmap;
	graphicsView->scene()->clear();
	graphicsView->scene()->addPixmap(pixmap);
}

QPixmap PixmapDialog::pixmap()
{
	return m_pixmap;
}

void PixmapDialog::on_clearButton_clicked()
{
	m_pixmap = QPixmap();
	graphicsView->scene()->clear();
	graphicsView->scene()->addPixmap(m_pixmap);
}

void PixmapDialog::on_chooseButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
	QImage img;
	if (img.load(fileName))
	{
		m_pixmap = QPixmap::fromImage(img);
		graphicsView->scene()->clear();
		graphicsView->scene()->addPixmap(m_pixmap);
	}
}
