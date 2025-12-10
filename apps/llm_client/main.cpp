#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include "SecureSocket.h"
#include "OpenTransportSocket.h"
#include <string>
#include <vector>
#include <sstream>

using namespace MacModern::GUI;
using namespace MacModern::Crypto;
using namespace MacModern::Net;

// Hardcoded PSK (must match bridge)
const std::array<uint8_t, 32> PSK = {
    '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',
    '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'
};

class LLMApp {
public:
    void run() {
        Application::init();

        auto win = Window::create("LLM Client", 500, 300);
        win->add(std::make_shared<Label>("Ask the AI:", 20, 20));

        // In a real app we'd need a TextField widget and an output area.
        // For this demo, we'll simulate sending a prompt on click.

        auto btn = std::make_shared<Button>("Send 'Hello'", 20, 50, 150, 40);
        win->add(btn);

        auto status = std::make_shared<Label>("Ready.", 20, 100);
        win->add(status);

        // Note: Event handling in this simplified GUI isn't fully hooked up to callbacks yet.
        // We'll simulate the logic that would happen inside a callback.

        Application::addWindow(win);
        Application::run();
    }

    void sendPrompt() {
        OpenTransportSocket otSock;
        SecureSocket sock(&otSock, PSK);

        if (sock.connect("192.168.1.10", 8080)) { // Bridge IP
            std::string req = "POST https://api.openai.com/v1/chat/completions {\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"Hello!\"}]}";

            std::vector<uint8_t> data(req.begin(), req.end());
            sock.send(data);

            auto resp = sock.receive();
            std::string respStr(resp.begin(), resp.end());
            // Update UI with response...
        }
    }
};

int main() {
    LLMApp app;
    app.run();
    return 0;
}
