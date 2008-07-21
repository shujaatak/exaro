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

#ifndef BARCODE_H
#define BARCODE_H

#include <iteminterface.h>


class Barcode : public Report::ItemInterface
{
	Q_OBJECT
	Q_INTERFACES(Report::ItemInterface);

	Q_ENUMS(BarcodeTypes PaintTypes FrameTypes DrawTextTypes);
	Q_PROPERTY(PaintTypes howToPaintBarcode READ paintType WRITE setPaintType)
	Q_PROPERTY(FrameTypes frameType READ frameType WRITE setFrameType)
	Q_PROPERTY(BarcodeTypes barcodeType READ barcodeType WRITE setBarcodeType)
	Q_PROPERTY(int barcodeHeight READ barcodeHeight WRITE setBarcodeHeight)
	Q_PROPERTY(int barcodeWhitespaceWidth READ barcodeWhitespaceWidth WRITE setBarcodeWhitespaceWidth)
	Q_PROPERTY(int barcodeBoderWidth READ barcodeBoderWidth WRITE setBarcodeBoderWidth)
	Q_PROPERTY(QColor barcodeForegroundColor READ barcodeForegroundColor WRITE setBarcodeForegroundColor)
	Q_PROPERTY(QColor barcodeBackgroundColor READ barcodeBackgroundColor WRITE setBarcodeBackgroundColor)
	Q_PROPERTY(int barcodeWidth READ barcodeWidth WRITE setBarcodeWidth)
	Q_PROPERTY(int barcodeSecurityLevel READ barcodeSecurityLevel WRITE setBarcodeSecurityLevel)
	Q_PROPERTY(int pdf417Max READ pdf417Max WRITE setPdf417Max)
	Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType)
	Q_PROPERTY(QString primaryMessage READ primaryMessage WRITE setPrimaryMessage)
	Q_PROPERTY(QString script READ script WRITE setScript)
	Q_PROPERTY(QString testText READ testText WRITE setTestText)

public:
	enum DrawTextTypes
	{
		NO_TEXT = 0,
		DRAW_TOP = 1,
		DRAW_BOTTOM = 2,
		DRAW_ABOVE = 3,
		DRAW_BELOW = 4
	};
	enum FrameTypes
	{
		NO_FRAME = 0,
		BARCODE_BIND = 1,
		BARCODE_BOX = 2
	};

	enum PaintTypes
	{
		IgnoreAspectRatio=0,
		KeepAspectRatio=1,
		CenterBarCode=2
	};

	enum BarcodeTypes
	{
		BARCODE_CODE11 = 1,
		BARCODE_C25MATRIX = 2,
		BARCODE_C25INTER = 3,
		BARCODE_C25IATA = 4,
		BARCODE_C25LOGIC = 6,
		BARCODE_C25IND = 7,
		BARCODE_CODE39 = 8,
		BARCODE_EXCODE39 = 9,
		BARCODE_EANX = 13,
		BARCODE_EAN128 = 16,
		BARCODE_CODABAR = 18,
		BARCODE_CODE128 = 20,
		BARCODE_DPLEIT = 21,
		BARCODE_DPIDENT = 22,
		BARCODE_CODE16K = 23,
		BARCODE_CODE93 = 25,
		BARCODE_FLAT = 28,
		BARCODE_RSS14 = 29,
		BARCODE_RSS_LTD = 30,
		BARCODE_RSS_EXP = 31,
		BARCODE_TELEPEN = 32,
		BARCODE_UPCA = 34,
		BARCODE_UPCE = 37,
		BARCODE_POSTNET = 40,
		BARCODE_MSI_PLESSEY = 47,
		BARCODE_FIM = 49,
		BARCODE_LOGMARS = 50,
		BARCODE_PHARMA = 51,
		BARCODE_PZN = 52,
		BARCODE_PHARMA_TWO = 53,
		BARCODE_PDF417 = 55,
		BARCODE_PDF417TRUNC = 56,
		BARCODE_MAXICODE = 57,
		BARCODE_QRCODE = 58,
		BARCODE_CODE128B = 60,
		BARCODE_AUSPOST = 63,
		BARCODE_AUSREPLY = 66,
		BARCODE_AUSROUTE = 67,
		BARCODE_AUSREDIRECT = 68,
		BARCODE_ISBNX = 69,
		BARCODE_RM4SCC = 70,
		BARCODE_DATAMATRIX = 71,
		BARCODE_ITF14 = 72,
		BARCODE_NVE18 = 75,
		BARCODE_KOREAPOST = 77,
		BARCODE_RSS14STACK = 79,
		BARCODE_RSS14STACK_OMNI = 80,
		BARCODE_RSS_EXPSTACK = 81,
		BARCODE_PLANET = 82,
		BARCODE_MICROPDF417 = 84,
		BARCODE_ONECODE = 85,
		BARCODE_PLESSEY = 86,
		BARCODE_TELEPEN_NUM = 100,
		BARCODE_MSI_10 = 101,
		BARCODE_MSI_10_10 = 102,
		BARCODE_MSI_11 = 103,
		BARCODE_MSI_11_10 = 104,
		BARCODE_CODE39_43 = 105,
		BARCODE_EXCODE39_43 = 106,
		BARCODE_AZTEC = 107,
		BARCODE_SUPER = 108,
		BARCODE_ULTRA = 109,
		BARCODE_EANX_CC = 110,
		BARCODE_EAN128_CC = 111,
		BARCODE_RSS14_CC = 112,
		BARCODE_RSS_LTD_CC = 113,
		BARCODE_RSS_EXP_CC = 114,
		BARCODE_UPCA_CC = 115,
		BARCODE_UPCE_CC = 116,
		BARCODE_RSS14STACK_CC = 117,
		BARCODE_RSS14_OMNI_CC = 118,
		BARCODE_RSS_EXPSTACK_CC = 119,
	};

public:
	Barcode(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	QIcon toolBoxIcon();
	QString toolBoxText();
	QString toolBoxGroup();

	BarcodeTypes barcodeType();
	void setBarcodeType(BarcodeTypes barcodeType);

	PaintTypes paintType();
	void setPaintType(PaintTypes paintType);

	FrameTypes frameType();
	void setFrameType(FrameTypes frameType);

	int barcodeHeight();
	void setBarcodeHeight(int barcodeHeight);

	int barcodeWhitespaceWidth();
	void setBarcodeWhitespaceWidth(int barcodeWhitespaceWidth);

	int barcodeBoderWidth();
	void setBarcodeBoderWidth(int barcodeBoderWidth);

	QColor barcodeForegroundColor();
	void setBarcodeForegroundColor(QColor barcodeForegroundColor);

	QColor barcodeBackgroundColor();
	void setBarcodeBackgroundColor(QColor barcodeBackgroundColor);

	int barcodeWidth();
	void setBarcodeWidth(int barcodeWidth);

	int barcodeSecurityLevel();
	void setBarcodeSecurityLevel(int barcodeSecurityLevel);

	int pdf417Max();
	void setPdf417Max(int pdf417Max);

	DrawTextTypes drawTextType();
	void setDrawTextType(DrawTextTypes drawTextType);

	const QString & primaryMessage();
	void setPrimaryMessage(const QString &primaryMessage);

	QString script();
	void setScript(const QString & script);

	QString testText();
	void setTestText(const QString & testText);

	QObject * createInstance(QGraphicsItem* parent = 0, QObject* parentObject = 0);

private:
	BarcodeTypes m_barcodeType;
	PaintTypes m_paintType;
	FrameTypes m_frameType;
	int m_barcodeHeight;
	int m_barcodeWhitespaceWidth;
	int m_barcodeBoderWidth;
	QColor m_barcodeForegroundColor;
	QColor m_barcodeBackgroundColor;
	int m_barcodeWidth;
	int m_barcodeSecurityLevel;
	int m_pdf417Max;
	DrawTextTypes m_drawTextType;
	QString m_testText;
	QString m_primaryMessage;
	QString m_script;
};


#endif
