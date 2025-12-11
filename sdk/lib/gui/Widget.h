#pragma once

#include <QuickDraw.h>

namespace MacModern {
namespace GUI {

class Widget {
public:
    Widget(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    virtual ~Widget() = default;

    virtual void draw() = 0;

    // Returns true if the event was handled
    virtual bool handleMouseDown(int localX, int localY) { return false; }

    bool contains(int localX, int localY) const {
        return (localX >= x && localX < x + w && localY >= y && localY < y + h);
    }

protected:
    int x, y, w, h;
};

}
}
