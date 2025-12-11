#pragma once

#include "Widget.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace MacModern {
namespace GUI {

class Container : public Widget {
public:
    Container(int x = 0, int y = 0, int w = 0, int h = 0) : Widget(x, y, w, h) {}

    virtual void addChild(std::shared_ptr<Widget> w) {
        children.push_back(w);
    }

    void draw() override {
        for (auto& c : children) {
            if (c->isVisible()) c->draw();
        }
    }

    bool handleMouseDown(int localX, int localY) override {
        for (auto& c : children) {
            if (c->isVisible() && c->handleMouseDown(localX, localY)) return true;
        }
        return false;
    }

    // Propagate idle
    void handleIdle() override {
        for (auto& c : children) {
            if (c->isVisible()) c->handleIdle();
        }
    }

    // Helper to find focusable widget
    // ...

protected:
    std::vector<std::shared_ptr<Widget>> children;
};

class VStack : public Container {
public:
    VStack(int spacing = 10) : spacing(spacing) {
        // Auto-sizing logic would go here in a real system
    }

    void layout(int startX, int startY, int width) {
        int currentY = startY;
        for (auto& c : children) {
            // Simplified: Assume children have fixed height or we set them
            // In a real system: c->measure()
            // Here we just set their position.
            // We need to access protected members x,y.
            // Since we are friendly or base, we need setters in Widget?
            // Widget members are protected.

            // We need a way to set geometry on children.
            // Let's assume we can static_cast or friend, or add setGeometry to Widget.
        }
    }

private:
    int spacing;
};

}
}
