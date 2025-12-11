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

        statusLabel = std::make_shared<Label>("Ready.", 20, 100);

        auto btn = std::make_shared<Button>("Send 'Hello'", 20, 50, 150, 40, [this]() {
            this->sendPrompt();
        });

        win->add(btn);
        win->add(statusLabel);

        Application::addWindow(win);
        Application::run();
    }

    void sendPrompt() {
        statusLabel->setText("Sending...");
        Application::forceRedraw(); // Crude update

        OpenTransportSocket otSock;
        SecureSocket sock(&otSock, PSK);

        if (sock.connect("192.168.1.10", 8080)) {
            std::string req = "POST https://api.openai.com/v1/chat/completions {\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"Hello!\"}]}";

            std::vector<uint8_t> data(req.begin(), req.end());
            sock.send(data);

            auto resp = sock.receive();
            if (resp.empty()) {
                statusLabel->setText("Error receiving.");
            } else {
                std::string respStr(resp.begin(), resp.end());
                statusLabel->setText("Got Reply (see console/log)");
                // In real app, parse JSON and show
            }
        } else {
            statusLabel->setText("Connect failed.");
        }
        Application::forceRedraw();
    }

private:
    std::shared_ptr<Label> statusLabel;
};

int main() {
    LLMApp app;
    app.run();
    return 0;
}
