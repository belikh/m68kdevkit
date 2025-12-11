#include "Application.h"
#include "Window.h"
#include "Widget.h"
#include <QuickDraw.h>
#include <MacTypes.h>
#include <vector>
#include <QDOffscreen.h>

using namespace MacModern::GUI;

// A sprite class that pre-renders to a GWorld
class SpriteWidget : public Widget {
public:
    SpriteWidget(int x, int y, int w, int h)
        : Widget(x, y, w, h), dx(2), dy(2), spriteWorld(NULL) {
        ballX = 10;
        ballY = 10;

        // Create Sprite GWorld
        Rect r;
        SetRect(&r, 0, 0, 20, 20);
        NewGWorld(&spriteWorld, 0, &r, NULL, NULL, 0);

        // Draw into sprite
        if (spriteWorld) {
            GDHandle oldDevice;
            GWorldPtr oldPort;
            GetGWorld(&oldPort, &oldDevice);
            SetGWorld(spriteWorld, NULL);
            PixMapHandle pixMap = GetGWorldPixMap(spriteWorld);
            if (LockPixels(pixMap)) {
                EraseRect(&r);
                FillOval(&r, &qd.black); // Simple black ball
                UnlockPixels(pixMap);
            }
            SetGWorld(oldPort, oldDevice);
        }
    }

    ~SpriteWidget() {
        if (spriteWorld) DisposeGWorld(spriteWorld);
    }

    void draw() override {
        // Update physics
        ballX += dx;
        ballY += dy;

        if (ballX < 0 || ballX > w - 20) dx = -dx;
        if (ballY < 0 || ballY > h - 20) dy = -dy;

        // Draw Frame
        Rect r;
        SetRect(&r, x, y, x + w, y + h);
        FrameRect(&r);

        // Blit Sprite
        if (spriteWorld) {
            PixMapHandle srcPix = GetGWorldPixMap(spriteWorld);
            if (LockPixels(srcPix)) {
                Rect srcRect;
                SetRect(&srcRect, 0, 0, 20, 20);
                Rect dstRect;
                SetRect(&dstRect, x + ballX, y + ballY, x + ballX + 20, y + ballY + 20);

                // Get destination pixmap properly using GetPortPixMap
                CGrafPtr destPort = (CGrafPtr)qd.thePort;
                PixMapHandle destPix = GetPortPixMap(destPort);

                // Need to lock dst if it's offscreen?
                // Application/Window handles locking the backBuffer if double buffered.
                // If direct screen, it's usually always accessible or handled by OS.

                // CopyBits from GWorld to current port
                CopyBits((BitMap*)*srcPix, (BitMap*)*destPix, &srcRect, &dstRect, srcCopy, NULL);

                UnlockPixels(srcPix);
            }
        }
    }

private:
    int ballX, ballY;
    int dx, dy;
    GWorldPtr spriteWorld;
};

int main() {
    Application::init();

    auto win = Window::create("Animation Demo", 400, 300);
    win->enableDoubleBuffering(); // Flicker-free updates!

    win->add(std::make_shared<SpriteWidget>(50, 50, 300, 200));
    Application::addWindow(win);

    Application::setIdleTask([]() {
        Application::forceRedraw();
    });

    Application::run();
    return 0;
}
