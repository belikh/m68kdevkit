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

    void handleContentClick(int globalX, int globalY);

    WindowPtr getNativeHandle() const { return windowRef; }

    void enableDoubleBuffering();

private:
    WindowPtr windowRef;
    std::string title;
    int width, height;
    std::vector<std::shared_ptr<Widget>> widgets;

    bool doubleBuffered;
    GWorldPtr backBuffer;
};

}
}
