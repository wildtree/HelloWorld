/*
 * Graphics Library
 */

#include <graph.h>

Queue::Queue(int size)
    : _size(size), _head(0), _tail(0)
{
    _fifo = new Point [_size];
}

Queue::Queue(const Queue &x)
    : _size(x._size), _head(x._head), _tail(x._tail)
{
    _fifo = new Point [_size];
    for (int i = 0 ; i < _size ; i++) _fifo[i] = x._fifo[i]; 
}

Queue::~Queue()
{
    delete [] _fifo;
}

void
Queue::push(const Point p)
{
    _fifo[_tail++] = p;
    if (_tail == _size) _tail = 0;
}

Point
Queue::pop()
{
    Point p = _fifo[_head++];
    if (_head == _size) _head = 0;
    return p;
}

Canvas::Canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    : _ox(x), _oy(y), _w(w), _h(h)
{
    _v = new uint16_t [_w * _h];
    cls();
}

Canvas::Canvas(const Canvas &x)
    : _ox(x._ox), _oy(x._oy), _w(x._w), _h(x._h)
{
    _v = new uint16_t [_w * _h];
    cls();
}

Canvas::~Canvas()
{
    cls();
    delete [] _v;
}

void
Canvas::pset(uint16_t x, uint16_t y, uint16_t col)
 {
    if (x >= _w || y >= _h) return;
    M5.Lcd.drawPixel(_ox + x, _oy + y, col);
    _v[x + y * _w] = col;
}

void
Canvas::cls()
 { 
    M5.Lcd.fillRect(_ox, _oy, _w, _h, BLACK); 
    for (int i = 0 ; i < _w * _h ; i++) _v[i] = BLACK;
 }

void
Canvas::line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col)
{
    //M5.Lcd.drawLine(_ox + x0, _oy + y0, _ox + x1, _oy + y1, col);
    int dx, ddx, dy, ddy;
    int wx, wy;
    int x, y;
    dy = y1 - y0;
    ddy = 1;
    if (dy < 0)
    {
         dy = -dy;
         ddy = -1;
    }
    wy = dy / 2;
    dx = x1 - x0;
    ddx = 1;
    if (dx < 0)
    {
        dx = -dx;
        ddx = -1;
    }
    wx = dx / 2;
    if (dx == 0 && dy == 0)
    {
        pset(x0, y0, col);
    }
    if (dy == 0)
    {
        for (x = x0 ; x != x1 ; x += ddx) pset(x, y0, col);
        return;
    }
    if (dx == 0)
    {
        for (y = y0 ; y != y1 ; y += ddy) pset(x0, y, col);
        return;
    }

    pset(x0, y0, col);
    if (dx > dy)
    {
        y = y0;
        for (x = x0 ; x != x1 ; x += ddx)
        {
            pset(x, y, col);
            wx -= dy;
            if (wx < 0)
            {
                wx += dx;
                y += ddy;
            }
        }
    }
    else
    {
        x = x0;
        for (y = y0 ; y != y1 ; y += ddy)
        {
            pset(x, y, col);
            wy -= dx;
            if (wy < 0)
            {
                wy += dy;
                x += ddx;
            }
        }
    }
    pset(x1, y1, col);
}

void
Canvas::paint(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc)
{
    int l, r;
    int wx;
    uint16_t c;

    Queue q;    
    c = pget(x, y);
    if (c == fc || c == bc)
    {
        return;
    }
    q.push(Point(x,y));
    while (!q.empty())
    {
        Point p = q.pop();
        c = pget(p.x, p.y);
        if (c == fc || c == bc) continue;
        for (l = p.x - 1 ; l >= 0 ; l--)
        {
            c = pget(l, p.y);
            if (c == fc || c == bc) break;
        }
        ++l;
        for (r = p.x + 1 ; r < _w ; r++)
        {
            c = pget(r, p.y);
            if (c == fc || c == bc) break;
        }
        --r;
        line(l, p.y, r, p.y, fc);
        for (wx = l ; wx <= r ; wx++)
        {
            int uy = p.y - 1;
            if (uy >= 0)
            {
                c = pget(wx, uy);
                if (c != fc && c != bc)
                {
                    if (wx == r)
                    {
                        q.push(Point(wx, uy));
                    }
                    else
                    {
                        c = pget(wx + 1, uy);
                        if (c == fc || c == bc) q.push(Point(wx,uy));
                    }
                }
            }
            int ly = p.y + 1;
            if (ly < _h)
            {
                c = pget(wx, ly);
                if (c != fc && c != bc)
                {
                    if (wx == r)
                    {
                        q.push(Point(wx, ly));
                    }
                    else
                    {
                        c = pget(wx + 1, ly);
                        if (c == fc || c == bc) q.push(Point(wx, ly));
                    }
                }
            }
        }
    }
}

void
Canvas::tonePaint(const uint8_t tone[], bool tiling)
{
    uint8_t pat[][3] = {
        {0x00, 0x00, 0x00},
        {0xff, 0x00, 0x00},
        {0x00, 0xff, 0x00},
        {0xff, 0xff, 0x00},
        {0x00, 0x00, 0xff},
        {0xff, 0x00, 0xff},
        {0x00, 0xff, 0xff},
        {0xff, 0xff, 0xff}
    };
    uint16_t col[] = {
        BLACK,
        BLUE,
        RED,
        MAGENTA,
        GREEN,
        CYAN,
        YELLOW,
        WHITE
    };
    int p = 0;
    int n = tone[p++];
    for (int i = 1 ; i <= n ; i++)
    {
        pat[i][0] = (tone[p++] & 0xff);
        pat[i][1] = (tone[p++] & 0xff);
        pat[i][2] = (tone[p++] & 0xff);
        uint8_t b = 0, r = 0, g = 0;
        for (int bit = 0 ; bit < 8 ; bit++)
        {
            uint8_t mask = (1 << bit);
            if (pat[i][0] & mask) ++b;
            if (pat[i][1] & mask) ++r;
            if (pat[i][2] & mask) ++g;
        }
        r = (r == 0) ? 0 : r * 4 - 1;
        g = (g == 0) ? 0 : g * 8 - 1;
        b = (b == 0) ? 0 : b * 4 - 1;
        col[i] = (r << 11) | (g << 5) | b; // RGB565 is for M5Stack
    }
    for (int wy = 0 ; wy < _h ; wy++)
    {
        for (int wx = 0 ; wx < _w ; wx++)
        {
            uint16_t c = pget(wx, wy);
            uint8_t b = c & 0x1f;
            uint8_t g = (c >> 5) & 0x3f;
            uint8_t r = (c >> 11) & 0x1f;
            int ci = ((b == 0) ? 0 : 1) + ((r == 0) ? 0 : 2) + ((g == 0) ? 0 : 4);
            uint16_t cc = col[ci];
            if (tiling)
            {
                b = ((pat[ci][0] >> (7 - wx % 8)) & 1) == 0 ? 0 : 0x1f;
                r = ((pat[ci][1] >> (7 - wx % 8)) & 1) == 0 ? 0 : 0x1f;
                g = ((pat[ci][2] >> (7 - wx % 8)) & 1) == 0 ? 0 : 0x3f;
                cc = (r << 11) | (g << 5) | b;
            }
            pset(wx, wy, cc);
        }
    }
}