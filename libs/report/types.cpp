#include "types.h"

/// ====== Fill

Fill::Fill()
{
    m_texture = 0;
    m_textureImage = 0;
}

Fill::~Fill()
{
    if (m_texture)
	delete m_texture;
    if (m_textureImage)
	delete m_textureImage;
}

const bool & Fill::enable()
{
    return m_enable;
}

void Fill::setEnable(bool & b)
{
    m_enable = b;
}

void Fill::setColor(const QColor & color )
{
    m_color = color;
}

const QColor & Fill::color()
{
    return m_color;
}

void Fill::setPattern (Fill::PaternType & ptype)
{
    m_type = ptype;
}

const Fill::PaternType & Fill::pattern()
{
    return m_type;
}

void Fill::setTexture ( const QPixmap & pixmap )
{
    if (m_texture)
	delete m_texture;
    if (m_textureImage)
	delete m_textureImage;
    m_textureImage = 0;
    m_texture = pixmap;
}
void Fill::setTexture ( const QImage & image)
{

}
QPixmap Fill::texture () const;
QImage Fill::textureImage () const;

/// ========== Fill end
