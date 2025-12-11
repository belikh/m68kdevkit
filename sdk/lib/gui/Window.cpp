#include "Window.h"
#include "Widget.h"
#include <cstring>

namespace MacModern {
namespace GUI {

std::shared_ptr<Window> Window::create(const std::string& title, int width, int height) {
    return std::make_shared<Window>(title, width, height);
}

Window::Window(const std::string& title, int width, int height)
    : title(title), width(width), height(height) {

    Rect bounds;
    SetRect(&bounds, 50, 50, 50 + width, 50 + height);

    unsigned char pTitle[256];
    pTitle[0] = (unsigned char)title.length();
    std::memcpy(&pTitle[1], title.c_str(), title.length());

    // Create new window (documentProc = standard window)
    windowRef = NewCWindow(NULL, &bounds, pTitle, false, documentProc, (WindowPtr)-1, true, 0);
}

Window::~Window() {
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

void Window::draw() {
    SetPort(windowRef);
    EraseRect(&windowRef->portRect);

    for (auto& widget : widgets) {
        widget->draw();
    }
}

}
}
