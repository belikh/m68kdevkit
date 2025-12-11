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

    void handleIdle() override {
        for (auto& c : children) {
            if (c->isVisible()) c->handleIdle();
        }
    }

protected:
    std::vector<std::shared_ptr<Widget>> children;
};

}
}
