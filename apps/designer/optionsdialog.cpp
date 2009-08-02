/***************************************************************************
 *   Copyright (C) 2009 by Alexander Mikhalov                              *
 *   alexmi3@rambler.ru                                                    *
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

#include <QtGui>
#include "optionsdialog.h"

OptionsDialog::OptionsDialog( QWidget *parent ) :
		QDialog( parent )
{
	setupUi( this );
	QSettings s;
	leDesignerPath->setText( s.value( "Options/designerPath" ).toString() );
	cbIconSize->setCurrentIndex( cbIconSize->findText( s.value( "Options/iconSize" ).toString() ) );
	drawSelection->setChecked( s.value( "Items/drawSelectionBorder", true ).toBool());
}

void OptionsDialog::on_bDesignerPath_clicked()
{
#if defined(Q_OS_WIN)
	QString fileName = QFileDialog::getOpenFileName( this, "designer.exe", ".", "designer.exe" );
#else
	QString fileName = QFileDialog::getOpenFileName( this, "designer", "/usr/bin", "designer" );
#endif
	leDesignerPath->setText( fileName );
}

void OptionsDialog::accept()
{
	QSettings s;
	s.setValue( "Options/designerPath", leDesignerPath->text() );
	s.setValue( "Options/iconSize", cbIconSize->currentText() );
	s.setValue( "Items/drawSelectionBorder", drawSelection->isChecked());
	QDialog::accept();
}
