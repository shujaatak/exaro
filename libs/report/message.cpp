#include "message.h"


Message::Message(QObject *parent)    
{
    QMessageBox::setWindowTitle("Atentie");
}

void Message::setText(const QString &text)
{     
    QMessageBox::setText(text);
}

void Message::setWindowTitle(const QString &title)
{
    QMessageBox::setWindowTitle(title);
}
