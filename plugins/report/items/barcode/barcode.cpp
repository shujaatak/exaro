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

Barcode::Barcode(QGraphicsItem* parent, QObject* parentObject) : ItemInterface(parent, parentObject)
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
	m_script = "05072006";
	m_testText = "05072006";
/*	m_drawTextType = NO_TEXT;*/
	m_pdf417Max = 928;
	m_pdfDataColmns=0;
	m_pdf_ECC=ECC_AUTO;
	m_msiPlessey=MSI_PLESSEY_;
	m_aztecResize=AUTOMATIC_RESIZE;
	m_inputMode=UNICODE;
	m_pdfDataColmns=0;
	m_pdf_ECC=ECC_AUTO;
	m_code39CheckDigit=NO_CHECK_DIGIT;
	m_maxiCodeEncodingMode=MODE_4;
	m_barcodeChannelChannels=AUTO;
	m_codeoneSymbolSize=AUTOMATIC_SYMBOL_SIZE;
	m_microQrResize=MQR_AUTO_RESIZE;
	m_qrResize=QR_AUTO_RESIZE;
	m_dataMatrixEncodingMode=ECC_200_RECOMMENDED;
	m_dataMatrixNonECC200Size=NONECC200SIZE_AUTOMATIC;
	m_dataMatrixECC200Size=ECC200SIZE_AUTOMATIC;
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

Barcode::InputMode Barcode::inputMode()
{
	return m_inputMode;
}

void Barcode::setInputMode(Barcode::InputMode inputMode)
{
	m_inputMode=inputMode;
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


Barcode::AztecResize Barcode::aztecResize()
{
	return m_aztecResize;
}
void Barcode::setAztecResize(Barcode::AztecResize aztecResize)
{
	m_aztecResize=aztecResize;
	update();
}


Barcode::Pdf_ECC Barcode::pdf_ECC()
{
	return m_pdf_ECC;
}
void Barcode::setPdf_ECC(Barcode::Pdf_ECC pdf_ECC)
{
	m_pdf_ECC=pdf_ECC;
	update();
}


int Barcode::pdfDataColmns()
{
	return m_pdfDataColmns;
}
void Barcode::setPdfDataColmns(int pdfDataColmns)
{
	if (pdfDataColmns<0||pdfDataColmns>20)
		pdfDataColmns=0;
	m_pdfDataColmns=pdfDataColmns;
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

Barcode::Code39CheckDigit Barcode::code39CheckDigit()
{
	return m_code39CheckDigit;
}
void Barcode::setCode39CheckDigit(Barcode::Code39CheckDigit code39CheckDigit)
{
	m_code39CheckDigit=code39CheckDigit;
	update();
}

// Barcode::DrawTextTypes Barcode::drawTextType()
// {
// 	return m_drawTextType;
// }
// void Barcode::setDrawTextType(DrawTextTypes drawTextType)
// {
// 	m_drawTextType = drawTextType;
// 	update();
// }

const QString & Barcode::primaryMessage()
{
	return m_primaryMessage;
}
void Barcode::setPrimaryMessage(const QString &primaryMessage)
{
	m_primaryMessage=primaryMessage;
	update();
}


Barcode::MaxiCodeEncodingMode Barcode::maxiCodeEncodingMode()
{
	return m_maxiCodeEncodingMode;
}
void Barcode::setMaxiCodeEncodingMode(Barcode::MaxiCodeEncodingMode maxiCodeEncodingMode)
{
	m_maxiCodeEncodingMode=maxiCodeEncodingMode;
	update();
}


Barcode::BarcodeChannelChannels Barcode::barcodeChannelChannels()
{
	return m_barcodeChannelChannels;
}
void Barcode::setBarcodeChannelChannels(Barcode::BarcodeChannelChannels barcodeChannelChannels)
{
	m_barcodeBackgroundColor=barcodeChannelChannels;
	update();
}

Barcode::CodeoneSymbolSize Barcode::codeoneSymbolSize()
{
	return m_codeoneSymbolSize;
}
void Barcode::setCodeoneSymbolSize(Barcode::CodeoneSymbolSize codeoneSymbolSize)
{
	m_codeoneSymbolSize=codeoneSymbolSize;
	update();
}

Barcode::MicroQrResize Barcode::microQrResize()
{
	return m_microQrResize;
}
void Barcode::setMicroQrResize(Barcode::MicroQrResize microQrResize)
{
	m_microQrResize=microQrResize;
	update();
}

Barcode::QrResize Barcode::qrResize()
{
	return m_qrResize;
}
void Barcode::setQrResize(Barcode::QrResize qrResize)
{
	m_qrResize=qrResize;
	update();
}

Barcode::DataMatrixEncodingMode Barcode::dataMatrixEncodingMode()
{
	return m_dataMatrixEncodingMode;
}
void Barcode::setDataMatrixEncodingMode(Barcode::DataMatrixEncodingMode dataMatrixEncodingMode)
{
	m_dataMatrixEncodingMode=dataMatrixEncodingMode;
	update();
}

Barcode::DataMatrixECC200Size Barcode::dataMatrixECC200Size()
{
	return m_dataMatrixECC200Size;
}
void Barcode::setDataMatrixECC200Size(Barcode::DataMatrixECC200Size dataMatrixECC200Size)
{
	m_dataMatrixECC200Size=dataMatrixECC200Size;
	update();
}

Barcode::DataMatrixNonECC200Size Barcode::dataMatrixNonECC200Size()
{
	return m_dataMatrixNonECC200Size;
}
void Barcode::setDataMatrixNonECC200Size(Barcode::DataMatrixNonECC200Size dataMatrixNonECC200Size)
{
	m_dataMatrixNonECC200Size=dataMatrixNonECC200Size;
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


	QRectF rect = (option->type == QStyleOption::SO_GraphicsItem) ? boundingRect() : option->exposedRect;

	if (option->type == QStyleOption::SO_GraphicsItem)
		drawSelection(painter, boundingRect());

	setupPainter(painter);

	adjustRect(rect);

	Zint::QZint bc;

	bc.setSymbol(m_barcodeType);
	bc.setBorderType((Zint::QZint::BorderType)m_frameType);
	bc.setHeight(m_barcodeHeight);
	bc.setWidth(m_barcodeWidth);
	bc.setBorderWidth(m_barcodeBoderWidth);
	bc.setPdf417CodeWords(m_pdf417Max);
	bc.setFgColor(m_barcodeForegroundColor);
	bc.setBgColor(m_barcodeBackgroundColor);
	bc.setText(m_text);
	bc.setInputMode(m_inputMode);
	
	switch(m_barcodeType)
	{
		case  MSI_PLESSEY:
			bc.setWidth(m_msiPlessey);
			break;

		case BARCODE_PDF417:
			bc.setWidth(m_pdfDataColmns);
			bc.setSecurityLevel(m_pdf_ECC);
			bc.setPdf417CodeWords(m_pdf417Max);
			break;

		case BARCODE_AZTEC:
			if (!m_aztecResize)
				break;
			if (m_aztecResize<100)
				bc.setWidth(m_aztecResize);
			else
				bc.setSecurityLevel(m_aztecResize-100);
			break;

		case BARCODE_CODE39:
		case BARCODE_EXCODE39:
			bc.setWidth(m_code39CheckDigit);
			break;
			
		case BARCODE_HIBC_DM:
		case BARCODE_DATAMATRIX:
			bc.setSecurityLevel(m_dataMatrixEncodingMode);
			if (m_dataMatrixEncodingMode==ECC_200_RECOMMENDED)
				bc.setWidth(m_dataMatrixECC200Size);
			else
				bc.setWidth(m_dataMatrixNonECC200Size);
			break;

		case BARCODE_QRCODE:
			if (!m_qrResize)
				break;
			if (m_qrResize<100)
				bc.setWidth(m_qrResize);
			else
				bc.setSecurityLevel(m_qrResize-100);
			break;

		case BARCODE_MICROQR:
			if (!m_microQrResize)
				break;
			if (m_microQrResize<100)
				bc.setWidth(m_microQrResize);
			else
				bc.setSecurityLevel(m_microQrResize-100);
			break;
			
		case BARCODE_MAXICODE:
			bc.setSecurityLevel(m_maxiCodeEncodingMode);
			if (2==m_maxiCodeEncodingMode)
				bc.setPrimaryMessage(m_primaryMessage);
			break;

		case BARCODE_CHANNEL:
			bc.setWidth(m_barcodeChannelChannels);
			break;

		case BARCODE_CODEONE:
			bc.setWidth(m_codeoneSymbolSize);
			break;

		default:
			break;
	}

	bc.render(*painter,rect,(Zint::QZint::AspectRatioMode)m_paintType);

/*	switch (m_drawTextType)
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
	}*/
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
