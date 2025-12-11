#pragma once

#include "Container.h"

namespace MacModern {
namespace GUI {

class Layout : public Container {
public:
    Layout(int x, int y, int w, int h) : Container(x, y, w, h) {}

    virtual void updateLayout() = 0;

    void addChild(std::shared_ptr<Widget> w) override {
        Container::addChild(w);
        updateLayout();
    }
};

class VStack : public Layout {
public:
    VStack(int x, int y, int w, int h, int spacing = 10)
        : Layout(x, y, w, h), spacing(spacing) {}

    void updateLayout() override {
        int currentY = y + spacing;
        for (auto& c : children) {
            c->setGeometry(x + spacing, currentY, w - (2*spacing), c->getHeight());
            currentY += c->getHeight() + spacing;
        }
    }

private:
    int spacing;
};

class HStack : public Layout {
public:
    HStack(int x, int y, int w, int h, int spacing = 10)
        : Layout(x, y, w, h), spacing(spacing) {}

    void updateLayout() override {
        if (children.empty()) return;
        int itemWidth = (w - (spacing * (children.size() + 1))) / children.size();

        int currentX = x + spacing;
        for (auto& c : children) {
            c->setGeometry(currentX, y + spacing, itemWidth, h - (2*spacing));
            currentX += itemWidth + spacing;
        }
    }

private:
    int spacing;
};

}
}
