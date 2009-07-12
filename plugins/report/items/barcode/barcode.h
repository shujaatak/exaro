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

	Q_ENUMS(BarcodeTypes PaintTypes FrameTypes DrawTextTypes MsiPlessey);
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
	Q_PROPERTY(MsiPlessey msiPlessey READ msiPlessey WRITE setMsiPlessey)
	Q_PROPERTY(bool exCode39_43Extension READ exCode39_43Extension WRITE setExCode39_43Extension)
	Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType)
	Q_PROPERTY(QString primaryMessage READ primaryMessage WRITE setPrimaryMessage)
	Q_PROPERTY(QString script READ script WRITE setScript)
	Q_PROPERTY(QString testText READ testText WRITE setTestText)

public:
	enum MsiPlessey
	{
		MSI_PLESSEY_ 	= 0,
		MSI_10		= 1,
		MSI_10_10	= 2,
		MSI_11		= 3,
		MSI_11_10	= 4
	};

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
		CODE11		=1,
		C25MATRIX	=2,
		C25INTER	=3,
		C25IATA		=4,
		C25LOGIC	=6,
		C25IND		=7,
		CODE39		=8,
		EXCODE39	=9,
		EANX		=13,
		EAN128		=16,
		CODABAR		=18,
		CODE128		=20,
		DPLEIT		=21,
		DPIDENT		=22,
		CODE16K		=23,
		CODE93		=25,
		FLAT		=28,
		RSS14		=29,
		RSS_LTD		=30,
		RSS_EXP		=31,
		TELEPEN		=32,
		UPCA		=34,
		UPCE		=37,
		POSTNET		=40,
		MSI_PLESSEY	=47,
		FIM		=49,
		LOGMARS		=50,
		PHARMA		=51,
		PZN		=52,
		PHARMA_TWO	=53,
		PDF417		=55,
		PDF417TRUNC	=56,
		MAXICODE	=57,
		QRCODE		=58,
		CODE128B	=60,
		AUSPOST		=63,
		AUSREPLY	=66,
		AUSROUTE	=67,
		AUSREDIRECT	=68,
		ISBNX		=69,
		RM4SCC		=70,
		DATAMATRIX	=71,
		ITF14		=72,
		CODABLOCKF	=74,
		NVE18		=75,
		KOREAPOST	=77,
		RSS14STACK	=79,
		RSS14STACK_OMNI	=80,
		RSS_EXPSTACK	=81,
		PLANET		=82,
		MICROPDF417	=84,
		ONECODE		=85,
		PLESSEY		=86,
		
		/* Tbarcode 8 codes */
		KIX		=90,
		AZTEC		=92,
		DAFT		=93,
		ITALYPOST	=94,
		DPD		=96,
		MICROQR		=97,
		
		/* Zint specific */
		TELEPEN_NUM	=128,
		CODE32		=129,
		EANX_CC		=130,
		EAN128_CC	=131,
		RSS14_CC	=132,
		RSS_LTD_CC	=133,
		RSS_EXP_CC	=134,
		UPCA_CC		=135,
		UPCE_CC		=136,
		RSS14STACK_CC	=137,
		RSS14_OMNI_CC	=138,
		RSS_EXPSTACK_CC	=139
	};

public:
	Barcode(QGraphicsItem* parent = 0, QObject* parentObject = 0);

	QRectF boundingRect() const;
	void _paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QRectF & rect, QWidget * widget = 0);

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

	MsiPlessey msiPlessey();
	void setMsiPlessey(MsiPlessey msiPlessey);

	bool exCode39_43Extension();
	void setExCode39_43Extension(bool exCode39_43Extension);

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
	bool	m_exCode39_43Extension;
	MsiPlessey m_msiPlessey;
};


#endif
