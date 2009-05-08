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

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QUiLoader>
#include <QBuffer>
#include <QSettings>

#include "designeruiwidget.h"
#include "designeruitype.h"


namespace Report
{

DesignerUiWidget::DesignerUiWidget(QWidget* parent, Qt::WFlags fl)
		: QWidget(parent, fl), Ui::designerUiWidget()
{
	setupUi(this);
	setWindowTitle(tr("Uis"));
	m_listWidget->clear();
	refreshButtons();
	connect(m_addButton, SIGNAL(clicked()), this, SLOT(createItem()));
	connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()));
	connect(m_editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(m_editName, SIGNAL(clicked()), this, SLOT(editName()));
}

DesignerUiWidget::~DesignerUiWidget()
{
}


void DesignerUiWidget::editItem()
{
	QTemporaryFile file(QDir::tempPath() + "/XXXXXXXXXXXXXX.ui");
	if (!file.open())
	{
		qCritical() << "Can't create temp file";
		return;
	}
	file.write(m_uis[m_listWidget->currentItem()->text()].toByteArray());
	QString fileName = file.fileName();
	file.close();
	if (!m_uis[m_listWidget->currentItem()->text()].toByteArray().size())
		file.remove();
	QSettings s;
	s.beginGroup("Tools");
	QString m_designerTool = s.value("designer", "designer").toString();
	s.endGroup();

	QProcess designer;
	designer.start(m_designerTool, QStringList() << fileName);
	if (!designer.waitForStarted())
	{
		qCritical() << tr("Can't create process :") <<m_designerTool;
		return;
	}

	while (!designer.waitForFinished(500)) // wait designer to quit
		qApp->processEvents();

	QFile uiFile(fileName);
	if (!uiFile.open(QIODevice::ReadWrite))
	{
		qCritical() << tr("Can't open temp file");
		return;
	}
	m_uis[m_listWidget->currentItem()->text()] = QString(uiFile.readAll());
	uiFile.close();
	uiFile.remove();
}

void DesignerUiWidget::editName()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ui wigdet"), tr("ui name:"), QLineEdit::Normal, m_listWidget->currentItem()->text(), &ok);

	if (!ok || text.isEmpty())
		return;

	QString ui = m_uis[m_listWidget->currentItem()->text()].toString();
	m_uis.remove(m_listWidget->currentItem()->text());
	m_uis[text] = ui;
	m_listWidget->currentItem()->setText(text);
}

void DesignerUiWidget::refreshButtons()
{
	m_deleteButton->setEnabled(m_listWidget->count());
	m_editButton->setEnabled(m_listWidget->count());
	m_editName->setEnabled(m_listWidget->count());
}


void DesignerUiWidget::createItem()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Create an ui widget"), tr("ui name:"), QLineEdit::Normal, QString("ui_%1").arg(m_listWidget->count()), &ok);

	if (!ok || text.isEmpty())
		return;

	QListWidgetItem * i = new QListWidgetItem();
	i->setText(text);
	m_listWidget->addItem(i);
	m_listWidget->setCurrentItem(i);
	refreshButtons();
	m_uis[text] = "";
	DesignerUiType uitype;
	uitype.exec();

	if (uitype.customRadioButton->isChecked())
		m_uis[text] ="";
	else
		if (uitype.widgetRadioButton->isChecked())
			m_uis[text] = "<ui version=\"4.0\" ><class>Form</class><widget class=\"QWidget\" name=\"Form\"><property name=\"windowTitle\" ><string>Form</string></property></widget></ui>";
		else
			m_uis[text] = "<ui version=\"4.0\" ><class>Dialog</class><widget class=\"QDialog\" name=\"Dialog\"><property name=\"windowTitle\" ><string>Dialog</string></property></widget></ui>";
}

QMap <QString, QVariant> DesignerUiWidget::uis()
{
	return m_uis;
}

void DesignerUiWidget::setUis(QMap <QString, QVariant> uis)
{
	m_uis = uis;
	m_listWidget->clear();
	foreach(QString keys, m_uis.keys())
		m_listWidget->addItem(keys);

	if (m_listWidget->count())
		m_listWidget->setCurrentRow(0);

	refreshButtons();
}

void DesignerUiWidget::deleteItem()
{

	if (QMessageBox::Ok != QMessageBox::question(this, tr("eXaro"), tr("Delete current ui ?"), QMessageBox::Ok | QMessageBox::Cancel))
		return;

	m_uis.remove(m_listWidget->currentItem()->text());

	delete m_listWidget->currentItem();

	refreshButtons();
}

}
