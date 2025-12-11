#pragma once

#include <string>
#include <vector>
#include <memory>
#include <MacWindows.h>
#include <QDOffscreen.h>

namespace MacModern {
namespace GUI {

class Widget;

class Window {
public:
    static std::shared_ptr<Window> create(const std::string& title, int width, int height);

    Window(const std::string& title, int width, int height);
    ~Window();

    void show();
    void add(std::shared_ptr<Widget> widget);
    void draw();

    // Events
    void handleContentClick(int globalX, int globalY);
    void handleKeyDown(char key, int modifiers);
    void handleIdle();

    WindowPtr getNativeHandle() const { return windowRef; }

    void enableDoubleBuffering();

    // Focus
    void setFocus(std::shared_ptr<Widget> widget);

private:
    WindowPtr windowRef;
    std::string title;
    int width, height;
    std::vector<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Widget> focusedWidget;

    bool doubleBuffered;
    GWorldPtr backBuffer;
};

}
}
