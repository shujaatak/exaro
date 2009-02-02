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

#include "csvtxt.h"
#include "exportsettingsdialog.h"
#include <QFileDialog>
#include <QMessageBox>

CsvTxt::CsvTxt(QObject *parent)
 : ExportInterface(parent)
{
}

QString CsvTxt::exportFormat()
{
	return tr("Csv file *.csv");
};

QString CsvTxt::parseString(QString string, QString quote)
{
	return quote+string.replace(quote,quote+quote)+quote;
}

void CsvTxt::execExport(QDomNode & exportNode)
{
	ExportSettingsDialog ed;
	if (ed.exec()==QDialog::Rejected)
		return;

	QString dirName=QFileDialog::getExistingDirectory(0,tr("Folder to export to"),QDir::homePath());
	if (!dirName.length())
		return;

	QDomElement qryEl=exportNode.firstChildElement("query");

	QString delimiter;
	QString quote;

	if (ed.other->isChecked())
		delimiter=ed.delimiter->text();

	if (ed.comma->isChecked())
		delimiter=",";

	if (ed.semicolon->isChecked())
		delimiter=";";

	if (ed.space->isChecked())
		delimiter=" ";

	if (ed.tabulator->isChecked())
		delimiter=QString(1,0x09);

	if (ed.quote_3->isChecked())
		quote=ed.quote->text();

	if (ed.quote_1->isChecked())
		quote="\"";

	if (ed.quote_2->isChecked())
		quote="'";

	while(!qryEl.isNull())
	{
		QFile file(dirName+qryEl.attribute("name","query")+".csv");
		if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		{
			qryEl=qryEl.nextSiblingElement();
			continue;
		}
 		QTextStream out(&file);

		QDomNode rowNode=qryEl.firstChild();
		while(!rowNode.isNull())
		{
			QString line;
			QDomElement field=rowNode.firstChildElement();
			while(!field.isNull())
			{
				if (field.attribute("type","string")=="string")
					line+=parseString(field.text(),quote);
				else
					line+=field.text();
				line+=delimiter;
				field=field.nextSiblingElement();
			}
			line.remove(line.length()-1,1);
			out<<line<<"\n";
			rowNode=rowNode.nextSibling();
		}
		file.close();
		qryEl=qryEl.nextSiblingElement();
	}
	QMessageBox::information(0,tr("Export"),tr("Export done"),QMessageBox::Ok);
}

CsvTxt::~CsvTxt()
{
}

Q_EXPORT_PLUGIN2(csvTxt, CsvTxt)
