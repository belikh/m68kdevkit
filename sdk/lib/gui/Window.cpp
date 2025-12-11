#include "Window.h"
#include "Widget.h"
#include <cstring>

namespace MacModern {
namespace GUI {

std::shared_ptr<Window> Window::create(const std::string& title, int width, int height) {
    return std::make_shared<Window>(title, width, height);
}

Window::Window(const std::string& title, int width, int height)
    : title(title), width(width), height(height), doubleBuffered(false), backBuffer(NULL) {

    Rect bounds;
    SetRect(&bounds, 50, 50, 50 + width, 50 + height);

    unsigned char pTitle[256];
    pTitle[0] = (unsigned char)title.length();
    std::memcpy(&pTitle[1], title.c_str(), title.length());

    windowRef = NewCWindow(NULL, &bounds, pTitle, false, documentProc, (WindowPtr)-1, true, 0);
}

Window::~Window() {
    if (backBuffer) {
        DisposeGWorld(backBuffer);
    }
    if (windowRef) {
        DisposeWindow(windowRef);
    }
}

void Window::show() {
    ShowWindow(windowRef);
    SelectWindow(windowRef);
}

void Window::add(std::shared_ptr<Widget> widget) {
    widgets.push_back(widget);
}

void Window::enableDoubleBuffering() {
    if (doubleBuffered) return;

    Rect r = windowRef->portRect;
    QDErr err = NewGWorld(&backBuffer, 0, &r, NULL, NULL, 0);
    if (err == noErr) {
        doubleBuffered = true;
    }
}

void Window::draw() {
    CGrafPtr port = (CGrafPtr)windowRef;

    if (doubleBuffered && backBuffer) {
        GDHandle oldDevice;
        GWorldPtr oldPort;
        GetGWorld(&oldPort, &oldDevice);
        SetGWorld(backBuffer, NULL);

        PixMapHandle pixMap = GetGWorldPixMap(backBuffer);
        if (LockPixels(pixMap)) {
            EraseRect(&windowRef->portRect);

            for (auto& widget : widgets) {
                if(widget->isVisible()) widget->draw();
            }

            UnlockPixels(pixMap);

            SetGWorld(oldPort, oldDevice);
            SetPort(windowRef);

            Rect src = windowRef->portRect;
            Rect dst = windowRef->portRect;

            // Safe copy
            PixMapHandle portPix = GetPortPixMap(port);
            CopyBits((BitMap*)*pixMap, (BitMap*)*portPix, &src, &dst, srcCopy, NULL);
        } else {
            SetGWorld(oldPort, oldDevice);
        }
    } else {
        SetPort(windowRef);
        EraseRect(&windowRef->portRect);
        for (auto& widget : widgets) {
             if(widget->isVisible()) widget->draw();
        }
    }
}

void Window::handleContentClick(int globalX, int globalY) {
    SetPort(windowRef);
    Point pt = { (short)globalY, (short)globalX };
    GlobalToLocal(&pt);

    bool handled = false;
    for (auto& widget : widgets) {
        if (widget->isVisible() && widget->handleMouseDown(pt.h, pt.v)) {
            setFocus(widget);
            handled = true;
            break;
        }
    }

    if (!handled) {
        setFocus(nullptr);
    }
}

void Window::handleKeyDown(char key, int modifiers) {
    if (focusedWidget) {
        SetPort(windowRef);
        focusedWidget->handleKeyDown(key);
    }
}

void Window::handleIdle() {
    SetPort(windowRef);
    for (auto& widget : widgets) {
        if(widget->isVisible()) widget->handleIdle();
    }
}

void Window::setFocus(std::shared_ptr<Widget> widget) {
    if (focusedWidget == widget) return;

    if (focusedWidget) focusedWidget->onBlur();
    focusedWidget = widget;
    if (focusedWidget) focusedWidget->onFocus();
}

}
}
