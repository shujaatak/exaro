#include "variableseditor.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>

VariablesEditor::VariablesEditor(QWidget * parent)
	:QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    m_table = new QTableWidget(this);
    m_table ->setColumnCount(2);
    QStringList h;
    h << tr("Variable") << tr ("Value");
    m_table ->setHorizontalHeaderLabels(h);
    m_table->horizontalHeader()->setStretchLastSection ( true );

    gridLayout->addWidget(m_table, 0, 0, 1, 1);
    connect ( m_table, SIGNAL(cellChanged(int,int)), this , SLOT(cellChanged(int,int)));
}

void VariablesEditor::setVar(QString var, QVariant value)
{
//    m_hash.insert(var, value);
//    refresh();
}


void VariablesEditor::setVariables(QStringList list)
{
    qDebug("VariablesEditor::setVariables()");
    qDebug("count = %i", list.count() );
    QHash <QString, QString> hash;
    for (int i=0; i<m_table->rowCount(); i++)
	if (m_table->item(i,1))
	    hash.insert(m_table->item(i,0)->data(Qt::EditRole).toString(), m_table->item(i,1)->data(Qt::EditRole).toString());

    m_table->clearContents();
    m_table->setRowCount(list.count());
    int row = 0;
    foreach (QString var, list)
    {
	QTableWidgetItem *newItem = new QTableWidgetItem(var);
	m_table->setItem(row, 0, newItem);

	if (hash.contains( var ) )
	{
	    QTableWidgetItem *newItem = new QTableWidgetItem(hash.value(var ));
	    m_table->setItem(row , 1, newItem);
	}
	row++;
    }

    m_table->resizeRowsToContents ();
}

void VariablesEditor::setVarsMap(QVariantMap list)
{
    m_table->clearContents();
    m_table->setRowCount(list.count());
    int row = 0;
    foreach (QString var, list.keys())
    {
	QTableWidgetItem *newItem = new QTableWidgetItem(var);
	m_table->setItem(row, 0, newItem);

	if (list.contains( var ) )
	{
	    QTableWidgetItem *newItem = new QTableWidgetItem(list.value(var ).toString());
	    m_table->setItem(row , 1, newItem);
	}
	row++;
    }
    m_table->resizeRowsToContents ();
}


void VariablesEditor::addVar(QString var)
{
    m_table->setRowCount(m_table->rowCount() + 1);
    QTableWidgetItem *newItem = new QTableWidgetItem(var);
    m_table->setItem(m_table->rowCount() -1, 0, newItem);
    m_table->resizeRowsToContents ();
}

void VariablesEditor::removeVar(QString var)
{
    for (int i=0; i<m_table->rowCount(); i++)
	if (m_table->item(i,0)->data(Qt::EditRole).toString() == var)
	{
	m_table->removeRow(i);
	break;
    }
}

QStringList VariablesEditor::variables()
{
    QStringList list;
    for (int i=0; i<m_table->rowCount(); i++)
	list.append( m_table->item(i,0)->data(Qt::EditRole).toString() );

    return list;
}

QVariantMap VariablesEditor::varsMap()
{
    QVariantMap list;
    for (int i=0; i<m_table->rowCount(); i++)
	if (m_table->item(i,1))
	    list.insert( m_table->item(i,0)->data(Qt::EditRole).toString(), m_table->item(i,1)->data(Qt::EditRole).toString() );
	else
	    list.insert( m_table->item(i,0)->data(Qt::EditRole).toString(), QVariant() );
    return list;
}

void VariablesEditor::cellChanged (int row, int column)
{
    QString name =  m_table->item(row, 0)->data(Qt::EditRole).toString();
    QString value =  m_table->item(row, 1) ? m_table->item(row, 1)->data(Qt::EditRole).toString() : QString();
    emit variableChanged(name, value);
}
