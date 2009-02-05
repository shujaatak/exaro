#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QToolBox>

#include "mainwindow.h"

class QUndoCommand;

class mainWindow;


class AddCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	AddCommand( Report::PageInterface* page, const char* itemClassName, QPointF pos, mainWindow* mw );

	void undo();
	void redo();

private:
	QString pageName;
	const char* m_itemClassName;
	QPointF m_pos;
	mainWindow * mw;
	QString itemName;
};

class AddDomObject: public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	AddDomObject( Report::PageInterface* page, const QString & parent, const QString & domObject, QPointF pos, mainWindow* mw );

	void undo();
	void redo();

private:
	QString m_itemName;
	QString m_parentName;
	QString m_pageName;
	QPointF m_pos;
	mainWindow * mw;
	QString m_domObject;
	bool m_canUndo;
};



class MoveCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	MoveCommand( Report::ItemInterface *item, const QPointF &oldPos, mainWindow* mw );

	void redo();
	void undo();

private:
	mainWindow * mw;
	QString pageName;
	QString itemName;
	QPointF m_newPos;
	QPointF m_oldPos;
};


class DelCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	DelCommand( Report::ItemInterface* item, mainWindow* mw );

	void undo();
	void redo();

private:
	mainWindow* mw;
	QString pageName;
	QString parentName;
	QString itemName;
	QString domObject;
};

class PropertyChangeCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
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
	mainWindow* mw;
	QString itemName;
	QString pageName;
	QString propertyName;
	QVariant old_value;
	QVariant new_value;
};


class GeometryChangeCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
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
	mainWindow* mw;
	QString itemName;
	QString pageName;
	QRectF oldGeometry;
	QRectF newGeometry;
};


class NewPageCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
public:

	NewPageCommand( mainWindow * mw );

	void undo();
	void redo();

private:
	mainWindow* mw;
	int m_index;
};


class RemovePageCommand: public QObject, public QUndoCommand
{
	Q_OBJECT
public:

	RemovePageCommand( mainWindow * mw, int index );

	void undo();
	void redo();

private:
	mainWindow* mw;
//    int m_index;
	QString pageName;
//    Report::PageInterface* m_page;
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
