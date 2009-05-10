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
#include <QMessageBox>
#include "exaroapplication.h"
#include "ui_exceptiondialog.h"

ExaroApplication::ExaroApplication(int & argc, char ** argv)
 : QApplication(argc, argv)
{
}


ExaroApplication::~ExaroApplication()
{
}


bool ExaroApplication::notify(QObject * receiver, QEvent * event)
{
        bool res=false;
        try
        {
                res=QApplication::notify(receiver, event);
        }
	catch(const QString & error)
	{
		QDialog d;
 		Ui::exceptionDialog ed;
		ed.setupUi(&d);
		d.setWindowTitle(tr("Unhandled exception"));
		ed.exceptionTextEdit->setText(error);
		d.exec();
	}
        catch(...)
        {
		QMessageBox::critical( 0,tr("Unhandled exception"),tr("Unknown exception"));
        }
        return res;
}

