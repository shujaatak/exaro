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

#include <QtCore>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFontMetricsF>

#include "barcode.h"
#include "qzint.h"

using namespace Zint;

inline void initMyResource()
{
	Q_INIT_RESOURCE(barcode);
}

Barcode::Barcode(QGraphicsItem* parent, QObject* parentObject) : ItemInterfaceExt(parent, parentObject)
{
	m_barcodeType = (BarcodeTypes)BARCODE_CODE128;
	m_paintType = IgnoreAspectRatio;
	m_frameType = NO_FRAME;
	m_barcodeHeight = 10/UNIT;
	m_barcodeWhitespaceWidth = 0;;
	m_barcodeBoderWidth = 0;
	m_barcodeForegroundColor = qRgb(0, 0, 0);
	m_barcodeBackgroundColor = qRgb(0xff, 0xff, 0xff);
	m_barcodeWidth = 0;
	m_barcodeSecurityLevel = -1;
	m_script = "05072006";
	m_testText = "05072006";
	m_drawTextType = NO_TEXT;
	m_pdf417Max = 928;
	m_msiPlessey=MSI_PLESSEY_;
	m_exCode39_43Extension=false;
	initMyResource();
	setWidth(50/UNIT);
	setHeight(10/UNIT);
}

Barcode::BarcodeTypes Barcode::barcodeType()
{
	return m_barcodeType;
}
void Barcode::setBarcodeType(BarcodeTypes barcodeType)
{
	m_barcodeType = barcodeType;
	update();
}

Barcode::PaintTypes Barcode::paintType()
{
	return m_paintType;
}
void Barcode::setPaintType(PaintTypes paintType)
{
	m_paintType = paintType;
	update();
}

Barcode::FrameTypes Barcode::frameType()
{
	return m_frameType;
}

void Barcode::setFrameType(FrameTypes frameType)
{
	m_frameType = frameType;
	update();
}

int Barcode::barcodeHeight()
{
	return m_barcodeHeight;
}
void Barcode::setBarcodeHeight(int barcodeHeight)
{
	m_barcodeHeight = barcodeHeight;
	update();
}

int Barcode::barcodeWhitespaceWidth()
{
	return m_barcodeWhitespaceWidth;
}
void Barcode::setBarcodeWhitespaceWidth(int barcodeWhitespaceWidth)
{
	m_barcodeWhitespaceWidth = barcodeWhitespaceWidth;
	update();
}

int Barcode::barcodeBoderWidth()
{
	return m_barcodeBoderWidth;
}
void Barcode::setBarcodeBoderWidth(int barcodeBoderWidth)
{
	m_barcodeBoderWidth = barcodeBoderWidth;
	update();
}

QColor Barcode::barcodeForegroundColor()
{
	return m_barcodeForegroundColor;
}
void Barcode::setBarcodeForegroundColor(QColor barcodeForegroundColor)
{
	m_barcodeForegroundColor = barcodeForegroundColor;
	update();
}

QColor Barcode::barcodeBackgroundColor()
{
	return m_barcodeBackgroundColor;
}
void Barcode::setBarcodeBackgroundColor(QColor barcodeBackgroundColor)
{
	m_barcodeBackgroundColor = barcodeBackgroundColor;
	update();
}

int Barcode::barcodeWidth()
{
	return m_barcodeWidth;
}
void Barcode::setBarcodeWidth(int barcodeWidth)
{
	if (barcodeWidth<0)
		barcodeWidth=0;
	if (barcodeWidth>50)
		barcodeWidth=50;
	m_barcodeWidth = barcodeWidth;
	update();
}

int Barcode::barcodeSecurityLevel()
{
	return m_barcodeSecurityLevel;
}
void Barcode::setBarcodeSecurityLevel(int barcodeSecurityLevel)
{
	m_barcodeSecurityLevel = barcodeSecurityLevel;
	update();
}

int Barcode::pdf417Max()
{
	return m_pdf417Max;
}
void Barcode::setPdf417Max(int pdf417Max)
{
	m_pdf417Max=pdf417Max;
	if (m_pdf417Max<928)
		m_pdf417Max=928;
	if (m_pdf417Max>1900)
		m_pdf417Max=1900;
}

Barcode::MsiPlessey Barcode::msiPlessey()
{
	return m_msiPlessey;
}
void Barcode::setMsiPlessey(MsiPlessey msiPlessey)
{
	m_msiPlessey=msiPlessey;
	update();
}

bool Barcode::exCode39_43Extension()
{
	return m_exCode39_43Extension;
}
void Barcode::setExCode39_43Extension(bool exCode39_43Extension)
{
	m_exCode39_43Extension=exCode39_43Extension;
	update();
}

Barcode::DrawTextTypes Barcode::drawTextType()
{
	return m_drawTextType;
}
void Barcode::setDrawTextType(DrawTextTypes drawTextType)
{
	m_drawTextType = drawTextType;
	update();
}

const QString & Barcode::primaryMessage()
{
	return m_primaryMessage;
}
void Barcode::setPrimaryMessage(const QString &primaryMessage)
{
	m_primaryMessage=primaryMessage;
	update();
}

QString Barcode::script()
{
	return m_script;
}

void Barcode::setScript(const QString &script)
{
	m_script = script;
	update();
}

QString Barcode::testText()
{
	return m_testText;
}
void Barcode::setTestText(const QString & testText)
{
	m_testText=testText;
	update();
}

QRectF Barcode::boundingRect() const
{
	return QRectF(0, 0, width(), height());
}

void Barcode::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
	QString error;
	QString m_text;
	if (option->type == QStyleOption::SO_GraphicsItem)
		m_text=m_testText;
	else
		m_text=scriptEngine()->evaluate(m_script).toString();


	if (option->type != QStyleOption::SO_GraphicsItem)
		emit beforePrint(this);

	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);

	adjustRect(rect);

	Zint::QZint bc;

	bc.setSymbol(m_barcodeType);
	switch(m_barcodeType)
	{
		case  MSI_PLESSEY:
			bc.setMsiExtraSymbology(m_msiPlessey);
			break;
		case  CODE39:
			bc.setCode39ExtraSymbology(m_exCode39_43Extension);
			break;
		case  EXCODE39:
			bc.setExcode39ExtraSymbology(m_exCode39_43Extension);
			break;
	}
	bc.setPrimaryMessage(m_primaryMessage);
	bc.setBorderType((Zint::QZint::BorderType)m_frameType);
	bc.setHeight(m_barcodeHeight);
	bc.setWidth(m_barcodeWidth);
	bc.setSecurityLevel(m_barcodeSecurityLevel);
	bc.setBorderWidth(m_barcodeBoderWidth);
	bc.setPdf417CodeWords(m_pdf417Max);
	bc.setFgColor(m_barcodeForegroundColor);
	bc.setBgColor(m_barcodeBackgroundColor);
	bc.setText(m_text);

	bc.render(*painter,QRectF(rect.x()+1,rect.y()+1+((m_drawTextType==DRAW_ABOVE)?painter->fontMetrics().height():0),rect.width()-2, rect.height()-2-((m_drawTextType==DRAW_ABOVE || m_drawTextType==DRAW_BELOW)?painter->fontMetrics().height():0)),(Zint::QZint::AspectRatioMode)m_paintType);

	switch (m_drawTextType)
	{
		case DRAW_TOP:
			painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, m_text);
			break;
		case DRAW_BOTTOM:
			painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, m_text);
			break;
		case DRAW_ABOVE:
			painter->drawText(rect, Qt::AlignTop| Qt::AlignHCenter, m_text);
			break;
		case DRAW_BELOW:
			painter->drawText(rect, Qt::AlignBottom| Qt::AlignHCenter, m_text);
			break;
		default:
			break;
	}

	if (option->type != QStyleOption::SO_GraphicsItem)
		emit afterPrint(this);

}

QIcon Barcode::toolBoxIcon()
{
	return QIcon(":/barcode.png");
};

QString Barcode::toolBoxText()
{
	return tr("Barcode");
}

QString Barcode::toolBoxGroup()
{
	return tr("Shapes");
}

QObject * Barcode::createInstance(QGraphicsItem* parent, QObject* parentObject)
{
	return new Barcode(parent, parentObject);
}

Q_EXPORT_PLUGIN2(barcode, Barcode)
