#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include "TextField.h"
#include "ProgressBar.h"
#include "Layout.h"
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

        auto win = Window::create("LLM Client", 500, 400);

        // Use Layout
        auto layout = std::make_shared<VStack>(0, 0, 500, 400, 10);

        layout->addChild(std::make_shared<Label>("Ask the AI:", 0, 0)); // Coords ignored by layout

        inputField = std::make_shared<TextField>(0, 0, 0, 60, "Why is the sky blue?"); // H fixed
        layout->addChild(inputField);

        statusLabel = std::make_shared<Label>("Ready.", 0, 0);
        layout->addChild(statusLabel);

        progressBar = std::make_shared<ProgressBar>(0, 0, 0, 20);
        progressBar->setVisible(false);
        layout->addChild(progressBar);

        auto btn = std::make_shared<Button>("Send", 0, 0, 0, 40, [this]() {
            this->sendPrompt();
        });
        layout->addChild(btn);

        layout->addChild(std::make_shared<Label>("Response:", 0, 0));

        responseField = std::make_shared<TextField>(0, 0, 0, 120, "");
        layout->addChild(responseField);

        // Window needs a root container support, but for now we manually add
        // Wait, window logic expects widgets. We can add the layout itself as a widget!
        // Container inherits Widget.
        win->add(layout);

        Application::addWindow(win);
        Application::run();
    }

    void sendPrompt() {
        statusLabel->setText("Sending...");
        progressBar->setVisible(true);
        progressBar->setIndeterminate(true);
        Application::forceRedraw();

        std::string prompt = inputField->getText();
        std::string req = "POST https://api.openai.com/v1/chat/completions {\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}]}";

        OpenTransportSocket otSock;
        SecureSocket sock(&otSock, PSK);

        if (sock.connect("192.168.1.10", 8080)) {
            std::vector<uint8_t> data(req.begin(), req.end());
            sock.send(data);

            auto resp = sock.receive();
            if (resp.empty()) {
                statusLabel->setText("Error receiving.");
            } else {
                std::string respStr(resp.begin(), resp.end());
                statusLabel->setText("Done.");
                responseField->setText(respStr);
            }
        } else {
            statusLabel->setText("Connect failed.");
        }

        progressBar->setVisible(false);
        Application::forceRedraw();
    }

private:
    std::shared_ptr<Label> statusLabel;
    std::shared_ptr<TextField> inputField;
    std::shared_ptr<TextField> responseField;
    std::shared_ptr<ProgressBar> progressBar;
};

int main() {
    LLMApp app;
    app.run();
    return 0;
}
