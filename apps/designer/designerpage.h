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

#ifndef DESIGNERPAGE_H
#define DESIGNERPAGE_H

#include <QGraphicsScene>
#include <QDomNode>
#include "iteminterface.h"

class DesignerPage : /*public QObject, */public QGraphicsScene
{
	Q_OBJECT

private:
	QDomNode * m_pageNode;
	QDomNode m_unsavedPageNode;

public:
	DesignerPage(QDomNode *pageNode, QObject *parent = 0);
	~DesignerPage();
	void setNode(QDomNode *pageNode);

public:
	static QGraphicsView * newDesignerPage(QDomNode *pageNode);
};

#endif
