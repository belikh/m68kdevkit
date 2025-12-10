#include "Application.h"
#include "Window.h"
#include <MacWindows.h>
#include <Fonts.h>
#include <QuickDraw.h>
#include <TextEdit.h>
#include <Dialogs.h>
#include <Events.h>
#include <ToolUtils.h>

namespace MacModern {
namespace GUI {

bool Application::running = false;
std::vector<std::shared_ptr<Window>> Application::windows;
std::function<void()> Application::idleTask = nullptr;

void Application::init() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(NULL);
    InitCursor();
}

void Application::run() {
    running = true;
    EventRecord event;

    while (running) {
        // WaitNextEvent with 0 sleep for max performance if we have an idle task
        // otherwise a small sleep to be nice to the OS.
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
                    }
                    break;
                }
                case keyDown:
                case autoKey: {
                     char key = event.message & charCodeMask;
                     if (event.modifiers & cmdKey) {
                         if (key == 'q' || key == 'Q') running = false;
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

        // Run idle task
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
