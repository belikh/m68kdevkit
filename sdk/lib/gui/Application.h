#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace MacModern {
namespace GUI {

class Window;

class Application {
public:
    static void init();
    static void run();
    static void quit();

    // Add a window to the application
    static void addWindow(std::shared_ptr<Window> window);

    // Set a callback to run when the application is idle
    static void setIdleTask(std::function<void()> task);

    // Force a redraw of all windows
    static void forceRedraw();

private:
    static bool running;
    static std::vector<std::shared_ptr<Window>> windows;
    static std::function<void()> idleTask;
};

}
}
