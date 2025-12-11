#pragma once

#include "Widget.h"
#include <string>
#include <cstring>
#include <MacTypes.h>
#include <QuickDraw.h>
#include <Controls.h>

namespace MacModern {
namespace GUI {

class Button : public Widget {
public:
    Button(const std::string& label, int x, int y, int w, int h)
        : Widget(x, y, w, h), label(label) {}

    void draw() override {
        Rect r;
        SetRect(&r, x, y, x + w, y + h);

        // Simple manual draw for now as Control Manager needs more setup attached to window
        // In a full SDK we would use NewControl

        FrameRoundRect(&r, 16, 16);
        MoveTo(x + 10, y + 20);

        unsigned char pStr[256];
        pStr[0] = label.length();
        std::memcpy(&pStr[1], label.c_str(), label.length());
        DrawString(pStr);
    }

private:
    std::string label;
};

class Label : public Widget {
public:
    Label(const std::string& text, int x, int y)
        : Widget(x, y, 0, 0), text(text) {} // w/h ignored for simple label

    void draw() override {
        MoveTo(x, y + 12);
        unsigned char pStr[256];
        pStr[0] = text.length();
        std::memcpy(&pStr[1], text.c_str(), text.length());
        DrawString(pStr);
    }

private:
    std::string text;
};

}
}
