#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>
#include <QSize>

class Options
{
private:
    Options(){};

    QSettings s;
public:
    static QSize iconSize();

    //colors

    //etc
};

#endif // OPTIONS_H
