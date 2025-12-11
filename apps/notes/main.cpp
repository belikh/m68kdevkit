#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include <fstream>
#include <sstream>

using namespace MacModern::GUI;

// Very simple text editor using standard fstream
// Notes are saved to "Note.txt" in the app folder

class NotesApp {
public:
    void run() {
        Application::init();

        auto win = Window::create("Notes", 400, 300);
        win->add(std::make_shared<Label>("Simple Note Taker", 20, 20));

        // Load existing
        std::ifstream inFile("Note.txt");
        std::string content = "Type here...";
        if (inFile.good()) {
            std::stringstream buffer;
            buffer << inFile.rdbuf();
            content = buffer.str();
        }

        // In a real GUI we'd have a TextArea.
        // For this demo we just show the content in a Label (truncated)
        // and a button to "Save" dummy text.

        auto lbl = std::make_shared<Label>(content.substr(0, 50), 20, 50);
        win->add(lbl);

        // Simulating edit
        auto btn = std::make_shared<Button>("Save Timestamp", 20, 100, 150, 40);
        win->add(btn);
        // We'd hook up callback to write to file:
        // std::ofstream outFile("Note.txt");
        // outFile << "Timestamp: " << ...;

        Application::addWindow(win);
        Application::run();
    }
};

int main() {
    NotesApp app;
    app.run();
    return 0;
}
