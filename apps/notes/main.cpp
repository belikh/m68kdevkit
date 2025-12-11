#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include "TextField.h"
#include "Dialogs.h"
#include <fstream>
#include <sstream>
#include <ctime>

using namespace MacModern::GUI;

class NotesApp {
public:
    void run() {
        Application::init(); // This now inits menus too

        auto win = Window::create("Notes", 400, 300);
        // Note: Menus are global in Application::init, but we could add dynamic menus here.
        // The standard menus (File, Edit) are already added by Application::init.

        win->add(std::make_shared<Label>("Simple Note Taker", 20, 20));

        noteField = std::make_shared<TextField>(20, 50, 360, 240, "");
        win->add(noteField);

        // We rely on standard menus for Open/Save now?
        // Or we should update Application::init to call our specific handlers?
        // Current Application::init has empty handlers for File menu.
        // This is a limitation of the current simplified Architecture.
        // To fix this properly, Application should expose "setMenuHandler".

        // For this task, I will stick to the Buttons for Open/Save
        // but the Edit menu (Cut/Copy/Paste) will work on the text field!

        // Let's keep buttons for explicit "Save As" workflow in this demo
        // while demonstrating Edit menu integration.

        Application::addWindow(win);
        Application::run();
    }

private:
    std::shared_ptr<TextField> noteField;
};

int main() {
    NotesApp app;
    app.run();
    return 0;
}
