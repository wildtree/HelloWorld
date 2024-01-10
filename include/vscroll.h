//
// VScroll Controller
//
#ifndef VSCROLL_H
#define VSCROLL_H

#include <M5Unified.h>

#if !defined(ILI9342C_VSCRDEF)
#define ILI9342C_VSCRDEF (0x33)
#endif

#if !defined(ILI9342C_VSCADRS)
#define ILI9342C_VSCADRS (0x37)
#endif

#if !defined(ILI9342C_RDMADCTL)
#define ILI9342C_RDMADCTL (0x0b)
#endif

class ZVScroll
{
protected:
    int _top, _bottom;
    int _x, _y;
    int _tx, _ty;
    int _h;
    uint16_t *_buf;
    M5Canvas *_canvas;

    void writeWordData(uint16_t data);
    void scrollAddress(uint16_t vsp);
public:
    ZVScroll(uint16_t top = 0, uint16_t bottom = 0, int x = 0, int y = 0);
    ZVScroll(const ZVScroll &x);
    ~ZVScroll();

    void setTextColor(uint16_t c) { _canvas->setTextColor(c); }
    int scrollLine();
    void print(const String &s);
    void cls(void);
    void invalidate() const;
    static const int YMax = 240;
    static const int XMax = 320;
    static const int FontHeight = 16;
    static const int FontWidth = 8;
};

#endif /* VSCROLL_H */