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

#include "ods.h"
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

inline void initMyResource()
{
	Q_INIT_RESOURCE(odsResource);
}

Ods::Ods(QObject *parent)
 : Report::ExportInterface(parent)
{
	initMyResource();
}


QString Ods::exportFormat()
{
	return tr("OASIS Spreadsheet *.ods");
};

void Ods::execExport(QDomNode & exportNode)
{
	QString fileName=QFileDialog::getSaveFileName(0,tr("Export"),QDir::homePath()+"/export.ods", tr("ODS (*.ods)"));
	if (!fileName.length())
		return;
	QDomDocument content;
	QFile file(":/content.xml");
	if (!file.open(QIODevice::ReadOnly))
		return;
	content.setContent(&file);
	file.close();

	file.setFileName(":/manifest.xml");
	if (!file.open(QIODevice::ReadOnly))
		return;
	QByteArray manifest=file.readAll();
	file.close();

	file.setFileName(":/meta.xml");
	if (!file.open(QIODevice::ReadOnly))
		return;
	QByteArray meta=file.readAll();
	file.close();

	QDomElement spreadRoot=content.documentElement().firstChildElement("office:body").firstChildElement("office:spreadsheet");
	if (spreadRoot.isNull())
		return;

	QDomElement qryEl=exportNode.firstChildElement("query");
	while(!qryEl.isNull())
	{
		QDomElement se=content.createElement("table:table");

		se.setAttribute("table:name",qryEl.attribute("name","query"));
		se.setAttribute("table:style-name","ta1");

		QDomNode rowNode=qryEl.firstChild();
		while(!rowNode.isNull())
		{
			QDomElement tr=content.createElement("table:table-row");

			QString line;
			QDomElement field=rowNode.firstChildElement();
			while(!field.isNull())
			{
				QDomElement tf=content.createElement("table:table-cell");
				if (field.attribute("type","string")=="float")
				{
					tf.setAttribute("office:value-type", "float");
					tf.setAttribute("office:value", field.text());
				}
				else
					tf.setAttribute("office:value-type", "string");

				QDomElement tfp=content.createElement("text:p");
				tfp.appendChild(content.createTextNode(field.text()));
				tf.appendChild(tfp);
				tr.appendChild(tf);
				field=field.nextSiblingElement();
			}
			se.appendChild(tr);
			rowNode=rowNode.nextSibling();
		}
		spreadRoot.appendChild(se);
		qryEl=qryEl.nextSiblingElement();
	}

	QuaZip zip(fileName);
	if(!zip.open(QuaZip::mdCreate))
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't create %1 error no: %2").arg(fileName).arg(zip.getZipError()));
		return;
  	}
	QuaZipFile outFile(&zip);
	if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo("meta.xml")))
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't create meta.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.write(meta);
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't write data to meta.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.close();
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't close meta.xml, error no: %1").arg(zip.getZipError()));
		return;
	}

	if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo("META-INF/manifest.xml")))
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't create META-INF/manifest.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.write(manifest);
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't write data to META-INF/manifest.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.close();
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't close META-INF/manifest.xml, error no: %1").arg(zip.getZipError()));
		return;
	}

	if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo("content.xml")))
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't create content.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.write(content.toByteArray(2));
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't write data to content.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	outFile.close();
    	if(outFile.getZipError()!=UNZ_OK)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't close content.xml, error no: %1").arg(zip.getZipError()));
		return;
	}
	zip.close();
	if(zip.getZipError()!=0)
	{
		QMessageBox::critical(0,tr("ODS Export"), tr("Can't close %1 error no: %2").arg(fileName).arg(zip.getZipError()));
		return;
	}

	QMessageBox::information(0,tr("Export"),tr("Export done"),QMessageBox::Ok);
}

Ods::~Ods()
{
}

Q_EXPORT_PLUGIN2(ods, Ods)
