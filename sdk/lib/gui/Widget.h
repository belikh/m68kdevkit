#pragma once

#include <QuickDraw.h>

namespace MacModern {
namespace GUI {

class Widget {
public:
    Widget(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    virtual ~Widget() = default;

    virtual void draw() = 0;

protected:
    int x, y, w, h;
};

}
}
