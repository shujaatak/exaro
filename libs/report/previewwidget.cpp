/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        * 
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      * 
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QSettings>
#include <QDesktopWidget>
#include "previewwidget.h"

namespace Report
{

#define screen_widthMM (((double)QDesktopWidget().screen()->width() /(double)QDesktopWidget().screen()->physicalDpiX() )*25.4)

PreviewWidget::PreviewWidget(QWidget *parent)
		: QGraphicsView(parent)
{
	m_currentZoom=(double)QDesktopWidget().screen()->width()/(screen_widthMM*10);
	m_zoomMin=m_currentZoom/4;
	m_zoomMax=m_currentZoom*4;
	m_zoomStep=m_currentZoom/4;
}


PreviewWidget::~PreviewWidget()
{
}

void PreviewWidget::zoomIn()
{
	m_currentZoom += m_zoomStep;
	if (m_currentZoom > m_zoomMax)
		m_currentZoom = m_zoomMax;
	emit(zoomChanged((int)((m_currentZoom/((double)QDesktopWidget().screen()->width()/(screen_widthMM*10)))*100 + 0.5)));
}

void PreviewWidget::zoomOut()
{
	m_currentZoom -= m_zoomStep;
	if (m_currentZoom < m_zoomMin)
		m_currentZoom = m_zoomMin;
	emit(zoomChanged((int)((m_currentZoom/((double)QDesktopWidget().screen()->width()/(screen_widthMM*10)))*100 + 0.5)));
}

void PreviewWidget::zoomTo(int zoom)
{
	qreal z=((double)zoom/100)*((double)QDesktopWidget().screen()->width()/(screen_widthMM*10));
	if (z > m_zoomMax)
		z = m_zoomMax;

	if (z < m_zoomMin)
		z = m_zoomMin;

	resetTransform();
	scale(z, z);
	m_currentZoom = z;
}

void PreviewWidget::setZoomStep(qreal step)
{
	m_zoomStep = step;
}

void PreviewWidget::setZoomMax(qreal max)
{
	m_zoomMax = max;
}

void PreviewWidget::setZoomMin(qreal min)
{
	m_zoomMin = min;
}

}
