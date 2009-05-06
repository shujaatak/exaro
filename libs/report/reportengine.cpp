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

#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QKeySequence>
#include <QMetaProperty>
#include <QBuffer>

#include <limits.h>

#include "reportengine.h"
#include "globals.h"

#ifndef LIB_INSTALL_DIR
    #define LIB_INSTALL_DIR "../lib"
#endif

namespace Report
{

ReportEngine::ReportEngine(QObject *parent)
		: QObject(parent)
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS")
	{
		pluginsDir.cdUp();
		pluginsDir.cd("PlugIns");
	}
#else
	pluginsDir.cd(LIB_INSTALL_DIR);
#endif
	pluginsDir.cd("report");
	QStringList paths=qApp->libraryPaths();
	paths<<pluginsDir.absolutePath();
	qApp->setLibraryPaths(paths);
	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader;
		loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
		loader.setFileName(pluginsDir.absoluteFilePath(fileName));
		if (!loader.load())
		{
			qCritical() << loader.errorString();
			continue;
		}
		QObject *plugin = loader.instance();

		if (plugin)
		{
			if (dynamic_cast<Report::ItemInterface*>(plugin))
				m_items.insertMulti((dynamic_cast<Report::ItemInterface*>(plugin))->toolBoxGroup(), dynamic_cast<Report::ItemInterface*>(plugin));

			if (dynamic_cast<Report::PageInterface*>(plugin))
				m_pages.push_back(dynamic_cast<Report::PageInterface*>(plugin));

			if (dynamic_cast<Report::ReportInterface*>(plugin))
				m_reports.push_back(dynamic_cast<Report::ReportInterface*>(plugin));
		}
		else
			qCritical() << plugin << loader.errorString();
	}
}

bool ReportEngine::saveReport(const QString & fileName, ReportInterface * report)
{
	QFile file(fileName);

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QDomDocument doc("report");
		doc.appendChild(objectProperties(report, &doc));
		file.write(doc.toByteArray(4));
		file.close();
		return true;
	}

	return false;
}

ReportInterface* ReportEngine::loadReport(const QString & fileName)
{
	ReportInterface* report = 0;
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly))
	{
		report = dynamic_cast<ReportInterface*>(loadReport(&file));
		file.close();
	}

	return report;
}

ReportInterface* ReportEngine::loadReport(QIODevice * device)
{
	QDomDocument doc("report");

	if (!doc.setContent(device->readAll()))
		return 0;

	return dynamic_cast<Report::ReportInterface*>(objectFromDom(0, doc.firstChildElement()));
}


ReportInterface* ReportEngine::loadReportFromString(const QString & content)
{
	QDomDocument doc("report");

	if (!doc.setContent(content))
		return 0;

	return dynamic_cast<Report::ReportInterface*>(objectFromDom(0, doc.firstChildElement()));
}

const QMap<QString, ItemInterface*> & ReportEngine::items() const
{
	return m_items;
}

ItemInterface * ReportEngine::findItemByClassName(const QString & name)
{
    foreach (ItemInterface * item, m_items)
    {
	if (item->metaObject()->className() == name)
	    return item;
    }
    return 0;
}


const QList<PageInterface*>  & ReportEngine::pages() const
{
	return m_pages;
}

const QList<ReportInterface*> & ReportEngine::reports() const
{
	return m_reports;
}

ReportEngine::~ReportEngine()
{
}



bool ReportEngine::isUniqueName(QObject * object, QString name)
{
	if (!object)
		return true;

	if (object->objectName() == name)
		return false;

	for (int i = 0;i < object->children().size();i++)
		if (!(isUniqueName(object->children()[i], name)))
			return false;

	return true;
}

QString ReportEngine::uniqueName(const QString & className, QObject * rootObject)
{
	QString name = className.toLower();

	for (int i = 0;i < name.size();i++)
		if (name[i] < 'a' && name[i] > 'z' && name[i] < '0' && name[i] > '9')
			name[i] = '_';

	if (isUniqueName(rootObject, name))
		return name;

	int number = 1;

	forever
	{
		if (isUniqueName(rootObject, name + "_" + QString::number(number, 10)))
			return name + "_" + QString::number(number, 10);

		number++;
	}
}

bool ReportEngine::cmpBands(BandInterface * b1, BandInterface * b2)
{
	if (b1->bandType()==b2->bandType())
		return b1->order()<b2->order();

	return b1->bandType()<b2->bandType();
}

QDomElement ReportEngine::objectProperties(QObject * object, QDomDocument * doc)
{
	if (!dynamic_cast<ReportInterface*>(object) && !dynamic_cast<PageInterface*>(object) && !dynamic_cast<ItemInterface*>(object))
		return QDomElement();

	QDomElement d = doc->createElement(object->metaObject()->className());

	if (object->metaObject()->propertyCount())
	{
		QDomElement pro = doc->createElement("properties");

		for (int p = 0;p < object->metaObject()->propertyCount();p++)
			pro.appendChild(variatToDom(doc, object->metaObject()->property(p).name(), object->property(object->metaObject()->property(p).name())));

		d.appendChild(pro);
	}

	QList<BandInterface*> bands;

	foreach(QObject * child, object->children())
		if (dynamic_cast<BandInterface*>(child))
			bands.push_back(dynamic_cast<BandInterface*>(child));

	if (bands.size())
		qSort(bands.begin(), bands.end(), cmpBands);

	if (object->children().size())
	{
		QDomElement ch = doc->createElement("children");

		for (int b=0;b<bands.size();b++)
			ch.appendChild(objectProperties(bands[b], doc));

		for (int c = 0; c < object->children().size();c++)
		{
			bool isBand=false;

			for (int b=0;b<bands.size();b++)
				if (bands[b]==object->children()[c])
				{
					isBand=true;
					break;
				}

			if (!isBand)
				ch.appendChild(objectProperties(object->children()[c], doc));
		}

		d.appendChild(ch);
	}

	return d;
}

void ReportEngine::setObjectPropertiesFromDom(QObject * object, const QDomElement & dom)
{
	for (QDomElement prop = dom.firstChildElement("properties").firstChild().toElement();!prop.isNull();prop = prop.nextSibling().toElement())
		object->setProperty(prop.tagName().toAscii(), domToVariant(prop));
}


QObject * ReportEngine::objectFromDom(QObject * parent, const QDomElement & dom)
{
	QObject * obj = 0;

	for (int r = 0;r < m_reports.size();r++)
		if (dom.tagName() == m_reports[r]->metaObject()->className())
		{
			obj = m_reports[r]->createInstance(parent);
			setObjectPropertiesFromDom(obj, dom);
			break;
		}

	if (!obj)
		for (int p = 0;p < m_pages.size();p++)
			if (dom.tagName() == m_pages[p]->metaObject()->className())
			{
				obj = m_pages[p]->createInstance(parent);
				setObjectPropertiesFromDom(obj, dom);
				break;
			}

	if (!obj)
		for (int i = 0;i < m_items.size();i++)
			if (dom.tagName() == m_items.values()[i]->metaObject()->className())
			{
				obj = m_items.values()[i]->createInstance(dynamic_cast<QGraphicsItem*>(parent), parent);

				if (dynamic_cast<Report::PageInterface*>(parent))
					dynamic_cast<Report::PageInterface*>(parent)->addItem(dynamic_cast<QGraphicsItem*>(obj));

				setObjectPropertiesFromDom(obj, dom);

				break;
			}

	if (obj)
		for (QDomElement ch = dom.firstChildElement("children").firstChildElement();!ch.isNull();ch = ch.nextSiblingElement())
			objectFromDom(obj, ch);
	else
		qWarning() << tr("Can't find %1").arg(dom.tagName());

	return obj;
}

void ReportEngine::copy(QObject* object)
{
	QDomDocument doc;
	doc.appendChild(objectProperties(object, &doc));
	lastObject = doc.toString(0);
}

QObject * ReportEngine::paste(QObject* parent)
{
	QDomDocument doc;
	doc.setContent(lastObject);
	QObject * obj = objectFromDom(parent, doc.firstChildElement());

	if (dynamic_cast<Report::BandInterface*>(obj))
	{
		dynamic_cast<Report::BandInterface*>(obj)->setOrder(INT_MAX);
		dynamic_cast<Report::BandInterface*>(obj)->setGeometry(QRectF(0, 0, dynamic_cast<Report::BandInterface*>(obj)->geometry().width(), dynamic_cast<Report::BandInterface*>(obj)->geometry().height()));
	}

	return obj;
}

}
