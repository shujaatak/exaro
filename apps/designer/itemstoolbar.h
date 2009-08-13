#ifndef ITEMSTOOLBAR_H
#define ITEMSTOOLBAR_H

#include <QToolBar>
#include "mainwindow.h"

class Menu;

class ItemsToolBar : public QToolBar
{
    Q_OBJECT
public:
    ItemsToolBar(mainWindow * mw);

private slots:
    void showItems();
private:
    mainWindow * m_mw;
    QHash<QString, Menu*> menus;
    Menu * m_menu;
};

class Menu : public QMenu
{
    Q_OBJECT
public:
    Menu(QWidget * parent = 0);
protected:
    void leaveEvent ( QEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent ( QDragLeaveEvent * /*event*/ );
};


#endif // ITEMSTOOLBAR_H
