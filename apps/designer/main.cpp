/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bogdan@licentia.eu                                                    *
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


#include <QTranslator>
#include <QDir>

//#include <QSplashScreen>
//#include <QPixmap>
#include "exaroapplication.h"
#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication::setOrganizationName("licentia");
	QApplication::setOrganizationDomain("licentia.eu");
	QApplication::setApplicationName("eXaro");
	ExaroApplication app(argc, argv);

	QDir dir;
#if defined(Q_OS_WIN)
	dir.setPath(app.applicationDirPath()+"/translations/");
#elif defined(Q_OS_MAC)
	dir.setPath(app.applicationDirPath()+"/translations/");
#else
	dir.setPath(app.applicationDirPath()+"/../share/eXaro/translations/");
#endif
	QTranslator translator;
	if (dir.exists())
		foreach (QString file, dir.entryList(QStringList()<<QString("*_%1.qm").arg(QLocale::system().name()),QDir::Readable|QDir::Files))
		{
			translator.load(file,dir.absolutePath()+"/");
			app.installTranslator(&translator);
		}
/*
	QPixmap pixmap(":/images/exaro_splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
*/
	mainWindow w;
	w.show();

//	splash.finish(&w);
	return app.exec();
}

