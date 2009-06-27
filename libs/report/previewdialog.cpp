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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QLabel>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPrintDialog>
#include <QPrinter>
#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QSplashScreen>
#include <QSettings>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QDebug>

#include "document.h"
#include "page.h"
#include "pagegraphicsitem.h"

#include "previewdialog.h"
#include "previewwidget.h"
#include "searchwidget.h"

#ifndef LIB_INSTALL_DIR
    #define LIB_INSTALL_DIR "../lib"
#endif

inline void initMyResource()
{
	Q_INIT_RESOURCE(preview);
}

namespace Report
{

PreviewDialog::PreviewDialog(QWidget *parent)
		: QDialog(parent), m_previewWidget(0), m_doc(0), m_searchPage(0),m_spaceBetweenPages(50)
{
	initMyResource();


	m_showPrintDialog = true;
	m_showExitConfirm = true;

	setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
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
	pluginsDir.cd("report/export");

	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		loader.setLoadHints(QLibrary::ResolveAllSymbolsHint|QLibrary::ExportExternalSymbolsHint);
		QObject *plugin = loader.instance();
		if (plugin)
		{
			if (dynamic_cast<ExportInterface*>(plugin))
				m_exports.push_back(dynamic_cast<ExportInterface*>(plugin));
		}
		else
			qCritical() << plugin << loader.errorString();
	}


	m_searchWidget = new SearchWidget(this);
	m_searchWidget->setIconSize(QSize(32, 32));
	m_searchWidget->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	m_searchWidget->hide();

	QToolBar * toolbar = new QToolBar(this);
	toolbar->setIconSize(QSize(32, 32));
	toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	toolbar->setFloatable( false );

	m_previewWidget = new PreviewWidget(this);


	QAction * act = toolbar->addAction(QIcon(":/images/zoom-in.png"), tr("Zoom in"));
	act->setShortcut(QKeySequence(QKeySequence::ZoomIn));
	connect(act, SIGNAL(triggered(bool)), m_previewWidget, SLOT(zoomIn()));

	m_zoomSpinBox = new QSpinBox;
	toolbar->addWidget(m_zoomSpinBox);
	m_zoomSpinBox->setSuffix(" %");
	m_zoomSpinBox->setMinimum(25);
	m_zoomSpinBox->setMaximum(400);
	m_zoomSpinBox->setSingleStep(5);
	m_zoomSpinBox->setValue(100);

	connect(m_previewWidget, SIGNAL(zoomChanged(int)), m_zoomSpinBox, SLOT(setValue(int)));
	connect(m_zoomSpinBox, SIGNAL(valueChanged(int)), m_previewWidget, SLOT(zoomTo(int)));

	act = toolbar->addAction(QIcon(":/images/zoom-out.png"), tr("Zoom out"));
	act->setShortcut(QKeySequence(QKeySequence::ZoomOut));
	connect(act, SIGNAL(triggered(bool)), m_previewWidget, SLOT(zoomOut()));

	toolbar->addSeparator();

	act = toolbar->addAction(QIcon(":/images/go-first.png"), tr("First page"));
	act->setShortcut(QKeySequence(Qt::Key_Home));
	connect(act, SIGNAL(triggered(bool)), SLOT(firstPage()));

	act = toolbar->addAction(QIcon(":/images/go-previous.png"), tr("Read previous"));
	act->setShortcut(QKeySequence(Qt::Key_PageUp));
	connect(act, SIGNAL(triggered(bool)), SLOT(readPrevious()));

	act = toolbar->addAction(QIcon(":/images/go-next.png"), tr("Read next"));
	act->setShortcut(QKeySequence(Qt::Key_PageDown));
	connect(act, SIGNAL(triggered(bool)), SLOT(readNext()));

	act = toolbar->addAction(QIcon(":/images/go-last.png"), tr("Last page"));
	act->setShortcut(QKeySequence(Qt::Key_End));
	connect(act, SIGNAL(triggered(bool)), SLOT(lastPage()));

	toolbar->addSeparator();

	act = toolbar->addAction(QIcon(":/images/export.png"), tr("Export"));
	act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	connect(act, SIGNAL(triggered(bool)), SLOT(exportDocument()));

	toolbar->addSeparator();

	act = toolbar->addAction(QIcon(":/images/edit-find.png"), tr("Search"));
	act->setShortcut(QKeySequence(QKeySequence::Find));
	connect(act, SIGNAL(triggered(bool)), m_searchWidget, SLOT(show()));
	connect(m_searchWidget, SIGNAL(searchNext(const QString&)), SLOT(searchNext(const QString&)));
	connect(m_searchWidget, SIGNAL(searchPrevious(const QString&)), SLOT(searchPrevious(const QString&)));
	connect(this, SIGNAL(textNotFound()), m_searchWidget, SLOT(notFound()));
	connect(m_searchWidget, SIGNAL(closed()), SLOT(clearSelection()));
	connect(m_searchWidget, SIGNAL(closed()), m_previewWidget, SLOT(setFocus()));

	toolbar->addSeparator();

	act = toolbar->addAction(QIcon(":/images/print.png"), tr("Print"));
	act->setShortcut(QKeySequence(QKeySequence::Print));
	connect(act, SIGNAL(triggered(bool)), SLOT(print()));

	toolbar->addSeparator();
	act = toolbar->addAction(QIcon(":/images/quit.png"), tr("Quit"));
	act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
	connect(act, SIGNAL(triggered(bool)), SLOT(reject()));

	QVBoxLayout * vlayout = new QVBoxLayout;
	vlayout->addWidget(toolbar);
	vlayout->addWidget(m_previewWidget);
	vlayout->addWidget(m_searchWidget);
	setLayout(vlayout);
}

PreviewDialog::~PreviewDialog()
{
	foreach(pageStruct pag, m_pages)
		delete pag.page;
	delete m_doc;
}

void PreviewDialog::exportDocument()
{
	QDialog d;
	QVBoxLayout * vlayout = new QVBoxLayout;
	QListWidget * list = new QListWidget(&d);

	foreach(ExportInterface* ei, m_exports)
		list->addItem(ei->exportFormat());

	if (list->count())
		list->item(0)->setSelected(true);

	vlayout->addWidget(new QLabel(tr("Please select an export format"), &d));
	vlayout->addWidget(list);
	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->addStretch(10);

	QPushButton * but = new QPushButton(tr("&Ok"), &d);
	connect(but, SIGNAL(pressed()), &d, SLOT(accept()));
	hlayout->addWidget(but);

	but = new QPushButton(tr("&Cancel"), &d);
	connect(but, SIGNAL(pressed()), &d, SLOT(reject()));
	hlayout->addWidget(but);

	vlayout->addLayout(hlayout);
	d.setLayout(vlayout);
	d.setWindowTitle(tr("Export format"));

	if (d.exec() == QDialog::Accepted && list->currentRow() != -1)
		m_exports[list->currentRow()]->execExport(m_exportNode);
}

void PreviewDialog::setVisible(bool visible)
{
	QDialog::setVisible(visible);
	if (visible)
		m_previewWidget->setFocus();

}

void PreviewDialog::print()
{
	if (!m_doc->numPages())
		return;
	bool showPrintDialog=m_showPrintDialog;
	int i=0;
	QPrinter * printer=0;
	QPainter painter;

newOrientation:

	delete printer;
	printer = new QPrinter();
	if (!m_printerName.isEmpty())
		printer->setPrinterName(m_printerName);
	printer->setPaperSize(m_pages[i].page->pageSize()*UNIT,QPrinter::Millimeter);
	printer->setOrientation(m_pages[i].page->paperOrientation());
	printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
	if (!m_reportName.isEmpty())
		printer->setDocName(m_reportName);

	if (showPrintDialog)
	{
		QPrintDialog d(printer, this);
		if (d.exec() == QDialog::Rejected)
			return;
	}
	showPrintDialog=false;
	bool firstPage=true;
	painter.begin(printer);
	for (;i < m_doc->numPages();i++)
	{
		Page * p = m_pages[i].page;
		if (p->paperOrientation()!=printer->orientation())
		{
			painter.end();
			goto newOrientation;
		}

		if (!firstPage)
			printer->newPage();

		printer->setFullPage(true);
		firstPage=false;

		painter.resetTransform();
		painter.scale((double)printer->paperSize(QPrinter::DevicePixel).width()*UNIT/(double)printer->paperSize(QPrinter::Millimeter).width(),
			(double)printer->paperSize(QPrinter::DevicePixel).height()*UNIT/(double)printer->paperSize(QPrinter::Millimeter).height());
		p->render(&painter, QRectF(printer->pageRect(QPrinter::Millimeter).x()/UNIT,
					printer->pageRect(QPrinter::Millimeter).y()/UNIT,
					printer->pageRect(QPrinter::Millimeter).width()/UNIT,
					printer->pageRect(QPrinter::Millimeter).height()/UNIT));
	}
	painter.end();
	delete printer;
}

void PreviewDialog::clearSelection()
{
	foreach(QGraphicsItem * item, m_previewWidget->scene()->items())
		if (dynamic_cast<QGraphicsRectItem *>(item))
			delete item;
}

void PreviewDialog::drawSelection(QGraphicsItem *  parent, QRectF & rect)
{
	QGraphicsRectItem * r = new QGraphicsRectItem(rect, parent);
	QBrush b;
	b.setColor(QColor(25, 25, 25, 100));
	b.setStyle(Qt::SolidPattern);
	r->setBrush(b);
	QPen p;
	p.setWidth(0);
	p.setBrush(b);
	r->setPen(p);
	m_previewWidget->ensureVisible(r);
}

void PreviewDialog::searchNext(const QString& text)
{
	if (!m_pages.size())
	{
		emit(textNotFound());
		return;
	}

	clearSelection();

	if (!m_searchPage)
		m_searchPage = m_pages[0].page;

	int i = 0;
	for (;i < m_pages.size();i++)
		if (m_searchPage == m_pages[i].page)
			break;

	for (;i < m_pages.size();i++)
	{
		QRectF rect;
		if (m_pages[i].page->search(text, rect, Report::Page::NextResult))
		{
			drawSelection(m_pages[i].previewItem, rect);
			m_searchPage = m_pages[i].page;
			return;
		}
	}
	m_searchPage = 0;
	emit(textNotFound());
}

void PreviewDialog::searchPrevious(const QString& text)
{
	if (!m_pages.size())
	{
		emit(textNotFound());
		return;
	}

	clearSelection();

	if (!m_searchPage)
		m_searchPage = m_pages[m_pages.size()-1].page;

	int i = m_pages.size() - 1;
	for (;i >= 0;i--)
		if (m_searchPage == m_pages[i].page)
			break;

	for (;i >= 0;i--)
	{
		QRectF rect;
		if (m_pages[i].page->search(text, rect, Report::Page::PreviousResult))
		{
			drawSelection(m_pages[i].previewItem, rect);
			m_searchPage = m_pages[i].page;
			return;
		}
	}
	m_searchPage = 0;
	emit(textNotFound());
}

void PreviewDialog::firstPage()
{
	if (m_pages.size())
		m_previewWidget->ensureVisible(m_pages[0].previewItem);
}

void PreviewDialog::lastPage()
{
	if (m_pages.size())
		m_previewWidget->ensureVisible(m_pages[m_pages.size()-1].previewItem);
}

void PreviewDialog::readPrevious()
{
	QGraphicsItem * item = 0;
	for (int i = 0;i < m_previewWidget->viewport()->rect().height();i++)
		if ((item = m_previewWidget->itemAt(m_previewWidget->viewport()->rect().width() / 2, i)))
			break;
	if (!item)
		return;

	for (int i = 0;i < m_pages.size();i++)
		if (item == m_pages[i].previewItem)
		{
			if (i > 0)
				m_previewWidget->ensureVisible(m_pages[i-1].previewItem);
			break;
		}
}

void PreviewDialog::readNext()
{
	QGraphicsItem * item = 0;
	for (int i = m_previewWidget->viewport()->rect().height();i;i--)
		if ((item = m_previewWidget->itemAt(m_previewWidget->viewport()->rect().width() / 2, i)))
			break;
	if (!item)
		return;

	for (int i = 0;i < m_pages.size();i++)
		if (item == m_pages[i].previewItem)
		{
			if (i + 1 < m_pages.size())
				m_previewWidget->ensureVisible(m_pages[i+1].previewItem);
			break;
		}
}

void PreviewDialog::setSpaceBetweenPages(int spaceBetweenPages)
{
	m_spaceBetweenPages=spaceBetweenPages;
}

void PreviewDialog::setExportDocument(QDomNode exportNode)
{
	m_exportNode=exportNode;
}

void PreviewDialog::setDocument(QIODevice * doc)
{
	m_docDevice=doc;
	int y = m_spaceBetweenPages;
	int w = 0;
	foreach(pageStruct pag, m_pages)
		delete pag.page;

	m_pages.clear();

	delete m_previewWidget->scene();

	delete m_doc;
	m_doc = new Document(doc);

	QGraphicsScene * previewScene = new QGraphicsScene(this);

	previewScene->setBackgroundBrush(QBrush(Qt::gray));
	m_previewWidget->setScene(previewScene);
	m_previewWidget->resetTransform();
	m_previewWidget->scale((double)QDesktopWidget().screen()->width()/(screen_widthMM*10),(double)QDesktopWidget().screen()->height()/(screen_heightMM*10));
	for (int i = 0;i < m_doc->numPages();i++)
	{
		pageStruct pag;
		pag.page = m_doc->page(i);
		pag.previewItem= new PageGraphicsItem(pag.page);

		previewScene->addItem(pag.previewItem);
		pag.previewItem->setPos(0, y);
		pag.previewItem->setZValue(0);
		m_pages.push_back(pag);

		if (w<pag.previewItem->boundingRect().width())
			w=pag.previewItem->boundingRect().width();

		y += m_spaceBetweenPages + pag.previewItem->boundingRect().height();
	}
	previewScene->setSceneRect(0,0,w,y);
}

void PreviewDialog::reject()
{
	if (!m_showExitConfirm || QMessageBox::question(this, tr("Quit ?"), tr("Really quit?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		QDialog::reject();
}

void PreviewDialog::setPrinterName(const QString & name)
{
	m_printerName = name;
}

void PreviewDialog::setShowPrintDialog(bool show)
{
	m_showPrintDialog = show;
}

void PreviewDialog::setShowExitConfirm(bool show)
{
	m_showExitConfirm = show;
}

void PreviewDialog::setReportName(const QString & name)
{
	m_reportName = name;
}
}
