/*
 * Graphic Library
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <M5Unified.h>

class Canvas
{
protected:
    uint16_t _ox, _oy;
    uint16_t _w, _h;
    uint16_t *_v;
public:
    Canvas(uint16_t x = 0, uint16_t y = 0, uint16_t w = 320, uint16_t h = 240);
    Canvas(const Canvas &x);
    ~Canvas();

    void cls();
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col);
    void pset(uint16_t x, uint16_t y, uint16_t col);
    uint16_t pget(uint16_t x, uint16_t y) const { return _v[x + y * _w]; }
    void paint(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc);
    void tonePaint(const uint8_t tone[], bool tiling = false);
};

class Point
{
public:
    uint16_t x, y;
    Point(uint16_t x_ = 0, uint16_t y_ = 0) : x(x_), y(y_) {}
    Point(const Point &p) : x(p.x), y(p.y) {}
};

class Queue
{
protected:
    Point *_fifo;
    int _size;
    int _head, _tail;
public:
    Queue(int size = 1024);
    Queue(const Queue &x);
    ~Queue();

    void push(const Point p);
    Point pop();
    bool empty() const { return _head == _tail;}
};
#endif /* GRAPH_H */