#include "bandtitle.h"

namespace Report
{

BandTitle::BandTitle(BandInterface * parentBand, const QSizeF & size, const QString & text, QGraphicsItem * parent):
		QGraphicsItem(parent),m_parentBand(parentBand),m_size(size), m_text(text)
{
}

void BandTitle::setSize(QSizeF size)
{
	prepareGeometryChange ();
	if (m_size==size)
		return;
	m_size=size;
	update();
}

QRectF BandTitle::boundingRect() const
{
	return QRectF(0,0,m_size.width(), m_size.height());
}

void BandTitle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect=boundingRect();
//	qreal penwidth = painter->pen().widthF();
//	rect=rect.adjusted(penwidth,penwidth,-penwidth,-penwidth);
	painter->save();
	QFont f=painter->font();
	f.setPointSizeF(20);
	f.setBold(true);
	painter->setFont(f);
	painter->setPen(QColor(224,224,224));
	QBrush a;
//	a.setColor(QColor(194,255,240,50));
	a.setColor(QColor(224,224,224));
	a.setStyle(Qt::SolidPattern);
//	painter->fillRect(rect, QColor(0,0,40));
	QPainterPath p;
		p.moveTo(0, 40);
		p.lineTo(40, 0);
		p.lineTo(rect.width() - 40, 0);
		p.lineTo(rect.width(), rect.height());
		painter->drawPath(p);
		painter->fillPath(p,a);
//		painter->rotate(270);
		painter->setPen(QColor(50,50,50));
//		painter->drawText(rect, Qt::AlignCenter, m_text);
		painter->drawText(40,0, rect.width() - 40, rect.height(), Qt::AlignLeft, m_text);
	painter->restore();
}

void BandTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_parentBand->selectBand();
}

void BandTitle::setCaption(QString str)
{
    m_text = str;
    update();
}

QString BandTitle::caption()
{
    return m_text;
}

}
