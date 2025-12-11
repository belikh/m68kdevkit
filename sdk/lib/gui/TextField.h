#pragma once

#include "Widget.h"
#include <string>
#include <cstring>
#include <TextEdit.h>

namespace MacModern {
namespace GUI {

class TextField : public Widget {
public:
    TextField(int x, int y, int w, int h, const std::string& initialText = "")
        : Widget(x, y, w, h) {

        Rect r;
        SetRect(&r, x, y, x + w, y + h);

        te = nullptr;
        pendingText = initialText;
    }

    ~TextField() {
        if (te) TEDispose(te);
    }

    void draw() override {
        Rect r;
        SetRect(&r, x, y, x + w, y + h);
        FrameRect(&r);
        InsetRect(&r, 1, 1);
        EraseRect(&r);

        ensureTE();
        TEUpdate(&r, te);
    }

    bool handleMouseDown(int localX, int localY) override {
        if (contains(localX, localY)) {
            ensureTE();
            Point pt = { (short)localY, (short)localX };
            TEClick(pt, (EventModifiers)0, te);
            return true;
        }
        return false;
    }

    bool handleKeyDown(char key) override {
        ensureTE();
        TEKey(key, te);
        return true;
    }

    void handleIdle() override {
        if (te) TEIdle(te);
    }

    void onFocus() override {
        if (te) TEActivate(te);
    }

    void onBlur() override {
        if (te) TEDeactivate(te);
    }

    std::string getText() {
        if (!te) return pendingText;

        CharsHandle hText = (*te)->hText;
        long len = (*te)->teLength;

        HLock((Handle)hText);
        std::string s(*hText, len);
        HUnlock((Handle)hText);
        return s;
    }

    void setText(const std::string& text) {
        if (te) {
            TESetText(text.c_str(), text.length(), te);
        } else {
            pendingText = text;
        }
    }

private:
    void ensureTE() {
        if (!te) {
            Rect r;
            SetRect(&r, x, y, x + w, y + h);
            InsetRect(&r, 2, 2);
            Rect dest = r;
            te = TENew(&dest, &r);
            if (!pendingText.empty()) {
                TESetText(pendingText.c_str(), pendingText.length(), te);
                pendingText.clear();
            }
        }
    }

    TEHandle te;
    std::string pendingText;
};

}
}
