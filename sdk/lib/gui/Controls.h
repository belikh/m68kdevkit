#pragma once

#include "Widget.h"
#include <string>
#include <cstring>
#include <MacTypes.h>
#include <QuickDraw.h>
#include <Controls.h>
#include <functional>

namespace MacModern {
namespace GUI {

class Button : public Widget {
public:
    Button(const std::string& label, int x, int y, int w, int h, std::function<void()> onClick = nullptr)
        : Widget(x, y, w, h), label(label), onClick(onClick) {}

    void draw() override {
        Rect r;
        SetRect(&r, x, y, x + w, y + h);

        FrameRoundRect(&r, 16, 16);
        MoveTo(x + 10, y + 20);

        unsigned char pStr[256];
        pStr[0] = label.length();
        std::memcpy(&pStr[1], label.c_str(), label.length());
        DrawString(pStr);
    }

    bool handleMouseDown(int localX, int localY) override {
        if (contains(localX, localY)) {
            // Visual feedback could go here (invert rect)
            if (onClick) onClick();
            return true;
        }
        return false;
    }

private:
    std::string label;
    std::function<void()> onClick;
};

class Label : public Widget {
public:
    Label(const std::string& text, int x, int y)
        : Widget(x, y, 0, 0), text(text) {}

    void draw() override {
        MoveTo(x, y + 12);
        unsigned char pStr[256];
        pStr[0] = text.length();
        std::memcpy(&pStr[1], text.c_str(), text.length());
        DrawString(pStr);
    }

    void setText(const std::string& t) {
        text = t;
        // In a real system, trigger redraw
    }

private:
    std::string text;
};

}
}
