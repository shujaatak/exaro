#ifndef BANDTITLE_H
#define BANDTITLE_H

#include <QGraphicsItem>
#include <QString>
#include <bandinterface.h>

namespace Report
{

class BandTitle : public QGraphicsItem
{
    public:
	BandTitle( BandInterface * parentBand, const QSizeF & size, const QString & text, QGraphicsItem * parent=0 );

	void setSize(QSizeF size);
	inline const QSizeF & size(){return m_size;};
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void setCaption(QString str);
	QString caption();
    protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
	QSizeF m_size;
	QString m_text;
	BandInterface * m_parentBand;
};

}

#endif // BANDTITLE_H
