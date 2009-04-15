/***************************************************************************
 *   Copyright (C) 2009 by Alexander Mikhalov                              *
 *   alexmi3@rambler.ru                                                    *
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

#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QToolBox>

#include "mainwindow.h"

class QUndoCommand;

class mainWindow;


class AddCommand: public QUndoCommand
{
public:
	AddCommand( Report::PageInterface* page, const QString & itemClassName, QPointF pos, mainWindow* mw );

	void undo();
	void redo();

private:
	QString m_pageName;
	QString m_itemClassName;
	QPointF m_pos;
	mainWindow * m_mainWindow;
	QString m_itemName;
	bool m_canUndo;
};

class AddDomObject: public QUndoCommand
{
public:
	AddDomObject( Report::PageInterface* page, const QString & parent, const QString & domObject, QPointF pos, mainWindow* mw );

	void undo();
	void redo();
	void makeUnique(QObject * object) const;
	bool isUnique(QObject * object, QObject * ownerObject, const QString & name ) const;

private:
	QString m_itemName;
	QString m_parentName;
	QString m_pageName;
	QPointF m_pos;
	mainWindow * m_mainWindow;
	QString m_domObject;
	bool m_canUndo;
};



class MoveCommand: public QUndoCommand
{
public:
	MoveCommand( Report::ItemInterface *item, const QPointF &oldPos, mainWindow* mw );

	void redo();
	void undo();

private:
	mainWindow * m_mainWindow;
	QString m_pageName;
	QString m_itemName;
	QPointF m_newPos;
	QPointF m_oldPos;
};


class DelCommand: public QUndoCommand
{
public:
	DelCommand( Report::ItemInterface* item, mainWindow* mw );

	void undo();
	void redo();

private:
	mainWindow* m_mainWindow;
	QString m_pageName;
	QString m_parentName;
	QString m_itemName;
	QString m_domObject;
};

class PropertyChangeCommand: public QUndoCommand
{
public:
	enum { Id = 1234 };

	PropertyChangeCommand( QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value, mainWindow* mw );

	void undo();
	void redo();
	bool mergeWith( const QUndoCommand *command );
	int id() const
	{
		return Id;
	}

private:
	mainWindow* m_mainWindow;
	QString m_itemName;
	QString m_pageName;
	QString m_propertyName;
	QVariant m_oldValue;
	QVariant m_newValue;
};


class GeometryChangeCommand: public QUndoCommand
{
public:
	enum { Id = 2345 };

	GeometryChangeCommand( QObject* obj, QRectF newGeometry, QRectF oldGeometry, mainWindow* mw );

	void undo();
	void redo();
	bool mergeWith( const QUndoCommand *command );
	int id() const
	{
		return Id;
	}

private:
	mainWindow* m_mainWindow;
	QString m_itemName;
	QString m_pageName;
	QRectF m_oldGeometry;
	QRectF m_newGeometry;
};


class NewPageCommand: public QUndoCommand
{
public:

	NewPageCommand( mainWindow * mw );

	void undo();
	void redo();

private:
	mainWindow* m_mainWindow;
	int m_index;
};


class RemovePageCommand: public QUndoCommand
{
public:

	RemovePageCommand( mainWindow * mw, int index );

	void undo();
	void redo();

private:
	mainWindow* m_mainWindow;
	QString m_pageName;
	QString m_domObject;
};

QString createCommandString( Report::ItemInterface *item, const QPointF &pos );
QString createCommandString( const QString & name, const QPointF &pos );

QObject* findObject( Report::PageInterface * page, QString name );
QWidget * findObjectByTabName( QTabWidget * tw, QString tabName );
int findIndexByTabName( QTabWidget * tw, QString tabName );
void connectItems(QObject * object, QObject * mw);

#endif // COMMAND_H
