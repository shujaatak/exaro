#ifndef TYPES_H
#define TYPES_H

#include <QObject>
#include <QBrush>

class Fill
{
    Fill();
    ~Fill();
public:
    enum FillType {Solid, Pattern, Gradient, Texture};
    enum PatternType {Dense1 = 2, Dense2 = 3, Dense3 = 4, Dense4 = 5, Dense5 = 6, Dense6 = 7, Dense7 = 8,
		     Hor = 9, Ver = 10, Cross = 11, BDiag = 12, FDiag = 13, DiagCross = 14};
    enum GradientType {Linear = 15, Conical = 17, Radial = 16};

    const bool & enable();
    void setEnable(bool & b);

    void setColor(const QColor & color );
    const QColor & color();

    void setPattern (Fill::PaternType & ptype);
    const Fill::PaternType & pattern();

    void setTexture ( const QPixmap & pixmap );
    void setTexture ( const QImage & image);
    QPixmap texture () const;
    QImage textureImage () const;

private:
    bool m_enable;
    QColor m_color;
    FillType m_type;
    PaternType m_patternType;
    GradientType m_gradientType;
    QPixmap * m_texture;
    QImage * m_textureImage;
}


Q_DECLARE_METATYPE(Fill)
#endif // TYPES_H
