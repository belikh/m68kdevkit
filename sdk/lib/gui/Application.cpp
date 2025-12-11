#include "Application.h"
#include "Window.h"
#include "MenuBar.h"
#include "TextField.h"
#include <MacWindows.h>
#include <Fonts.h>
#include <QuickDraw.h>
#include <TextEdit.h>
#include <Dialogs.h>
#include <Events.h>
#include <ToolUtils.h>
#include <Menus.h>

namespace MacModern {
namespace GUI {

bool Application::running = false;
std::vector<std::shared_ptr<Window>> Application::windows;
std::function<void()> Application::idleTask = nullptr;

// Menu IDs
enum {
    mApple = 128,
    mFile,
    mEdit
};

enum {
    iNew = 1,
    iOpen,
    iClose,
    iSave,
    iQuit = 6
};

enum {
    iUndo = 1,
    iCut = 3,
    iCopy,
    iPaste,
    iClear
};

void Application::init() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(NULL);
    InitCursor();

    MenuBar::init();

    // Create Standard Menus
    auto appleMenu = std::make_shared<Menu>(mApple, "\p\024"); // Apple Logo
    appleMenu->addItem("About...", []() { SysBeep(1); });
    appleMenu->addSeparator();
    MenuBar::addMenu(appleMenu);

    auto fileMenu = std::make_shared<Menu>(mFile, "File");
    fileMenu->addItem("New", []() {}, 'N');
    fileMenu->addItem("Open...", []() {}, 'O');
    fileMenu->addItem("Close", []() {}, 'W');
    fileMenu->addItem("Save", []() {}, 'S');
    fileMenu->addSeparator();
    fileMenu->addItem("Quit", []() { Application::quit(); }, 'Q');
    MenuBar::addMenu(fileMenu);

    auto editMenu = std::make_shared<Menu>(mEdit, "Edit");
    editMenu->addItem("Undo", []() {}, 'Z');
    editMenu->addSeparator();
    editMenu->addItem("Cut", []() {
        WindowPtr front = FrontWindow();
        for(auto& w : windows) {
            if (w->getNativeHandle() == front) {
                auto fw = w->getFocusedWidget();
                if (fw) {
                    // We need a common interface or dynamic cast
                    // Assuming TextField for now, or add virtual methods to Widget
                    // Let's add simple cut/copy/paste to Widget base that does nothing by default?
                    // Or dynamic cast.
                    auto tf = std::dynamic_pointer_cast<TextField>(fw);
                    if (tf) tf->cut();
                }
            }
        }
    }, 'X');
    editMenu->addItem("Copy", []() {
        WindowPtr front = FrontWindow();
        for(auto& w : windows) {
            if (w->getNativeHandle() == front) {
                auto fw = w->getFocusedWidget();
                auto tf = std::dynamic_pointer_cast<TextField>(fw);
                if (tf) tf->copy();
            }
        }
    }, 'C');
    editMenu->addItem("Paste", []() {
        WindowPtr front = FrontWindow();
        for(auto& w : windows) {
            if (w->getNativeHandle() == front) {
                auto fw = w->getFocusedWidget();
                auto tf = std::dynamic_pointer_cast<TextField>(fw);
                if (tf) tf->paste();
            }
        }
    }, 'V');
    editMenu->addItem("Clear", []() {
        WindowPtr front = FrontWindow();
        for(auto& w : windows) {
            if (w->getNativeHandle() == front) {
                auto fw = w->getFocusedWidget();
                auto tf = std::dynamic_pointer_cast<TextField>(fw);
                if (tf) tf->clear();
            }
        }
    });
    MenuBar::addMenu(editMenu);
}

void Application::run() {
    running = true;
    EventRecord event;

    while (running) {
        int sleepTime = (idleTask != nullptr) ? 0 : 60;

        bool gotEvent = WaitNextEvent(everyEvent, &event, sleepTime, NULL);

        if (gotEvent) {
            switch (event.what) {
                case mouseDown: {
                    WindowPtr win;
                    short part = FindWindow(event.where, &win);
                    if (part == inDrag) {
                        DragWindow(win, event.where, &qd.screenBits.bounds);
                    } else if (part == inGoAway) {
                        if (TrackGoAway(win, event.where)) {
                             running = false;
                        }
                    } else if (part == inContent) {
                        SelectWindow(win);
                        for(auto& w : windows) {
                            if (w->getNativeHandle() == win) {
                                w->handleContentClick(event.where.h, event.where.v);
                            }
                        }
                    } else if (part == inMenuBar) {
                        MenuBar::handleSelect(MenuSelect(event.where));
                    }
                    break;
                }
                case keyDown:
                case autoKey: {
                     char key = event.message & charCodeMask;
                     if (event.modifiers & cmdKey) {
                         MenuBar::handleSelect(MenuKey(key));
                     } else {
                         WindowPtr front = FrontWindow();
                         for(auto& w : windows) {
                            if (w->getNativeHandle() == front) {
                                w->handleKeyDown(key, event.modifiers);
                            }
                        }
                     }
                     break;
                }
                case updateEvt: {
                    WindowPtr win = (WindowPtr)event.message;
                    BeginUpdate(win);
                    for(auto& w : windows) {
                        if (w->getNativeHandle() == win) {
                            w->draw();
                        }
                    }
                    EndUpdate(win);
                    break;
                }
            }
        }

        for(auto& w : windows) {
            w->handleIdle();
        }

        if (idleTask) {
            idleTask();
        }
    }
}

void Application::quit() {
    running = false;
}

void Application::addWindow(std::shared_ptr<Window> window) {
    windows.push_back(window);
    window->show();
}

void Application::setIdleTask(std::function<void()> task) {
    idleTask = task;
}

void Application::forceRedraw() {
    for(auto& w : windows) {
        w->draw();
    }
}

}
}
