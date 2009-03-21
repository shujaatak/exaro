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

#ifndef REPORTINTERFACE_H
#define REPORTINTERFACE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QDomDocument>
#include <QPainter>
#include <QFile>
#include <QScriptEngine>
#include <QScriptValue>
#include <QUiLoader>
#include <QSqlRecord>
#include <QSplashScreen>
#include <QSqlDatabase>


#include "pageinterface.h"

#include "bandinterface.h"
#include "globals.h"
#include "processdialog.h"

/** \namespace Report */
namespace Report
{

/** \class ReportInterface
* \brief Interface for reports
*
* This is the base class for all report(s) object(s).
*/

class PaintDevice;
class KONTAMABIL_EXPORTS ReportInterface : public QObject
{

	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QString author READ author WRITE setAuthor)
	Q_PROPERTY(QString script READ script WRITE setScript DESIGNABLE false)
	Q_PROPERTY(QVariantMap queries READ queries WRITE setQueries DESIGNABLE false)
	Q_PROPERTY(QVariantMap uis READ uis WRITE setUis DESIGNABLE false)
	Q_PROPERTY(int version READ version WRITE setVersion DESIGNABLE false)
public:
	struct FunctionValue
	{
		QScriptEngine::FunctionSignature function;
		int args;
		QScriptValue::PropertyFlags flags;
	};
	typedef QMap<QString, FunctionValue> FunctionValues;

	struct ReportValue
	{
		QVariant value;
		QScriptValue::PropertyFlags flags;
	};
	typedef QMap<QString, ReportValue> ReportValues;
public:
	/**
	 * ReportInterface constructor
	 * @param parent parent object
	 */
	ReportInterface(QObject *parent = 0);

	virtual ~ReportInterface();

	/**
	 * Exec the report. I the report is not canceled the preview dialog is shown.
	 * @return true if the report is not canceled
	 */
	virtual bool exec();


	/**
	 * Return report name
	 * @return report name
	 * @see setName()
	 */
	QString name();
	/**
	 * Set the report name.
	 * @param name report name
	 * @see name()
	 */
	void setName(const QString & name);

	/**
	 * The report author.
	 * @return report author
	 * @see setAuthor()
	 */
	QString author();
	/**
	 * Set report author
	 * @param author report author
	 * @see author()
	 */
	void setAuthor(const QString & author);

	/**
	 * Before you add a object to report call this method to see if the report can containt the object.
	 * @param object object you want to add
	 * @return true if the report can contain the object
	 */
	virtual bool canContain(QObject * object);

	/**
	 * Create the report instance.
	 * @param parent parent object
	 * @return report instance
	 */
	virtual Report::ReportInterface * createInstance(QObject * parent) = 0;

	/**
	 * Return report script
	 * @return report script
	 */
	QString script();
	/**
	 * Set the report script
	 * @param script report script
	 */
	virtual void setScript(const QString & script);

	/**
	 * Return the report queries
	 * @return queries
	 */
	QVariantMap queries();
	/**
	 * Sets the report queries
	 * @param queries report queries
	 */
	void setQueries(QVariantMap queries);

	/**
	 * Return the report uis
	 * @return
	 */
	QVariantMap uis();
	/**
	 * Sets the report uis. The uis can be used in script to interact with the user.
	 * @param uis
	 */
	void setUis(QVariantMap uis);

	/**
	 * Return the script engine
	 * @return script engine
	 */
	QScriptEngine * scriptEngine();

	/**
	 * Retrun global objects
	 * @return global objects
	 */
	const QObjectList & globalObjects();

	/**
	 * Sets the globalObjects.
	 * @param objectList
	 */
	void setGlobalObjects(const QObjectList & objectList);

	/**
	 * Adds a value to the engine.
	 * @param name value name
	 * @param value value value
	 * @param flags value flags
	 */
	void setReportGlobalValue(QString name, QVariant value, const QScriptValue::PropertyFlags & flags = QScriptValue::KeepExistingFlags );

	/**
	 * Adds a funtion to the engine.
	 * @param name function name
	 * @param function function signature
	 * @param flags function flags
	 */
	void setReportFunction( const QString & name, const QScriptEngine::FunctionSignature & function, int args=0, const QScriptValue::PropertyFlags & flags = QScriptValue::KeepExistingFlags );

	/**
	 * Sets the report database
	 * @param db
	 */
	void setDatabase(const QSqlDatabase & db);

	/**
	 * Sets the plugins paths for the uis.
	 * @param Paths
	 */
	void setUiPluginsPaths(const QStringList & uiPluginsPaths);

	/**
	 * Get the report file version
	 * @param Paths
	 */

	double version();
	/**
	 * Sets the report file version
	 * @param Paths
	 */
	void setVersion(double tVersion);


public slots:
	/**
	 * Call this slot to cancel the report execution
	 */
	void cancelReport();
//	void showProcess(QString str);

signals:
	/**
	 * This signal is triggered before report execution
	 */
	void beforeExec();

	/**
	 * This signal is triggered after report execution
	 */
	void afterExec();

	void showProcess(QString str);
protected:
//	virtual void paintPage();
//	virtual void paintBand(BandInterface * band);
	virtual void setScriptEngineGlobalVariables();
//	virtual	void paintOverlays();

protected:
//	PaintDevice * m_printer;
//
//	QPainter m_painter;
	QDomDocument m_doc;
	QDomNode m_exportNode;
	QScriptEngine * m_scriptEngine;

private:
	void setReportObjectsToScriptEngineGlobalVariables(QObject * object);
	void addOrderedBand(QList<BandInterface *> & m_bands, BandInterface * band);
//	void newPage();
//	void prepareCurrentPage();
//	bool canPaint(BandInterface * band);
	void exportRecord(const QSqlRecord & record, QDomElement & el);
//	void paintObjects(ItemInterface * item, QPointF translate, const QRectF & clipRect);
	void cleanUpObjects();
	bool isNumber(QVariant::Type type);

private slots:
	void scriptException(const QScriptValue & exception );
	void previewFinished();

private:
	bool m_reportCanceled;
	QString m_name;
	QString m_author;
	QString m_script;
	QVariantMap m_queries;
	QVariantMap m_uis;
	double m_version;
	QList<BandInterface *> m_pageHeaderBands;
	QList<BandInterface *> m_titleBands;
	QList<BandInterface *> m_detailContainerBands;
	QList<BandInterface *> m_summaryBands;
	QList<BandInterface *> m_pageFooterBands;
	QList<BandInterface *> m_pageOverlayBands;
	QObjectList m_objectList;
	FunctionValues m_functionValues;
	ReportValues m_values;
	qreal m_currentHeight, m_currentTop, m_currentBottom;
//	PageInterface * m_currentPage;
//	QSplashScreen m_splashScreen;
	QSqlDatabase m_sqlDatabase;
	QStringList m_uiPluginsPaths;

	PaintInterface * paintInterface;
	ProcessDialog * processDialog;
	QTemporaryFile * pdf_file;

	friend class PaintInterface;
};
}
Q_DECLARE_INTERFACE(Report::ReportInterface, "ro.bigendian.ReportDesigner.ReportInterface/1.0");
#endif
