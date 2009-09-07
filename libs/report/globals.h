/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
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

#ifndef REPORTGLOBALS_H
#define REPORTGLOBALS_H

#include <QDomElement>
#include <QDomDocument>
#include <QVariant>
#include <QPainterPath>
#ifdef WIN32
#ifdef LIB_EXPORTS
#define EXARO_EXPORTS __declspec(dllexport)
#else
#define EXARO_EXPORTS __declspec(dllimport)
#endif
#else
#define EXARO_EXPORTS
#endif

//0.1 MM
#define UNIT 0.1

namespace Report
{

EXARO_EXPORTS QDomElement variantToDom(QDomDocument * doc, const QString & name, const QVariant & value);
EXARO_EXPORTS QVariant domToVariant(const QDomElement & dom);
EXARO_EXPORTS bool typeIsNumber(QVariant::Type type);

}
Q_DECLARE_METATYPE(QPainterPath);
#endif
