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

#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>

#include "html.h"

Html::Html(QObject *parent)
 : Report::ExportInterface(parent)
{
}


Html::~Html()
{
}


QString Html::exportFormat()
{
	return tr("Html *.html");
};

void Html::execExport(QDomNode & exportNode)
{
	QString fileName=QFileDialog::getSaveFileName(0,tr("Export"),QDir::homePath()+"/export.html", tr("HTML (*.html)"));
	if (!fileName.length())
		return;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return;

	QDomDocument content;
	QDomNode spreadRoot=content.createElement("html").appendChild(content.createElement("body"));
	QDomElement header=content.createElement("div");

	header.setAttribute("id","header");
	spreadRoot.appendChild(header);
	content.appendChild(spreadRoot);

	QDomElement qryEl=exportNode.firstChildElement("query");
	while(!qryEl.isNull())
	{
		QDomElement h2=content.createElement("h2");
		h2.setAttribute("id",qryEl.attribute("name","query"));
		h2.appendChild(content.createTextNode(qryEl.attribute("name","query")));
		QDomElement se=content.createElement("table");
		QDomElement aref=content.createElement("a");
		aref.setAttribute("href",QString("#")+qryEl.attribute("name","query"));
		aref.appendChild(content.createTextNode(QString(" <")+qryEl.attribute("name","query")+QString("> ")));
		header.appendChild(aref);
		spreadRoot.appendChild(h2);
		spreadRoot.appendChild(se);

		QDomNode rowNode=qryEl.firstChild();
		while(!rowNode.isNull())
		{
			QDomElement tr=content.createElement("tr");

			QString line;
			QDomElement field=rowNode.firstChildElement();
			while(!field.isNull())
			{
				QDomElement tf=content.createElement("td");
				if (field.attribute("type","string")=="float")
					tf.setAttribute("style", "border:1.0px solid #000000;text-align:right");
				else
					tf.setAttribute("style", "border:1.0px solid #000000;text-align:left");
				tf.appendChild(content.createTextNode(field.text()));
				tr.appendChild(tf);
				field=field.nextSiblingElement();
			}
			se.appendChild(tr);
			rowNode=rowNode.nextSibling();
		}
		qryEl=qryEl.nextSiblingElement();
	}
	file.write(content.toString(4).toAscii());
	file.close();
	QMessageBox::information(0,tr("Export"),tr("Export done"),QMessageBox::Ok);
}

Q_EXPORT_PLUGIN2(html, Html)
