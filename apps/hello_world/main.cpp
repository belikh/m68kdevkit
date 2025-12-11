#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include <Dialogs.h>

using namespace MacModern::GUI;

int main() {
    Application::init();

    auto win = Window::create("Hello World", 400, 300);
    win->add(std::make_shared<Label>("Welcome to Macintosh SE/30", 50, 50));

    auto btn = std::make_shared<Button>("Click Me!", 50, 100, 120, 40, []() {
        // Simple visual feedback using toolbox alert
        // Note: In a real app we'd need resources for alerts usually,
        // but ParamText + StopAlert works if ALRT resource 0 exists (standard).
        // Alternatively, SysBeep(1);
        SysBeep(1);
    });

    win->add(btn);

    Application::addWindow(win);
    Application::run();

    return 0;
}
