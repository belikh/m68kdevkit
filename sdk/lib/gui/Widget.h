#pragma once

#include <QuickDraw.h>

namespace MacModern {
namespace GUI {

class Widget {
public:
    Widget(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), visible(true) {}
    virtual ~Widget() = default;

    virtual void draw() = 0;

    virtual bool handleMouseDown(int localX, int localY) { return false; }
    virtual bool handleKeyDown(char key) { return false; }
    virtual void handleIdle() {}

    virtual void onFocus() {}
    virtual void onBlur() {}

    bool contains(int localX, int localY) const {
        return (localX >= x && localX < x + w && localY >= y && localY < y + h);
    }

    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }

    // Layout support
    void setGeometry(int newX, int newY, int newW, int newH) {
        x = newX; y = newY; w = newW; h = newH;
    }

    int getWidth() const { return w; }
    int getHeight() const { return h; }

protected:
    int x, y, w, h;
    bool visible;
};

}
}
