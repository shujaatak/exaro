#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include <QWidget>
#include <QVariant>
#include <QStringList>

class QGridLayout;
class QTableWidget;

class VariablesEditor : public QWidget
{
    Q_OBJECT
public:
    VariablesEditor(QWidget * parent = 0);

    QStringList variables();
    QVariantMap varsMap();
    void setVariables(QStringList list);
    void setVarsMap(QVariantMap list);

signals:
    void variableChanged(QString name, QString value);

public slots:
    void setVar(QString var, QVariant value);
    void removeVar(QString var);
    void addVar(QString var);

private slots:
    void cellChanged (int row, int column);
private:
    QGridLayout *gridLayout;
    QTableWidget *m_table;
};

#endif // VARIABLESEDITOR_H
