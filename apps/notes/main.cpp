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
        Application::init();

        auto win = Window::create("Notes", 400, 300);
        win->add(std::make_shared<Label>("Simple Note Taker", 20, 20));

        noteField = std::make_shared<TextField>(20, 50, 360, 200, "");
        win->add(noteField);

        auto openBtn = std::make_shared<Button>("Open", 20, 260, 80, 30, [this]() {
            std::string path = Dialogs::getOpenFilename();
            if (!path.empty()) {
                this->loadFile(path);
            }
        });
        win->add(openBtn);

        auto saveBtn = std::make_shared<Button>("Save As", 110, 260, 80, 30, [this]() {
            std::string path = Dialogs::getSaveFilename("Note.txt");
            if (!path.empty()) {
                this->saveFile(path);
            }
        });
        win->add(saveBtn);

        Application::addWindow(win);
        Application::run();
    }

    void loadFile(const std::string& path) {
        std::ifstream inFile(path);
        if (inFile.good()) {
            std::stringstream buffer;
            buffer << inFile.rdbuf();
            noteField->setText(buffer.str());
        }
        Application::forceRedraw();
    }

    void saveFile(const std::string& path) {
        std::ofstream outFile(path);
        outFile << noteField->getText();
        outFile.close();
    }

private:
    std::shared_ptr<TextField> noteField;
};

int main() {
    NotesApp app;
    app.run();
    return 0;
}
