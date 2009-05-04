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

#if defined __GNUC__ && __linux

#ifdef __i386__
#include "i386-signal.h"
#endif /*__i386__*/

#ifdef __x86_64__
#include "x86_64-signal.h"
#endif /*__x86_64__*/

#endif /*defined __GNUC__ && __linux*/


#if defined (HANDLE_SEGV) || defined(HANDLE_FPE)

/* Unblock a signal.  Unless we do this, the signal may only be sent
   once.  */
static void
unblock_signal (int signum __attribute__ ((__unused__)))
{
#ifdef _POSIX_VERSION
	sigset_t sigs;
	sigemptyset (&sigs);
	sigaddset (&sigs, signum);
	sigprocmask (SIG_UNBLOCK, &sigs, NULL);
#endif
}
#endif


#ifdef HANDLE_SEGV
SIGNAL_HANDLER (catch_segv)
{
	unblock_signal (SIGSEGV);
	MAKE_THROW_FRAME (nullp);
	QString nullp="Null pointer";
	throw nullp;
}
#endif

#ifdef HANDLE_FPE
SIGNAL_HANDLER (catch_fpe)
{
	unblock_signal (SIGFPE);
#ifdef HANDLE_DIVIDE_OVERFLOW
	HANDLE_DIVIDE_OVERFLOW;
#else
	MAKE_THROW_FRAME (arithexception);
#endif
	QString arithexception="/ by zero";
	throw arithexception;
}
#endif

int main(int argc, char **argv)
{
	#ifdef HANDLE_SEGV
	INIT_SEGV;
	#endif

	#ifdef HANDLE_FPE
	INIT_FPE;
	#endif

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

