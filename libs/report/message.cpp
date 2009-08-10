#include "message.h"
#include <QtGui/QLabel>
#include <QTimer>

Message * Message::m_instance = 0;

Message::Message()
	:m_monitor(0)
{
    timer = new QTimer( this );
    timer->setSingleShot( true );
    connect ( timer, SIGNAL(timeout()), this, SLOT(hide()));
}

Message * Message::instance()
{
    if (!m_instance)
	m_instance = new Message();
    return m_instance;
}

void Message::assignMonitor(QWidget * w)
{
    hide();
    timer->stop();

    m_monitor = w;
}

void Message::show(QString str, int sec)
{
    if (!m_monitor)
    {
	qDebug("Message:: %s",qPrintable(str));
	return;
    }

    if (dynamic_cast<QLabel *>(m_monitor) )
    {
	dynamic_cast<QLabel *>(m_monitor)->setText( str );
    }


    timer->start( sec * 1000 );
}

void Message::hide()
{
    if (dynamic_cast<QLabel *>(m_monitor) )
    {
	dynamic_cast<QLabel *>(m_monitor)->setText( "");
    }
}
