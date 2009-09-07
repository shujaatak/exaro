/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2009 by Mikhalov Alexaner                               *
 *   alexmi3@rambler.ru                                                    *
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
    show(mNormal, str, sec);
}

void Message::show( MessageType type, QString str, int sec)
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
