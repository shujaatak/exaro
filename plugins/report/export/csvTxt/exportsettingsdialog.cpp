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


#include "exportsettingsdialog.h"

ExportSettingsDialog::ExportSettingsDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::Dialog()
{
	setupUi(this);
}

ExportSettingsDialog::~ExportSettingsDialog()
{
}

QString ExportSettingsDialog::getDelimiter()
{
	QString result = ",";
#if defined(MOBILE_UI)
if (delimiterCombo->currentIndex() == (delimiterCombo->count()-1))
		result = delimiterEdit->text();
	else
		result = delimiterCombo->currentText();
#else
	if (other->isChecked())
		result = delimiter->text();
	if (comma->isChecked())
		result = ",";
	if (semicolon->isChecked())
		result = ";";
	if (space->isChecked())
		result = " ";
	if (tabulator->isChecked())
		result = QString(1,0x09);
#endif
	return result;
}

QString ExportSettingsDialog::getQuote()
{
	QString result = "";
#if defined(MOBILE_UI)
	if (quoteCombo->currentIndex() == (quoteCombo->count()-1))
		result = quoteEdit->text();
	else
		result = quoteCombo->currentText();
#else
	if (quote_3->isChecked())
		result = quote->text();

	if (quote_1->isChecked())
		result = "\"";

	if (quote_2->isChecked())
		result = "'";
#endif
	return result;
}


#if defined(MOBILE_UI)
void ExportSettingsDialog::on_delimiterCombo_currentIndexChanged(int index)
{
	delimiterEdit->setEnabled(index == (delimiterCombo->count()-1));
}

void ExportSettingsDialog::on_quoteCombo_currentIndexChanged(int index)
{
	quoteEdit->setEnabled(index == (quoteCombo->count()-1));
}

#endif

/*$SPECIALIZATION$*/


