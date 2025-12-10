#include "Application.h"
#include "Window.h"
#include "Widget.h"
#include <QuickDraw.h>
#include <MacTypes.h>
#include <vector>

using namespace MacModern::GUI;

class BouncingBall : public Widget {
public:
    BouncingBall(int x, int y, int w, int h)
        : Widget(x, y, w, h), dx(2), dy(2) {
        ballX = 10;
        ballY = 10;
    }

    void draw() override {
        // Clear previous pos (simplified: clearing whole widget area)
        Rect r;
        SetRect(&r, x, y, x + w, y + h);
        EraseRect(&r);
        FrameRect(&r);

        // Update physics
        ballX += dx;
        ballY += dy;

        if (ballX < 0 || ballX > w - 20) dx = -dx;
        if (ballY < 0 || ballY > h - 20) dy = -dy;

        // Draw Ball
        Rect ballRect;
        SetRect(&ballRect, x + ballX, y + ballY, x + ballX + 20, y + ballY + 20);
        FillOval(&ballRect, &qd.black);
    }

private:
    int ballX, ballY;
    int dx, dy;
};

int main() {
    Application::init();

    auto win = Window::create("Animation Demo", 400, 300);
    win->add(std::make_shared<BouncingBall>(50, 50, 300, 200));
    Application::addWindow(win);

    // Set idle task to force redraw continuously
    Application::setIdleTask([]() {
        Application::forceRedraw();
    });

    Application::run();
    return 0;
}
