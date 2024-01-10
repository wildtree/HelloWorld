/*
 * VScroll Contoller
 */

#include <vscroll.h>

// Hardware Scroll does not work for screen orientation 0 (320x240 mode)
// Instead of it, software scroll is implemented by this module.
// Constructer
ZVScroll::ZVScroll(uint16_t top, uint16_t bottom, int x, int y)
    :_top(top), _bottom(bottom), _ty(0), _tx(0), _x(x), _y(y)
{
#if 0
    M5.Display.writecommand(ILI9342C_VSCRDEF);
    this->writeWordData(_top);
    this->writeWordData(YMax - _top - _bottom);
    this->writeWordData(_bottom);
    this->scrollLine();
#else
    _h = YMax - _top - _bottom;
    _buf = new uint16_t [XMax];
    _canvas = new M5Canvas(&M5.Display);
    _canvas->createSprite(XMax, _h);
#endif
}

ZVScroll::ZVScroll(const ZVScroll &x)
    : _top(x._top), _bottom(x._bottom), _ty(x._ty), _tx(x._tx),_h(x._h), _x(x._x), _y(x._y)
{
#if 0
    M5.Display.writecommand(ILI9342C_VSCRDEF);
    this->writeWordData(_top);
    this->writeWordData(YMax - _top - _bottom);
    this->writeWordData(_bottom);
    this->scrollAddress(_y);
#else
    _buf = new uint16_t [XMax];
    _canvas = x._canvas;
#endif
}

// Destructer
ZVScroll::~ZVScroll() {
    if (_buf) delete[] _buf;
    if (_canvas) delete _canvas;
}

// writeWordData
void
ZVScroll::writeWordData(uint16_t data)
{
    M5.Display.writedata(data >> 8);
    M5.Display.writedata(data);
}

void
ZVScroll::scrollAddress(uint16_t data)
{
    M5.Display.writecommand(ILI9342C_VSCADRS);
    this->writeWordData(data);
}

int
ZVScroll::scrollLine()
{
#if 0
    _ty = _y;
    M5.Display.fillRect(0, _y, XMax, FontHeight, BLACK);
    _y += FontHeight;
    if (_y >= YMax - _bottom) _y -= YMax - _bottom - _top;
    scrollAddress(_y);
#else
    int ly = _ty + FontHeight;
    if (ly >= _h)
    {
        for (int y = 0 ; y < _h - FontHeight ; y++)
        {
            _canvas->readRect(0, y + FontHeight, XMax, 1, _buf);
            _canvas->pushImage(0, y, XMax, 1, _buf);
        }
        ly = _ty;
    }
    //Serial.printf("clear line to be written: (%d)\n", _y);
    _canvas->fillRect(0, ly, XMax, FontHeight, BLACK);
    invalidate();
    _ty = ly;
#endif
    _tx = 0;
    return _ty;
}

void
ZVScroll::print(const String &s)
{
    //Serial.printf("String: '%s' (length = %d)\n", s.c_str(), s.length());
    for (int i = 0 ; i < s.length() ; i++)
    {
        uint8_t c = s[i];
        //Serial.printf("idx = %d (code = %#02x) (%d,%d)\n", i, c, _tx, _ty);
        if (isascii(c))
        {
            if (_tx >= XMax - FontWidth)
            {
                scrollLine();
            }
            _canvas->setFont(&fonts::AsciiFont8x16); // it looks no ASCII font having 4x8 size
            _tx += _canvas->drawChar(s[i], _tx, _ty, 2);
        }
        else if (c >= 0x80 && c <= 0xbf) // UTF-8 letters (2nd or later byte letters)
        {
            _canvas->print(s[i]);
        }
        else
        {
            if (_tx >= XMax - FontWidth)
            {
                scrollLine();
            }
            _canvas->setFont(&fonts::lgfxJapanGothic_16);
            _canvas->setCursor(_tx, _ty);
            _canvas->print(s[i]);
            _tx += FontWidth * 2;
        }
    }
    invalidate();
    M5.Display.setFont(&fonts::AsciiFont8x16);
}

void
ZVScroll::cls(void)
{
    _tx = 0;
    _ty = 0;
    _canvas->fillRect(0, _top, XMax, YMax - _bottom - _top, BLACK);
    invalidate();
}

void
ZVScroll::invalidate() const
{
    float affine[] = {0.5,0,(float)_x,0,0.5,(float)_y};
    M5.Display.startWrite();
    _canvas->pushAffineWithAA(affine);
    M5.Display.endWrite();
}