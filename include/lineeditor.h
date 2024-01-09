/*
 * Line Editor
 */

#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <M5Unified.h>

class LineEditor
{
protected:
    char *_buf;
    int _cursor, _tail;
    uint16_t _size;

    String toString() const; 
public:
    LineEditor(uint16_t size = 40);
    LineEditor(const LineEditor &x);
    ~LineEditor();

    operator String() const;
    const String putChar(char c);
    void flush();
};

#endif /* LINEEDITOR_H */