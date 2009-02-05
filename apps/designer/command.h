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
	AddCommand( Report::PageInterface* page, const char* itemClassName, QPointF pos, mainWindow* mw );

	void undo();
	void redo();

private:
	QString m_pageName;
	const char* m_itemClassName;
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
};

/*
class ChangePageCommand: public QObject, public QUndoCommand
{
    Q_OBJECT
public:

    ChangePageCommand(mainWindow * mw, int newIndex, int oldIndex);

    void undo();
    void redo();

private:
    mainWindow* mw;
    int m_index;
    QString pageName;
    Report::PageInterface* m_page;
    QString newName;
    QString oldName;
    bool fromStack;
};
*/

QString createCommandString( Report::ItemInterface *item, const QPointF &pos );
QString createCommandString( const char* name, const QPointF &pos );

QObject* findObject( Report::PageInterface * page, QString name );
QWidget * findObjectByTabName( QTabWidget * tw, QString tabName );
int findIndexByTabName( QTabWidget * tw, QString tabName );

#endif // COMMAND_H
