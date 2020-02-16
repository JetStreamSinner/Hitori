#pragma once

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QBrush>

class HitoriRect : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit HitoriRect(QGraphicsItem* parent = nullptr);
    
    void mousePressEvent(QGraphicsSceneMouseEvent* e);

    QRectF boundingRect() const override ;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void setX(const int& x);
    void setY(const int& y);
    void setWidth(const int& width);
    void setHeight(const int& height);
    void setValue(const int& value);
    void setChecked(const bool state);
    void setIndex(const int& row_index, const int& column_index);

    int rowIndex() const;
    int columnIndex() const;
    int x() const;
    int y() const;
    int value() const;
    int width() const;
    int height() const;
    bool isChecked() const;


signals:
    void stateChanged(int row_index, int column_index);

private:
    int _row_index = 0;
    int _column_index = 0;
    
    int _x = 0;
    int _y = 0;
    int _width = 0;
    int _height = 0;
    int _value = 0;

    bool is_checked = false;

};