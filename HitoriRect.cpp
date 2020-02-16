#include <QRectF>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <QTextOption>
#include "HitoriRect.h"

HitoriRect::HitoriRect(QGraphicsItem* item) : QGraphicsItem(item)
{
}

void HitoriRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRectF rect(boundingRect());
    QString text = QString::number(_value);
    
    const QColor black(Qt::black);
    const QColor white(Qt::white);

    painter->setPen(black);
    painter->setBrush(!is_checked ? white : black);
    painter->drawRect(rect);

    painter->drawText(rect, text, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

QRectF HitoriRect::boundingRect() const
{
    return QRectF(_x, _y, _width, _height);
}

void HitoriRect::setIndex(const int& row_index, const int& column_index)
{
    _row_index = row_index;
    _column_index = column_index;
}

int HitoriRect::rowIndex() const
{
    return _row_index;
}

int HitoriRect::columnIndex() const
{
    return _column_index;
}

void HitoriRect::setValue(const int& value)
{
    _value = value;
}

void HitoriRect::setChecked(const bool state)
{
    is_checked = state;
    update();
}

bool HitoriRect::isChecked() const
{
    return is_checked;
}

int HitoriRect::value() const
{
    return _value;
}

void HitoriRect::setX(const int& x)
{
    _x = x;
}

void HitoriRect::setY(const int& y)
{
    _y = y;
}

int HitoriRect::x() const
{
    return _x;
}

int HitoriRect::y() const
{
    return _y;
}

void HitoriRect::setWidth(const int& width)
{
    _width = width;
}

void HitoriRect::setHeight(const int& height)
{
    _height = height;
}

int HitoriRect::width() const
{
    return _width;
}

int HitoriRect::height() const
{
    return _height;
}

void HitoriRect::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    is_checked = !is_checked;
    update();
    emit stateChanged(_row_index, _column_index);
}