#include "Application.h"
#include "Window.h"
#include "Controls.h"

using namespace MacModern::GUI;

int main() {
    Application::init();

    auto win = Window::create("Hello World", 400, 300);
    win->add(std::make_shared<Label>("Welcome to Macintosh SE/30", 50, 50));
    win->add(std::make_shared<Button>("Click Me!", 50, 100, 120, 40));

    Application::addWindow(win);

    Application::run();

    return 0;
}
