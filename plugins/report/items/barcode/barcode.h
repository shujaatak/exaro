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

	Q_ENUMS(BarcodeTypes PaintTypes FrameTypes /*DrawTextTypes*/ MsiPlessey AztecResize InputMode Pdf_ECC MaxiCodeEncodingMode BarcodeChannelChannels CodeoneSymbolSize QrResize MicroQrResize DataMatrixEncodingMode DataMatrixECC200Size DataMatrixNonECC200Size);
	Q_PROPERTY(PaintTypes howToPaintBarcode READ paintType WRITE setPaintType)
	Q_PROPERTY(FrameTypes frameType READ frameType WRITE setFrameType)
	Q_PROPERTY(BarcodeTypes barcodeType READ barcodeType WRITE setBarcodeType)
	Q_PROPERTY(int barcodeHeight READ barcodeHeight WRITE setBarcodeHeight)
	Q_PROPERTY(int barcodeWhitespaceWidth READ barcodeWhitespaceWidth WRITE setBarcodeWhitespaceWidth)
	Q_PROPERTY(int barcodeBoderWidth READ barcodeBoderWidth WRITE setBarcodeBoderWidth)
	Q_PROPERTY(QColor barcodeForegroundColor READ barcodeForegroundColor WRITE setBarcodeForegroundColor)
	Q_PROPERTY(QColor barcodeBackgroundColor READ barcodeBackgroundColor WRITE setBarcodeBackgroundColor)
	Q_PROPERTY(InputMode inputMode READ inputMode WRITE setInputMode)
	Q_PROPERTY(int barcodeWidth READ barcodeWidth WRITE setBarcodeWidth)
//	Q_PROPERTY(DrawTextTypes drawTextType READ drawTextType WRITE setDrawTextType)
	Q_PROPERTY(QString script READ script WRITE setScript)
	Q_PROPERTY(QString testText READ testText WRITE setTestText)
	
	Q_PROPERTY(int pdfDataColmns READ pdfDataColmns WRITE setPdfDataColmns)
	Q_PROPERTY(int pdfCodewords READ pdf417Max WRITE setPdf417Max)
	Q_PROPERTY(Pdf_ECC pdf_ECC READ pdf_ECC WRITE setPdf_ECC)
	
	Q_PROPERTY(MsiPlessey msiPlessey READ msiPlessey WRITE setMsiPlessey)

	Q_PROPERTY(AztecResize aztecResize READ aztecResize WRITE setAztecResize)

	Q_PROPERTY(Code39CheckDigit code39CheckDigit READ code39CheckDigit WRITE setCode39CheckDigit)

	Q_PROPERTY(MaxiCodeEncodingMode maxiCodeEncodingMode READ maxiCodeEncodingMode WRITE setMaxiCodeEncodingMode)
	Q_PROPERTY(QString primaryMessage READ primaryMessage WRITE setPrimaryMessage)

	Q_PROPERTY(BarcodeChannelChannels barcodeChannelChannels READ barcodeChannelChannels WRITE setBarcodeChannelChannels)

	Q_PROPERTY(CodeoneSymbolSize codeoneSymbolSize READ codeoneSymbolSize WRITE setCodeoneSymbolSize)

	Q_PROPERTY(QrResize qrResize READ qrResize WRITE setQrResize)

	Q_PROPERTY(MicroQrResize microQrResize READ microQrResize WRITE setMicroQrResize)

	Q_PROPERTY(DataMatrixEncodingMode dataMatrixEncodingMode READ dataMatrixEncodingMode WRITE setDataMatrixEncodingMode)
	Q_PROPERTY(DataMatrixNonECC200Size dataMatrixNonECC200Size READ dataMatrixNonECC200Size WRITE setDataMatrixNonECC200Size)
	Q_PROPERTY(DataMatrixECC200Size dataMatrixECC200Size READ dataMatrixECC200Size WRITE setDataMatrixECC200Size)
	
	
public:
	enum DataMatrixNonECC200Size
	{
		NONECC200SIZE_AUTOMATIC,
		NONECC200SIZE_9x9,
		NONECC200SIZE_11x11,
		NONECC200SIZE_13x13,
		NONECC200SIZE_15x15,
		NONECC200SIZE_17x17,
		NONECC200SIZE_19x19,
		NONECC200SIZE_21x21,
		NONECC200SIZE_23x23,
		NONECC200SIZE_25x25,
		NONECC200SIZE_27x27,
		NONECC200SIZE_29x29,
		NONECC200SIZE_31x31,
		NONECC200SIZE_33x33,
		NONECC200SIZE_35x35,
		NONECC200SIZE_37x37,
		NONECC200SIZE_39x39,
		NONECC200SIZE_41x41,
		NONECC200SIZE_43x43,
		NONECC200SIZE_45x45,
		NONECC200SIZE_47x47,
		NONECC200SIZE_49x49
	};

	enum DataMatrixECC200Size
	{
		ECC200SIZE_AUTOMATIC,
		ECC200SIZE_10x10,
		ECC200SIZE_12x12,
		ECC200SIZE_14x14,
		ECC200SIZE_16x16,
		ECC200SIZE_18x18,
		ECC200SIZE_20x20,
		ECC200SIZE_22x22,
		ECC200SIZE_24x24,
		ECC200SIZE_26x26,
		ECC200SIZE_32x32,
		ECC200SIZE_36x36,
		ECC200SIZE_40x40,
		ECC200SIZE_44x44,
		ECC200SIZE_48x48,
		ECC200SIZE_52x52,
		ECC200SIZE_64x64,
		ECC200SIZE_72x72,
		ECC200SIZE_80x80,
		ECC200SIZE_88x88,
		ECC200SIZE_96x96,
		ECC200SIZE_104x104,
		ECC200SIZE_120x120,
		ECC200SIZE_132x132,
		ECC200SIZE_144x144,
		ECC200SIZE_8x18,
		ECC200SIZE_8x32,
		ECC200SIZE_12x26,
		ECC200SIZE_12x36,
		ECC200SIZE_16x36,
		ECC200SIZE_16x48
	};
	
	enum DataMatrixEncodingMode
	{
		ECC_200_RECOMMENDED	=1,
		ECC_000			=2,
		ECC_050			=3,
		ECC_080			=4,
		ECC_100			=5,
		ECC_140			=6
	};
	
	enum QrResize
	{
		QR_AUTO_RESIZE		=0,
		VERSION_1_21X21		=1,
		VERSION_2_25X25		=2,
		VERSION_3_29X29		=3,
		VERSION_4_33X33		=4,
		VERSION_5_37X37		=5,
		VERSION_6_41X41		=6,
		VERSION_7_45X45		=7,
		VERSION_8_49X49		=8,
		VERSION_9_53X53		=9,
		VERSION_10_57X57	=10,
		VERSION_11_61X61	=11,
		VERSION_12_65X65	=12,
		VERSION_13_69X69	=13,
		VERSION_14_73X73	=14,
		VERSION_15_77X77	=15,
		VERSION_16_81X81	=16,
		VERSION_17_85X85	=17,
		VERSION_18_89X89	=18,
		VERSION_19_93X93	=19,
		VERSION_20_97X97	=20,
		VERSION_21_101X101	=21,
		VERSION_22_105X105	=22,
		VERSION_23_109X109	=23,
		VERSION_24_113X113	=24,
		VERSION_25_117X117	=25,
		VERSION_26_121X121	=26,
		VERSION_27_125X125	=27,
		VERSION_28_129X129	=28,
		VERSION_29_133X133	=29,
		VERSION_30_137X137	=30,
		VERSION_31_141X141	=31,
		VERSION_32_145X145	=32,
		VERSION_33_149X149	=33,
		VERSION_34_153X153	=34,
		VERSION_35_157X157	=35,
		VERSION_36_161X161	=36,
		VERSION_37_165X165	=37,
		VERSION_38_169X169	=38,
		VERSION_39_173X173	=39,
		VERSION_40_177X177	=40,
		LEVEL_L_20		=101,
		LEVEL_M_37		=102,
		LEVEL_Q_55		=103,
		LEVEL_H_65		=104
	};
	
	enum MicroQrResize
	{
		MQR_AUTO_RESIZE	=0,
		VERSION_M1_11X11=1,
		VERSION_M2_13X13=2,
		VERSION_M3_15X15=3,
		VERSION_M4_17X17=4,
		MQR_LEVEL_L_20	=101,
		MQR_LEVEL_M_37	=102,
		MQR_LEVEL_Q_55	=103
	};
	
	enum CodeoneSymbolSize
	{
		AUTOMATIC_SYMBOL_SIZE	=0,
		VERSION_A_16X18		=1,
		VERSION_B_22X22		=2,
		VERSION_C_28X32		=3,
		VERSION_D_40X42		=4,
		VERSION_E_52X54		=5,
		VERSION_F_70X76		=6,
		VERSION_G_104X98	=7,
		VERSION_H_148X134	=8,
		VERSION_S_8X_HEIGHT	=9,
		VERSION_T_16X_HEIGHT	=10
	};
	
	enum BarcodeChannelChannels
	{
		AUTO	=0,
		CH_3	=3,
		CH_4	=4,
		CH_5	=5,
		CH_6	=6,
		CH_7	=7,
		CH_8	=8
	};
	
	enum MaxiCodeEncodingMode
	{
		MODE_2_OR_3	=2,
		MODE_4		=4,
		MODE_5		=5,
		MODE_6		=6
	};
	
	enum Code39CheckDigit
	{
		NO_CHECK_DIGIT		=0,
		MOD_43__CHECK_DIGIT	=1
	};
	
	enum InputMode
	{
		DATA	=0,
		UNICODE =1,
		GS1	=2,
		KANJI	=3,
		SJIS	=4
	};
	
	enum Pdf_ECC
	{
		ECC_AUTO	=-1,
		ECC_2_WORDS	=0,
		ECC_4_WORDS	=1,
		ECC_8_WORDS	=2,
		ECC_16_WORDS	=3,
		ECC_32_WORDS	=4,
		ECC_64_WORDS	=5,
		ECC_128_WORDS	=6,
		ECC_256_WORDS	=7,
		ECC_512_WORDS	=8
	};
	
	enum AztecResize
	{
		AUTOMATIC_RESIZE	=0,
		ADJUST_SIZE_TO_15x15_COMPACT	=1,
		ADJUST_SIZE_TO_19x19_COMPACT	=2,
		ADJUST_SIZE_TO_23x23_COMPACT	=3,
		ADJUST_SIZE_TO_27x27_COMPACT	=4,
		ADJUST_SIZE_TO_19x19	=5,
		ADJUST_SIZE_TO_23x23	=6,
		ADJUST_SIZE_TO_27x27	=7,
		ADJUST_SIZE_TO_31x31	=8,
		ADJUST_SIZE_TO_37x37	=9,
		ADJUST_SIZE_TO_41x41	=10,
		ADJUST_SIZE_TO_45x45	=11,
		ADJUST_SIZE_TO_49x49	=12,
		ADJUST_SIZE_TO_53x53	=13,
		ADJUST_SIZE_TO_57x57	=14,
		ADJUST_SIZE_TO_61x61	=15,
		ADJUST_SIZE_TO_67x67	=16,
		ADJUST_SIZE_TO_71x71	=17,
		ADJUST_SIZE_TO_75x75	=18,
		ADJUST_SIZE_TO_79x79	=19,
		ADJUST_SIZE_TO_83x83	=20,
		ADJUST_SIZE_TO_87x87	=21,
		ADJUST_SIZE_TO_91x91	=22,
		ADJUST_SIZE_TO_95x95	=23,
		ADJUST_SIZE_TO_101x101	=24,
		ADJUST_SIZE_TO_105x105	=25,
		ADJUST_SIZE_TO_109x109	=26,
		ADJUST_SIZE_TO_113x113	=27,
		ADJUST_SIZE_TO_117x117	=28,
		ADJUST_SIZE_TO_121x121	=29,
		ADJUST_SIZE_TO_125x125	=30,
		ADJUST_SIZE_TO_131x131	=31,
		ADJUST_SIZE_TO_135x135	=32,
		ADJUST_SIZE_TO_139x139	=33,
		ADJUST_SIZE_TO_143x143	=34,
		ADJUST_SIZE_TO_147x147	=35,
		ADJUST_SIZE_TO_151x151	=36,
		ECC_10_3_words	=101,
		ECC_23_3_words	=102,
		ECC_36_3_words	=103,
		ECC_50_3_words	=104
	};
	
	enum MsiPlessey
	{
		MSI_PLESSEY_ 	= 0,
		MSI_10		= 1,
		MSI_10_10	= 2,
		MSI_11		= 3,
		MSI_11_10	= 4
	};
/*
	enum DrawTextTypes
	{
		NO_TEXT = 0,
		DRAW_TOP = 1,
		DRAW_BOTTOM = 2,
		DRAW_ABOVE = 3,
		DRAW_BELOW = 4
	};
*/
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
		AUSPOST		=63,
		AUSREPLY	=66,
		AUSROUTE	=67,
		AUSREDIRECT	=68,
		AZTEC		=92,
		AZRUNE		=128,
		CHANNEL		=140,
		CODABAR		=18,
		CODABLOCKF	=74,
		HIBC_BLOCKF	=110,
		CODEONE		=141,
		CODE11		=1,
		CODE128		=20,
		CODE128B	=60,
		HIBC_128	=98,
		CODE16K		=23,
		C25LOGIC	=6,
		C25IATA		=4,
		C25IND		=7,
		C25INTER	=3,
		C25MATRIX	=2,
		CODE32		=129,
		CODE39		=8,
		EXCODE39	=9,
		HIBC_39		=99,
		CODE49		=24,
		CODE93		=25,
		CODE_ONE	=141,
		RSS14		=29,
		RSS14_CC	=132,
		RSS_EXP		=31,
		RSS_EXP_CC	=134,
		RSS_EXPSTACK	=81,
		RSS_EXPSTACK_CC	=139,
		RSS_LTD		=30,
		RSS_LTD_CC	=133,
		RSS14STACK	=79,
		RSS14STACK_CC	=137,
		RSS14STACK_OMNI	=80,
		RSS14_OMNI_CC	=138,
		DATAMATRIX	=71,
		DPIDENT		=22,
		DPLEIT		=21,
		KIX		=90,
		EAN128		=16,
		EAN128_CC	=131,
		EAN14		=72,
		EANX		=13,
		EANX_CC		=130,
		FIM		=49,
		FLAT		=28,
		ITF14		=89,
		ISBNX		=69,
		JAPANPOST	=76,
		KOREAPOST	=77,
		LOGMARS		=50,
		MAXICODE	=57,
		MSI_PLESSEY	=47,
		NVE18		=75,
		PDF417		=55,
		PDF417TRUNC	=56,
		HIBC_PDF	=106,
		MICROPDF417	=84,
		HIBC_MICPDF	=108,
		PHARMA		=51,
		PHARMA_TWO	=53,
		PZN		=52,
		PLANET		=82,
		POSTNET		=40,
		QRCODE		=58,
		MICROQR		=97,
		HIBC_QR		=104,
		RM4SCC		=70,
		TELEPEN		=32,
		TELEPEN_NUM	=87,
		PLESSEY		=86,
		UPCA		=34,
		UPCA_CC		=135,
		UPCE		=37,
		UPCE_CC		=136,
		ONECODE		=85,
		DAFT		=93,
		HIBC_DM		=102
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

	InputMode inputMode();
	void setInputMode(InputMode inputMode);

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

	int pdf417Max();
	void setPdf417Max(int pdf417Max);

	int pdfDataColmns();
	void setPdfDataColmns(int pdfDataColmns);

	Pdf_ECC pdf_ECC();
	void setPdf_ECC(Pdf_ECC pdf_ECC);
	
	MsiPlessey msiPlessey();
	void setMsiPlessey(MsiPlessey msiPlessey);
	
	AztecResize aztecResize();
	void setAztecResize(AztecResize aztecResize);

	Code39CheckDigit code39CheckDigit();
	void setCode39CheckDigit(Code39CheckDigit code39CheckDigit);

// 	DrawTextTypes drawTextType();
// 	void setDrawTextType(DrawTextTypes drawTextType);

	const QString & primaryMessage();
	void setPrimaryMessage(const QString &primaryMessage);

	MaxiCodeEncodingMode maxiCodeEncodingMode();
	void setMaxiCodeEncodingMode(MaxiCodeEncodingMode maxiCodeEncodingMode);

	BarcodeChannelChannels barcodeChannelChannels();
	void setBarcodeChannelChannels(BarcodeChannelChannels barcodeChannelChannels);

	CodeoneSymbolSize codeoneSymbolSize();
	void setCodeoneSymbolSize(CodeoneSymbolSize codeoneSymbolSize);

	MicroQrResize microQrResize();
	void setMicroQrResize(MicroQrResize microQrResize);

	QrResize qrResize();
	void setQrResize(QrResize qrResize);

	DataMatrixEncodingMode dataMatrixEncodingMode();
	void setDataMatrixEncodingMode(DataMatrixEncodingMode dataMatrixEncodingMode);
	
	DataMatrixNonECC200Size dataMatrixNonECC200Size();
	void setDataMatrixNonECC200Size(DataMatrixNonECC200Size dataMatrixNonECC200Size);
	
	DataMatrixECC200Size dataMatrixECC200Size();
	void setDataMatrixECC200Size(DataMatrixECC200Size dataMatrixECC200Size);
	
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
	int m_pdf417Max;
/*	DrawTextTypes m_drawTextType;*/
	QString m_testText;
	QString m_primaryMessage;
	QString m_script;
	MsiPlessey m_msiPlessey;
	AztecResize m_aztecResize;
	InputMode m_inputMode;
	int m_pdfDataColmns;
	Pdf_ECC	m_pdf_ECC;
	Code39CheckDigit m_code39CheckDigit;
	MaxiCodeEncodingMode m_maxiCodeEncodingMode;
	BarcodeChannelChannels m_barcodeChannelChannels;
	CodeoneSymbolSize m_codeoneSymbolSize;
	MicroQrResize m_microQrResize;
	QrResize m_qrResize;
	DataMatrixEncodingMode m_dataMatrixEncodingMode;
	DataMatrixNonECC200Size m_dataMatrixNonECC200Size;
	DataMatrixECC200Size m_dataMatrixECC200Size;
};


#endif
