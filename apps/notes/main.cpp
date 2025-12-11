#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include <fstream>
#include <sstream>
#include <ctime>

using namespace MacModern::GUI;

class NotesApp {
public:
    void run() {
        Application::init();

        auto win = Window::create("Notes", 400, 300);
        win->add(std::make_shared<Label>("Simple Note Taker", 20, 20));

        contentLabel = std::make_shared<Label>("Loading...", 20, 50);
        reload();
        win->add(contentLabel);

        auto btn = std::make_shared<Button>("Save Timestamp", 20, 100, 150, 40, [this]() {
            std::ofstream outFile("Note.txt", std::ios::app);
            outFile << "Note entry at " << std::time(nullptr) << "\n";
            outFile.close();
            this->reload();
            SysBeep(1);
        });

        win->add(btn);

        Application::addWindow(win);
        Application::run();
    }

    void reload() {
        std::ifstream inFile("Note.txt");
        std::string content = "Empty.";
        if (inFile.good()) {
            std::stringstream buffer;
            buffer << inFile.rdbuf();
            content = buffer.str();
        }
        // Truncate for label display
        if (content.length() > 50) content = content.substr(0, 47) + "...";
        contentLabel->setText(content);
        Application::forceRedraw();
    }

private:
    std::shared_ptr<Label> contentLabel;
};

int main() {
    NotesApp app;
    app.run();
    return 0;
}
