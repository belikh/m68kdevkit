#include "Application.h"
#include "Window.h"
#include "Controls.h"

using namespace MacModern::GUI;

int main() {
    Application::init();

    auto win = Window::create("Home Assistant", 300, 400);
    win->add(std::make_shared<Label>("Living Room", 20, 20));

    auto btn1 = std::make_shared<Button>("Toggle Lights", 20, 50, 150, 40, []() {
        // Mock network call
        SysBeep(1);
    });
    win->add(btn1);

    win->add(std::make_shared<Label>("Kitchen", 20, 120));
    auto btn2 = std::make_shared<Button>("Toggle Lights", 20, 150, 150, 40, []() {
        // Mock network call
        SysBeep(1);
    });
    win->add(btn2);

    Application::addWindow(win);
    Application::run();
    return 0;
}
