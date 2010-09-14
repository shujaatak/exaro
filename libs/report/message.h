#ifndef MESSAGE_H
#define MESSAGE_H

#include <QMessageBox>
#include <QtCore/QObject>
#include <QtScript/QScriptable>

class Message : public QMessageBox
{
    Q_OBJECT    
public:
    Message(QObject *parent = 0);
public slots:
    void setText(const QString &text);
    void setWindowTitle ( const QString & title );
};

#endif // MESSAGE_H
