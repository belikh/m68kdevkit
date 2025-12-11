#pragma once

#include "Widget.h"
#include <algorithm>

namespace MacModern {
namespace GUI {

class ProgressBar : public Widget {
public:
    ProgressBar(int x, int y, int w, int h)
        : Widget(x, y, w, h), progress(0.0f), indeterminate(false) {}

    void setProgress(float p) {
        progress = std::max(0.0f, std::min(1.0f, p));
        // Trigger redraw in real system
    }

    void setIndeterminate(bool i) {
        indeterminate = i;
    }

    void draw() override {
        Rect r;
        SetRect(&r, x, y, x + w, y + h);
        FrameRect(&r);
        InsetRect(&r, 1, 1);
        EraseRect(&r);

        if (indeterminate) {
            // Draw barber pole or bouncing box
            // Simplified: Fill 50%
            r.right = r.left + (w/2);
            FillRect(&r, &qd.ltGray);
        } else {
            r.right = r.left + (int)((float)(w-2) * progress);
            FillRect(&r, &qd.black);
        }
    }

private:
    float progress;
    bool indeterminate;
};

}
}
