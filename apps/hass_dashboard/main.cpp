#include "Application.h"
#include "Window.h"
#include "Controls.h"

using namespace MacModern::GUI;

// Home Assistant Dashboard
// Mockup: Buttons to toggle lights

int main() {
    Application::init();

    auto win = Window::create("Home Assistant", 300, 400);
    win->add(std::make_shared<Label>("Living Room", 20, 20));
    win->add(std::make_shared<Button>("Toggle Lights", 20, 50, 150, 40));

    win->add(std::make_shared<Label>("Kitchen", 20, 120));
    win->add(std::make_shared<Button>("Toggle Lights", 20, 150, 150, 40));

    Application::addWindow(win);
    Application::run();
    return 0;
}
