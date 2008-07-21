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

#include <QPrinter>
#include <QPalette>
#include <QGraphicsView>
#include "designerpage.h"

DesignerPage::DesignerPage(QDomNode *pageNode, QObject *parent)
		: QGraphicsScene(parent)
{
	setNode(pageNode);
}

DesignerPage::~DesignerPage()
{
}

void DesignerPage::setNode(QDomNode *pageNode)
{
	m_pageNode = pageNode;
	m_unsavedPageNode = QDomNode();

	if (!m_pageNode)
		return;

	m_unsavedPageNode = m_pageNode->cloneNode();

	QPrinter a;
	a.setPageSize(QPrinter::A4);
	setSceneRect(a.paperRect());
}

QGraphicsView * DesignerPage::newDesignerPage(QDomNode *pageNode)
{

	QGraphicsView * gw = new QGraphicsView(new DesignerPage(pageNode));
	gw->setAutoFillBackground(true);
	gw->setBackgroundRole(QPalette::Base);
	QPalette pal = gw->palette();
	pal.setBrush(QPalette::Base, QPixmap(":/images/background.png"));
	pal.setColor(QPalette::HighlightedText, Qt::red);
	gw->setPalette(pal);
	return gw;
}
