/***************************************************************************
 *   This file is part of the eXaro project                                *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        * 
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      * 
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QKeySequence>
#include <QMetaProperty>
#include <QBuffer>
#include <QRectF>
#include <QPixmap>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QFont>
#include <QDebug>

#include "globals.h"

namespace Report
{

QDomElement variatToDom(QDomDocument * doc, const QString & name, const QVariant & value)
{
	if (!doc)
		return QDomElement();

	QDomElement dom = doc->createElement(name);
	dom.setAttribute("type", value.typeName());
	if (QString("QPainterPath")==value.typeName())
	{
			/*	<tag_name type="QPainterPath">
					<moveTo>x,y</moveTo>
					<lineTo>x,y</lineTo>
					<curveTo>x,y</curveTo>
					<curveToData>x,y</curveToData>
					<curveToData>x,y</curveToData>
					...
				</tag_name>
			*/
		QPainterPath p=value.value<QPainterPath>();
		for (int i=0;i<p.elementCount();i++)
		{
			QDomElement el;
			switch(p.elementAt(i).type)
			{
				case QPainterPath::MoveToElement:
					el= doc->createElement("moveTo");
					break;
				case QPainterPath::LineToElement:
					el= doc->createElement("lineTo");
					break;
				case QPainterPath::CurveToElement:
					el= doc->createElement("curveTo");
					break;
				case QPainterPath::CurveToDataElement:
					el= doc->createElement("curveToData");
					break;
			}
			el.appendChild(doc->createTextNode(QString("%1,%2").arg(p.elementAt(i).x).arg(p.elementAt(i).y)));
			dom.appendChild(el);
		}
		return dom;
	}

	switch (value.type())
	{
		case QVariant::Int:	//<tag_name type="Int">string_value</tag_name>
		case QVariant::Double:	//<tag_name type="Double">string_value</tag_name>
		case QVariant::LongLong://<tag_name type="LongLong">string_value</tag_name>
		case QVariant::UInt:	//<tag_name type="UInt">string_value</tag_name>
		case QVariant::ULongLong://<tag_name type="ULongLong">string_value</tag_name>
		case QVariant::Bool:	//<tag_name type="Bool">string_value</tag_name>
		case QVariant::Char:	//<tag_name type="Char">string_value</tag_name>
		case QVariant::Date:	//<tag_name type="Date">string_value</tag_name>
		case QVariant::DateTime://<tag_name type="DateTime">string_value</tag_name>
		case QVariant::Font:	//<tag_name type="Font">string_value</tag_name>
		case QVariant::KeySequence://<tag_name type="KeySequence">string_value</tag_name>
		case QVariant::Time:	//<tag_name type="Time">string_value</tag_name>
		case QVariant::String:	//<tag_name type="Time">string_value</tag_name>
			dom.appendChild(doc->createTextNode(value.toString()));
			break;

		case QVariant::StringList:
			/*	<tag_name type="StringList">
					<string>value1</string>
					<string>value2</string>
					...
					<string>valuex</string>

				</tag_name>
			*/
			foreach(QString str, value.toStringList())
				dom.appendChild(variatToDom(doc, "string", str));
			break;

		case QVariant::ByteArray://<tag_name type="ByteArray">string_value</tag_name>
			dom.appendChild(doc->createTextNode(value.toByteArray().toBase64()));
			break;

		case QVariant::Color:	//<tag_name type="Color">red,green,blue,alpha</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.value<QColor>().red()).arg(value.value<QColor>().green()).arg(value.value<QColor>().blue()).arg(value.value<QColor>().alpha())));
			break;

		case QVariant::Rect:	//<tag_name type="Rect">x,y,w,h</tag_name>
		case QVariant::RectF:	//<tag_name type="RectF">x,y,w,h</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.toRectF().x()).arg(value.toRectF().y()).arg(value.toRectF().width()).arg(value.toRectF().height())));
			break;

		case QVariant::Transform:	//<tag_name type="Rect">m11,m12,m13,m21,m22,m23,m31,m32,m33</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(value.value<QTransform>().m11()).arg(value.value<QTransform>().m12()).arg(value.value<QTransform>().m13()).arg(value.value<QTransform>().m21()).arg(value.value<QTransform>().m22()).arg(value.value<QTransform>().m23()).arg(value.value<QTransform>().m31()).arg(value.value<QTransform>().m32()).arg(value.value<QTransform>().m33())));
			break;

		case QVariant::Line:	//<tag_name type="Rect">x1,y1,x2,y2</tag_name>
		case QVariant::LineF:	//<tag_name type="RectF">x1,y1,x2,y2</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.toLineF().x1()).arg(value.toLineF().y1()).arg(value.toLineF().x2()).arg(value.toLineF().y2())));
			break;

		case QVariant::Size:	//<tag_name type="Size">w,h</tag_name>
		case QVariant::SizeF:	//<tag_name type="SizeF">w,h</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2").arg(value.toSizeF().width()).arg(value.toSizeF().height())));
			break;

		case QVariant::Point:	//<tag_name type="Point">x,y</tag_name>
		case QVariant::PointF:	//<tag_name type="PointF">x,y</tag_name>
			dom.appendChild(doc->createTextNode(QString("%1,%2").arg(value.toPointF().x()).arg(value.toPointF().y())));
			break;

		case QVariant::Pixmap:	//<tag_name type="Pixmap">base64png</tag_name>
		{
			QByteArray data;
			QBuffer buffer(&data);
			buffer.open(QBuffer::ReadWrite);
			value.value<QPixmap>().toImage().save(&buffer, "PNG");
			buffer.close();
			dom.appendChild(doc->createTextNode(data.toBase64()));
		}
		break;

		case QVariant::Image:	//<tag_name type="Pixmap">base64png</tag_name>
			return variatToDom(doc, name, QPixmap::fromImage(value.value<QImage>()));
			break;

		case QVariant::Brush:
			/*
			<tag_name type="Brush">
				<color>color_value</color>
				<style>style_value</style>
				<texture type="Pixmap" fromat="QImage::Format">base64image</texture>
				<gradient>
					<spread>spread_value</spread>
					<coordinateMode>coordinatemode_value</coordinateMode>
					<type>type_value</type>

					<!-- QConicalGradient --!>
					<angle>angle_value</angle>
					<center>x,y</center>
					<!-- QConicalGradient --!>

					<!-- QRadialGradient --!>
					<center>x,y</center>
					<radius>radius_value</radius>
					<focalPoint>x,y</focalPoint>
					<!-- QRadialGradient --!>

					<!-- QLinearGradient --!>
					<start>x,y</start>
					<final>x,y</final>
					<!-- QLinearGradient --!>
					<stops>
						<stop>
							<point>point_value</point>
							<color>color_value</color>
						</stop>
						.....
						<stop>
							<point>point_value</point>
							<color>color_value</color>
						</stop>
					</stops>
				</gradient>
			</tag_name>
			*/
			dom.appendChild(variatToDom(doc, "color", value.value<QBrush>().color()));
			dom.appendChild(variatToDom(doc, "style", value.value<QBrush>().style()));
			dom.appendChild(variatToDom(doc, "texture", value.value<QBrush>().texture()));
			if (value.value<QBrush>().gradient() && value.value<QBrush>().gradient()->type()!=QGradient::NoGradient)
			{
				QDomElement gel=doc->createElement("gradient");
				gel.appendChild(variatToDom(doc, "spread", value.value<QBrush>().gradient()->spread()));
				gel.appendChild(variatToDom(doc, "coordinateMode", value.value<QBrush>().gradient()->coordinateMode()));
				gel.appendChild(variatToDom(doc, "type", value.value<QBrush>().gradient()->type()));
				switch(value.value<QBrush>().gradient()->type())
				{
					case QGradient::LinearGradient:
						gel.appendChild(variatToDom(doc, "start",reinterpret_cast<const QLinearGradient*>(value.value<QBrush>().gradient())->start()));
						gel.appendChild(variatToDom(doc, "final",reinterpret_cast<const QLinearGradient*>(value.value<QBrush>().gradient())->finalStop()));
						break;
					case QGradient::RadialGradient:
						gel.appendChild(variatToDom(doc, "center",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->center()));
						gel.appendChild(variatToDom(doc, "radius",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->radius()));
						gel.appendChild(variatToDom(doc, "focalPoint",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->focalPoint()));
						break;
					case QGradient::ConicalGradient:
						gel.appendChild(variatToDom(doc, "angle",reinterpret_cast<const QConicalGradient*>(value.value<QBrush>().gradient())->angle()));
						gel.appendChild(variatToDom(doc, "center",reinterpret_cast<const QConicalGradient*>(value.value<QBrush>().gradient())->center()));
						break;
					default:
						break;
				}
				QDomElement stops=doc->createElement("stops");
				foreach(QGradientStop stop, value.value<QBrush>().gradient()->stops())
				{
					QDomElement stp=doc->createElement("stop");
					stp.appendChild(variatToDom(doc, "point",stop.first));
					stp.appendChild(variatToDom(doc, "color", stop.second));
					stops.appendChild(stp);
				}
				gel.appendChild(stops);
				dom.appendChild(gel);
			}
			break;

		case QVariant::Pen:
			/*
			<tag_name type="Pen">
				<brush type="Brush">
					<color>color_value</color>
					<style>style_value</style>
					<texture type="Pixmap" fromat="QImage::Format">base64image</texture>
				</brush>
				<capStyle>capStyle_value</capStyle>
				<color>color_value</color>
				<isCosmetic>isCosmetic_value</isCosmetic>
				<joinStyle>joinStyle_value</joinStyle>
				<miterLimit>miterLimit_value</miterLimit>
				<style>style_value</style>
				<width>width_value</width>
			</tag_name>
			*/
			dom.appendChild(variatToDom(doc, "brush", value.value<QPen>().brush()));
			dom.appendChild(variatToDom(doc, "capStyle", value.value<QPen>().capStyle()));
			dom.appendChild(variatToDom(doc, "color", value.value<QPen>().color()));
			dom.appendChild(variatToDom(doc, "isCosmetic", value.value<QPen>().isCosmetic()));
			dom.appendChild(variatToDom(doc, "joinStyle", value.value<QPen>().joinStyle()));
			dom.appendChild(variatToDom(doc, "miterLimit", value.value<QPen>().miterLimit()));
			dom.appendChild(variatToDom(doc, "style", value.value<QPen>().style()));
			dom.appendChild(variatToDom(doc, "width", value.value<QPen>().width()));
			break;

		case QVariant::Map:
			/*	<tag_name type="QVariantMap">
					<key_1>value</key_1>
					<key_2>value</key_2>
					...
					<key_x>value</key_x>

				</tag_name>
			*/
			foreach(QString key, value.toMap().uniqueKeys())
				foreach(QVariant val, value.toMap().values(key))
					dom.appendChild(variatToDom(doc, key, val));
			break;

		case QVariant::Region:
			/*	<tag_name type="QRegion">
					<rect>x,y,w,h</rect>
					<rect>x,y,w,h</rect>
					...
					<rect>x,y,w,h</rect>
				</tag_name>
			*/
			foreach(QRect r, value.value<QRegion>().rects())
				dom.appendChild(variatToDom(doc, "rect", r));
			break;

		default:
			qWarning() << "Type " << value.typeName() << " is not implemented. " << name << " will be empty";
			break;
	}
	return dom;
}

QVariant domToVariant(const QDomElement & dom)
{
	QVariant value;
	QStringList rect, size, point, strl;
	QBrush brush;
	QPen pen;
	QVariantMap map;
	QDomElement node;
	QRegion region;

	if (QString("QPainterPath")==dom.attribute("type", "").toAscii())
	{
		QPainterPath p;
		node = dom.firstChildElement();
		QPointF c1,c2;
		int cstate=0;
		for (;!node.isNull();node = node.nextSiblingElement())
		{
			point=node.text().split(',');
			if (node.tagName()=="moveTo")
				p.moveTo(point[0].toDouble(),point[1].toDouble());
			if (node.tagName()=="lineTo")
				p.lineTo(point[0].toDouble(),point[1].toDouble());
			if (node.tagName()=="curveTo")
			{
				c1.setX(point[0].toDouble());
				c1.setY(point[1].toDouble());
				cstate=1;
			}

			if (node.tagName()=="curveToData")
			{
				if (cstate==1)
				{
					c2.setX(point[0].toDouble());
					c2.setY(point[1].toDouble());
					cstate=2;
				}
				else
				{
					p.cubicTo(c1,c2,QPointF(point[0].toDouble(),point[1].toDouble()));
					cstate=0;
				}
			}

		}
		value.setValue(p);
		return value;
	}

	switch (QVariant::nameToType(dom.attribute("type", "").toAscii()))
	{
		case QVariant::Bool:	//<tag_name type="Bool">string_value</tag_name>
			value = QVariant(dom.text()).toBool();
			break;

		case QVariant::Int:	//<tag_name type="Int">string_value</tag_name>
		case QVariant::Char:	//<tag_name type="Char">string_value</tag_name>
			value = dom.text().toInt();
			break;

		case QVariant::Double:	//<tag_name type="Double">string_value</tag_name>
			value = dom.text().toDouble();
			break;

		case QVariant::LongLong://<tag_name type="LongLong">string_value</tag_name>
			value = dom.text().toLongLong();
			break;

		case QVariant::UInt:	//<tag_name type="UInt">string_value</tag_name>
		case QVariant::ULongLong://<tag_name type="ULongLong">string_value</tag_name>
			value = dom.text().toULongLong();
			break;

		case QVariant::Date:	//<tag_name type="Date">string_value</tag_name>
			value = QVariant(dom.text()).toDate();
			break;

		case QVariant::DateTime://<tag_name type="DateTime">string_value</tag_name>
			value = QVariant(dom.text()).toDateTime();
			break;

		case QVariant::Font:	//<tag_name type="Font">string_value</tag_name>
			value = QVariant(dom.text()).value<QFont>();
			break;

		case QVariant::KeySequence://<tag_name type="KeySequence">string_value</tag_name>
			value = QVariant(dom.text()).value<QKeySequence>();
			break;

		case QVariant::Time:	//<tag_name type="Time">string_value</tag_name>
			value = QVariant(dom.text()).toTime();
			break;

		case QVariant::String:	//<tag_name type="Time">string_value</tag_name>
			value = dom.text();
			break;

		case QVariant::ByteArray://<tag_name type="ByteArray">string_value</tag_name>
			value = QByteArray::fromBase64(dom.text().toAscii());
			break;

		case QVariant::Color:	//<tag_name type="Color">red,green,blue,alpha</tag_name>
			rect = dom.text().split(',');
			if (rect.size() == 4)
				value = QColor(rect[0].toInt(), rect[1].toInt(), rect[2].toInt(), rect[3].toInt());
			break;

		case QVariant::Rect:	//<tag_name type="Rect">x,y,w,h</tag_name>
		case QVariant::RectF:	//<tag_name type="RectF">x,y,w,h</tag_name>
			rect = dom.text().split(',');
			if (rect.size() == 4)
				value = QRectF(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble());
			break;

		case QVariant::Transform:	//<tag_name type="Rect">m11,m12,m21,m22,dx,dy</tag_name>
			rect = dom.text().split(',');
			if (rect.size() == 9)
				value = QTransform(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble(), rect[4].toDouble(), rect[5].toDouble(), rect[6].toDouble(), rect[7].toDouble(), rect[8].toDouble());
			break;

		case QVariant::Line:	//<tag_name type="Rect">x1,y1,x2,y2</tag_name>
		case QVariant::LineF:	//<tag_name type="RectF">x1,y1,x2,y2</tag_name>
			rect = dom.text().split(',');
			if (rect.size() == 4)
				value = QLineF(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble());
			break;

		case QVariant::Size:	//<tag_name type="Size">w,h</tag_name>
		case QVariant::SizeF:	//<tag_name type="SizeF">w,h</tag_name>
			size = dom.text().split(',');
			if (size.size() == 2)
				value = QSizeF(size[0].toDouble(), size[1].toDouble());
			break;

		case QVariant::Point:	//<tag_name type="Size">x,y</tag_name>
		case QVariant::PointF:	//<tag_name type="SizeF">x,y</tag_name>
			point = dom.text().split(',');
			if (point.size() == 2)
				value = QPointF(point[0].toDouble(), point[1].toDouble());
			break;

		case QVariant::Pixmap:	//<tag_name type="Pixmap">base64png</tag_name>
		{
			QByteArray data = QByteArray::fromBase64(dom.text().toAscii());
			QBuffer buffer(&data);
			buffer.open(QBuffer::ReadWrite);
			QImage img;
			img.load(&buffer, "PNG");
			buffer.close();
			value = QPixmap::fromImage(img);
		}
		break;

		case QVariant::Brush:
			/*
			<tag_name type="Brush">
				<color>color_value</color>
				<style>style_value</style>
				<texture type="Pixmap" fromat="QImage::Format">base64image</texture>
				<gradient>
					<spread>spread_value</spread>
					<coordinateMode>coordinatemode_value</coordinateMode>
					<type>type_value</type>

					<!-- QConicalGradient --!>
					<angle>angle_value</angle>
					<center>x,y</center>
					<!-- QConicalGradient --!>

					<!-- QRadialGradient --!>
					<center>x,y</center>
					<radius>radius_value</radius>
					<focalPoint>x,y</focalPoint>
					<!-- QRadialGradient --!>

					<!-- QLinearGradient --!>
					<start>x,y</start>
					<final>x,y</final>
					<!-- QLinearGradient --!>
					<stops>
						<stop>
							<point>point_value</point>
							<color>color_value</color>
						</stop>
						.....
						<stop>
							<point>point_value</point>
							<color>color_value</color>
						</stop>
					</stops>
				</gradient>
			</tag_name>
			*/
			brush.setTexture(domToVariant(dom.firstChildElement("texture")).value<QPixmap>());
			brush.setColor(domToVariant(dom.firstChildElement("color")).value<QColor>());
			if (!dom.firstChildElement("gradient").isNull())
			{
				QDomElement gel=dom.firstChildElement("gradient");
				QGradient *gr=0;
				switch(domToVariant(gel.firstChildElement("type")).toInt())
				{
					case QGradient::LinearGradient:
						gr = new QLinearGradient(domToVariant(gel.firstChildElement("start")).toPointF(), domToVariant(gel.firstChildElement("final")).toPointF());
						break;
					case QGradient::RadialGradient:
						gr = new QRadialGradient(domToVariant(gel.firstChildElement("center")).toPointF(), domToVariant(gel.firstChildElement("radius")).toDouble(), domToVariant(gel.firstChildElement("focalPoint")).toPointF());
						break;
					case QGradient::ConicalGradient:
						gr = new QConicalGradient(domToVariant(gel.firstChildElement("angle")).toPointF(), domToVariant(gel.firstChildElement("center")).toDouble());
						break;
				}
				gr->setCoordinateMode((QGradient::CoordinateMode)domToVariant(gel.firstChildElement("coordinateMode")).toInt());
				gr->setSpread((QGradient::Spread)domToVariant(gel.firstChildElement("spread")).toInt());
				QGradientStops gsps;
				node = gel.firstChildElement("stops").firstChildElement("stop");
				for (;!node.isNull();node = node.nextSiblingElement())
				{
					QGradientStop gsp;
					gsp.first=domToVariant(node.firstChildElement("point")).toDouble();
					gsp.second=domToVariant(node.firstChildElement("color")).value<QColor>();
					gsps.push_back(gsp);
				}
				gr->setStops(gsps);
				value = QBrush(*gr);
				delete gr;
			}
			else
			{
				brush.setStyle((Qt::BrushStyle)domToVariant(dom.firstChildElement("style")).toInt());
				value = brush;
			}
			break;

		case QVariant::Pen:
			/*
			<tag_name type="Pen">
				<brush type="Brush">
					<color>color_value</color>
					<style>style_value</style>
					<texture type="Pixmap" fromat="QImage::Format">base64image</texture>
				</brush>
				<capStyle>capStyle_value</capStyle>
				<color>color_value</color>
				<isCosmetic>isCosmetic_value</isCosmetic>
				<joinStyle>joinStyle_value</joinStyle>
				<miterLimit>miterLimit_value</miterLimit>
				<style>style_value</style>
				<width>width_value</width>
			</tag_name>
			*/
			pen.setBrush(domToVariant(dom.firstChildElement("brush")).value<QBrush>());
			pen.setCapStyle((Qt::PenCapStyle)domToVariant(dom.firstChildElement("capStyle")).toInt());
			pen.setColor(domToVariant(dom.firstChildElement("color")).value<QColor>());
			pen.setCosmetic(domToVariant(dom.firstChildElement("isCosmetic")).toBool());
			pen.setJoinStyle((Qt::PenJoinStyle)domToVariant(dom.firstChildElement("joinStyle")).toInt());
			pen.setMiterLimit(domToVariant(dom.firstChildElement("miterLimit")).toInt());
			pen.setStyle((Qt::PenStyle)domToVariant(dom.firstChildElement("style")).toInt());
			pen.setWidth(domToVariant(dom.firstChildElement("width")).toInt());
			value = pen;
			break;

		case QVariant::Map:
			/*	<tag_name type="QVariantMap">
					<key_1>value</key_1>
					<key_2>value</key_2>
					...
					<key_x>value</key_x>

				</tag_name>
			*/
			node = dom.firstChildElement();
			for (;!node.isNull();node = node.nextSiblingElement())
				map.insertMulti(node.tagName(), domToVariant(node));
			value = map;
			break;

		case QVariant::Region:
			/*	<tag_name type="QRegion">
					<rect>x,y,w,h</rect>
					<rect>x,y,w,h</rect>
					...
					<rect>x,y,w,h</rect>
				</tag_name>
			*/
			node = dom.firstChildElement();
			for (;!node.isNull();node = node.nextSiblingElement())
				region+=domToVariant(node).toRect();
			value=region;
			break;

		case QVariant::StringList:
			/*	<tag_name type="StringList">
					<string>value1</string>
					<string>value2</string>
					...
					<string>valuex</string>

				</tag_name>
			*/
			strl.clear();
			node = dom.firstChildElement();
			for (;!node.isNull();node = node.nextSiblingElement())
				strl.push_back(domToVariant(node).toString());
			value = strl;
			break;

		default:
			break;
	}
	return value;
}

bool typeIsNumber(QVariant::Type type)
{
	if (QVariant::Int == type || QVariant::LongLong == type || QVariant::UInt == type || QVariant::ULongLong == type || QVariant::Double == type)
		return true;
	return false;
}

}
