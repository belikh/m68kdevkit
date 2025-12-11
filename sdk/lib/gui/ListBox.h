#pragma once

#include "Widget.h"
#include <string>
#include <vector>
#include <Lists.h>
#include <MacWindows.h>

namespace MacModern {
namespace GUI {

class ListBox : public Widget {
public:
    ListBox(int x, int y, int w, int h)
        : Widget(x, y, w, h), listHandle(nullptr) {
    }

    ~ListBox() {
        if (listHandle) LDispose(listHandle);
    }

    void addItem(const std::string& item) {
        ensureList();
        int row = LAddRow(1, 0, listHandle);
        LSetCell(item.c_str(), item.length(), { (short)row, 0 }, listHandle);
        // Trigger update? LUpdate(listHandle->port->visRgn, listHandle);
    }

    void draw() override {
        ensureList();
        Rect r;
        SetRect(&r, x, y, x + w, y + h);
        FrameRect(&r);
        // List manager drawing
        LUpdate(getVisRgn(), listHandle); // Simplified, usually update region
        // Actually LUpdate expects the region to update.
        // We can just call LDraw(listHandle) if we want to force redraw?
        // Or LUpdate(window->visRgn, ...)
    }

    bool handleMouseDown(int localX, int localY) override {
        if (contains(localX, localY)) {
            ensureList();
            Point pt = { (short)localY, (short)localX };
            LClick(pt, 0, listHandle);
            return true;
        }
        return false;
    }

    // Helper
    RgnHandle getVisRgn() {
        // Assume current port is window
        return ((WindowPtr)qd.thePort)->visRgn;
    }

private:
    void ensureList() {
        if (!listHandle) {
            Rect r;
            SetRect(&r, x+1, y+1, x + w - 16, y + h - 1); // Room for scrollbar
            Rect dataBounds = {0, 0, 0, 1}; // 1 Column
            Point cellSize = {0, 0}; // Auto

            // 0 = standard text list definition
            listHandle = LNew(&r, &dataBounds, cellSize, 0, (WindowPtr)qd.thePort, true, false, false, true);
        }
    }

    ListHandle listHandle;
};

}
}
