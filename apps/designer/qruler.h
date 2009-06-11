/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Reginald Stadlbauer <reggie@kde.org>
   Copyright (C) 2006 Peter Simonsson <peter.simonsson@gmail.com>
   Copyright (C) 2007 Casper Boemann <cbr@boemann.dk>
   Copyright (C) 2007 Thomas Zander <zander@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

// Description: Ruler (header)

/******************************************************************/

#ifndef QRULER_H
#define QRULER_H

#include <QWidget>
#include <QTextOption>
#include "qunit.h"

class QPaintEvent;
class QRulerPrivate;
class QRuler;

/*
class RulerTabChooser : public QWidget {
public:
    RulerTabChooser(QWidget *parent) : QWidget(parent), m_type(QTextOption::LeftTab) {}
    ~RulerTabChooser() {}

    inline QTextOption::TabType type() {return m_type;}
    void mousePressEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *);

private:
    QTextOption::TabType m_type;
};
*/

class PaintingStrategy {
public:
    /// constructor
    PaintingStrategy() {}
    /// destructor
    ~PaintingStrategy() {}

    /**
     * Draw the background of the ruler.
     * @param ruler the ruler to draw on.
     * @param painter the painter we can paint with.
     */
    virtual QRectF drawBackground(const QRulerPrivate *ruler, QPainter &painter) = 0;

    /**
     * Draw the indicators for text-tabs.
     * @param ruler the ruler to draw on.
     * @param painter the painter we can paint with.
     */
    //virtual void drawTabs(const QRulerPrivate *ruler, QPainter &painter) = 0;

    /**
     * Draw the indicators for the measurements which typically are drawn every [unit].
     * @param ruler the ruler to draw on.
     * @param painter the painter we can paint with.
     * @param rectangle
     */
    virtual void drawMeasurements(const QRulerPrivate *ruler, QPainter &painter, const QRectF &rectangle) = 0;

    /**
     * Draw the indicators for the indents of a text paragraph
     * @param ruler the ruler to draw on.
     * @param painter the painter we can paint with.
     */
    virtual void drawIndents(const QRulerPrivate *ruler, QPainter &painter) = 0;

    /**
     *returns the size suggestion for a ruler with this strategy.
     */
    virtual QSize sizeHint(const QRulerPrivate *ruler) = 0;
};

class HorizontalPaintingStrategy : public PaintingStrategy {
public:
    HorizontalPaintingStrategy() : lengthInPixel(1) {}
    QRectF drawBackground(const QRulerPrivate *ruler, QPainter &painter);
    //void drawTabs(const QRulerPrivate *ruler, QPainter &painter);
    void drawMeasurements(const QRulerPrivate *ruler, QPainter &painter, const QRectF &rectangle);
    void drawIndents(const QRulerPrivate *ruler, QPainter &painter);
    QSize sizeHint(const QRulerPrivate *ruler);

private:
    qreal lengthInPixel;
};

class VerticalPaintingStrategy : public PaintingStrategy {
public:
    VerticalPaintingStrategy() : lengthInPixel(1) {}

    QRectF drawBackground(const QRulerPrivate *ruler, QPainter &painter);
    //void drawTabs(const QRulerPrivate *, QPainter &) {}
    void drawMeasurements(const QRulerPrivate *ruler, QPainter &painter, const QRectF &rectangle);
    void drawIndents(const QRulerPrivate *, QPainter &) { }
    QSize sizeHint(const QRulerPrivate *ruler);

private:
    qreal lengthInPixel;
};

class HorizontalDistancesPaintingStrategy : public HorizontalPaintingStrategy {
public:
    HorizontalDistancesPaintingStrategy() {}

    void drawMeasurements(const QRulerPrivate *ruler, QPainter &painter, const QRectF &rectangle);

private:
    void drawDistanceLine(const QRulerPrivate *d, QPainter &painter, const qreal start, const qreal end);
};

class QRulerPrivate {
public:
    QRulerPrivate(QRuler *parent, qreal scale, Qt::Orientation orientation);
    ~QRulerPrivate();

    void emitTabChanged();

    QUnit unit;
    const Qt::Orientation orientation;
    //const KoViewConverter * const viewConverter;

    int offset;
    qreal rulerLength;
    qreal activeRangeStart;
    qreal activeRangeEnd;

    int mouseCoordinate;
    int showMousePosition;

    bool showSelectionBorders;
    qreal firstSelectionBorder;
    qreal secondSelectionBorder;

    bool showIndents;
    qreal firstLineIndent;
    qreal paragraphIndent;
    qreal endIndent;

    //bool showTabs;
    //bool tabMoved; // set to true on first move of a selected tab
    //QList<QRuler::Tab> tabs;
    int originalIndex; //index of selected tab before we started dragging it.
    int currentIndex; //index of selected tab or selected HotSpot - only valid when selected indicates tab or hotspot
    //QRuler::Tab deletedTab;

    struct HotSpotData {
	qreal position;
	int id;
    };
    QList<HotSpotData> hotspots;

    bool rightToLeft;
    enum Selection {
	None,
	Tab,
	FirstLineIndent,
	ParagraphIndent,
	EndIndent,
	HotSpot
    };
    Selection selected;
    int selectOffset;

    QList<QAction*> popupActions;

    //RulerTabChooser *tabChooser;

    // Cached painting strategies
    PaintingStrategy * normalPaintingStrategy;
    PaintingStrategy * distancesPaintingStrategy;

    // Current painting strategy
    PaintingStrategy * paintingStrategy;

    QRuler *ruler;

    qreal numberStepForUnit() const;
    /// @return The rounding of value to the nearest multiple of stepValue
    qreal doSnapping(const qreal value) const;
    Selection selectionAtPosition(const QPoint & pos, int *selectOffset = 0);
    int hotSpotIndex(const QPoint & pos);

    QFont m_font;
    qreal m_scale;

    friend class VerticalPaintingStrategy;
    friend class HorizontalPaintingStrategy;
};
































/**
 * Decorator widget to draw a single ruler around a canvas.
 */
class QRuler : public QWidget
{
Q_OBJECT
public:
    /**
     * Creates a ruler with the orientation @p orientation
     * @param parent parent widget
     * @param orientation the orientation of the ruler
     * @param viewConverter the view converter used to convert from point to pixel
     */
    QRuler(QWidget* parent, Qt::Orientation orientation, qreal  scale = 1/*, const KoViewConverter* viewConverter*/);
    ~QRuler();

    /// For paragraphs each tab definition is represented by this struct.
    struct Tab {
        qreal position;    ///< distance in point from the start of the text-shape
        QTextOption::TabType type;       ///< Determine which type is used.
    };

    /// The ruler's unit
    QUnit unit() const;

    /// The length of the ruler in points (pt)
    qreal rulerLength() const;

    /// The orientation of the ruler
    Qt::Orientation orientation() const;

    /// The start indent of the first line
    qreal firstLineIndent() const;

    /// The start indent of the rest of the lines
    qreal paragraphIndent() const;

    /// The end indent of all lines
    qreal endIndent() const;

    /// The tab chooser widget, which you must put into a layout along with the ruler.
    /// Returns 0 for vertical rulers,
    QWidget *tabChooser();

    /**
     * set a list of actions that will be shown in a popup should the user right click on this ruler.
     * @param popupActionList the list of actions
     * @see popupActionList()
     */
    void setPopupActionList(const QList<QAction*> &popupActionList);
    /**
     * Return the actions list.
     * @see setPopupActionList()
     */
    QList<QAction*> popupActionList() const;

public slots:
    /// Set the unit of the ruler
    void setUnit(QUnit unit);

    /** Set the offset. Use this function to sync the ruler with
      * the canvas' position on screen
      * @param offset The offset in pixels
      */
    void setOffset(int offset);

    /// Sets the length of the ruler to @p length in points (pt)
    void setRulerLength(qreal length);

    /** Set the active range, ie the part of the ruler that is most likely used.
      * set to 0, 0 when there is no longer any active range
      * @param start the start of the range in pt
      * @param end the end of the range in pt
      */
    void setActiveRange(qreal start, qreal end);

    /** Set the state of the ruler so that it shows everything in right to left mode.
      * @param isRightToLeft state of right to left mode. Default is false.
      */
    void setRightToLeft(bool isRightToLeft);

    /** Set if the ruler should show indents as used in textditors.
      * Set the indents with setFirstLineIndent(), setParagraphIndent(), setEndIndent() .
      * @param show show indents if true. Default is false.
      */
    void setShowIndents(bool show);

    /** Set the position of the first line start indent relative to the active range.
      * If Right To left is set the indent is relative to the right side of the active range .
      * @param indent the value relative to the active range.
      */
    void setFirstLineIndent(qreal indent);

    /** Set the position of the rest of the lines start indent relative to the active range.
      * If Right To left is set the indent is relative to the right side of the active range .
      * @param indent the value relative to the active range.
      */
    void setParagraphIndent(qreal indent);

    /** Set the position of the end indent relative to the active range.
      * If Right To left is set the indent is relative to the left side of the active range .
      * @param indent the value relative to the active range.
      */
    void setEndIndent(qreal indent);

    /** Set whether the ruler should show the current mouse position.
      * Update the position with updateMouseCoordinate().
      * @param show show mouse position if true. Default is false.
      */
    void setShowMousePosition(bool show);

    /** Update the current position of the mouse pointer, repainting if changed.
      * The ruler offset will be applied before painting.
      * @param coordinate Either the x or y coordinate of the mouse depending
      *                   of the orientation of the ruler.
      */
    void updateMouseCoordinate(int coordinate);

    /**
     * Set whether the ruler should show the selection borders
     * @param show show selection borders if true, default is false.
     */
    void setShowSelectionBorders(bool show);

    /**
     * Update the selection borders
     * @param first the first selection border in points
     * @param second the other selection border in points
     */
    void updateSelectionBorders(qreal first, qreal second);

    /**
     * Set whether the ruler should show tabs
     * @param show show selection borders if true, default is false.
     */
//    void setShowTabs(bool show);

    /**
     * Update the tabs
     * @param tabs a list of tabs that is shown on the ruler the first selection border in points
     */
//    void updateTabs(const QList<Tab> &tabs);

    /***
     * Return the list of tabs set on this ruler.
     */
    //QList<Tab> tabs() const;

    /**
     * Clear all previously set hotspots.
     * A hotspot is a position on the ruler that the user can manipulate by dragging.
     */
    void clearHotSpots();

    /**
     * Add or set a hotspot.
     * A hotspot is a position on the ruler that the user can manipulate by dragging.
     * @param position the new position of the hotspot.
     * @param id the unique id for the hotspot. If the id has not been set before, it will be added.
     */
    void setHotSpot(qreal position, int id = -1);

    /**
     * Remove a previously set hotspot, returning true if one is actually returned.
     * @param id the unique id for the hotspot.
     * A hotspot is a position on the ruler that the user can manipulate by dragging.
     */
    bool removeHotSpot(int id);

    void setScale(qreal scale);

signals:
    /**
     * emitted when any of the indents is moved by the user.
     * @param final false until the user releases the mouse. So you can implement live update.
     */
    void indentsChanged(bool final);

    /**
     * Emitted when any of the tabs are moved, deleted or inserted by the user.
     * @param originalTabIndex the index in the list of tabs before the user interaction
     *          started, or -1 if this is a new tab
     * @param tab the new tab, or zero when the tab has been removed.
     */
    void tabChanged(int originalTabIndex, QRuler::Tab *tab);

    /// emitted when there the user is about to change a tab or hotspot
    void aboutToChange();

    void hotSpotChanged(int id, qreal newPosition);

    /// emitted when the user is dragging outside the ruler
    void guideLineCreated( Qt::Orientation orientation, int viewPosition );

protected:
    /// reimplemented
    virtual void paintEvent(QPaintEvent* event);
    /// reimplemented
    virtual void mousePressEvent ( QMouseEvent* ev );
    /// reimplemented
    virtual void mouseReleaseEvent ( QMouseEvent* ev );
    /// reimplemented
    virtual void mouseMoveEvent ( QMouseEvent* ev );
    /// reimplemented
    virtual QSize minimumSizeHint() const;
    /// reimplemented
    virtual QSize sizeHint() const;

private:
    QRulerPrivate * const d;
    friend class QRulerPrivate;
};

#endif
