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
#include <QSettings>
#include <QDesktopWidget>
#include "previewwidget.h"

namespace Report
{

PreviewWidget::PreviewWidget(QWidget *parent)
		: QGraphicsView(parent), m_zoomStep(0.25), m_zoomMax(4), m_zoomMin(0.25), m_currentZoom(1)
{
}


PreviewWidget::~PreviewWidget()
{
}

void PreviewWidget::zoomIn()
{
	if (m_currentZoom + m_zoomStep > m_zoomMax)
		m_currentZoom = m_zoomMax - m_zoomStep;
	emit(zoomChanged((int)((m_currentZoom + m_zoomStep)*100 + 0.5)));
}

void PreviewWidget::zoomOut()
{
	if (m_currentZoom - m_zoomStep < m_zoomMin)
		m_currentZoom = m_zoomMin + m_zoomStep;
	emit(zoomChanged((int)((m_currentZoom - m_zoomStep)*100 + 0.5)));
}

void PreviewWidget::zoomTo(int zoom)
{
	if (zoom > m_zoomMax*100)
		zoom = m_zoomMax * 100;

	if (zoom < m_zoomMin*100)
		zoom = m_zoomMin * 100;

	resetTransform();
	scale(((qreal)zoom / 100), ((qreal)zoom / 100));
	m_currentZoom = (qreal)zoom / 100;
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
