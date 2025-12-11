#include "Application.h"
#include "Window.h"
#include "Controls.h"
#include "TextField.h"
#include "ProgressBar.h"
#include "Layout.h"
#include "ListBox.h"
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

        auto layout = std::make_shared<VStack>(0, 0, 500, 400, 10);

        layout->addChild(std::make_shared<Label>("Chat History:", 0, 0));

        historyList = std::make_shared<ListBox>(0, 0, 0, 200);
        layout->addChild(historyList);

        layout->addChild(std::make_shared<Label>("Prompt:", 0, 0));
        inputField = std::make_shared<TextField>(0, 0, 0, 40, "Hello");
        layout->addChild(inputField);

        auto btn = std::make_shared<Button>("Send", 0, 0, 0, 30, [this]() {
            this->sendPrompt();
        });
        layout->addChild(btn);

        progressBar = std::make_shared<ProgressBar>(0, 0, 0, 10);
        progressBar->setVisible(false);
        layout->addChild(progressBar);

        statusLabel = std::make_shared<Label>("Ready.", 0, 0);
        layout->addChild(statusLabel);

        win->add(layout);
        Application::addWindow(win);
        Application::run();
    }

    void sendPrompt() {
        std::string prompt = inputField->getText();
        historyList->addItem("You: " + prompt);
        inputField->setText("");

        progressBar->setVisible(true);
        progressBar->setIndeterminate(true);
        statusLabel->setText("Thinking...");
        Application::forceRedraw();

        // Network call...
        OpenTransportSocket otSock;
        SecureSocket sock(&otSock, PSK);

        if (sock.connect("192.168.1.10", 8080)) {
             std::string req = "POST https://api.openai.com/v1/chat/completions {\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}]}";
             std::vector<uint8_t> data(req.begin(), req.end());
             sock.send(data);
             auto resp = sock.receive();
             if (!resp.empty()) {
                 std::string respStr(resp.begin(), resp.end());
                 historyList->addItem("AI: " + respStr);
                 statusLabel->setText("Done.");
             } else {
                 statusLabel->setText("Error.");
             }
        }

        progressBar->setVisible(false);
        Application::forceRedraw();
    }

private:
    std::shared_ptr<ListBox> historyList;
    std::shared_ptr<TextField> inputField;
    std::shared_ptr<Label> statusLabel;
    std::shared_ptr<ProgressBar> progressBar;
};

int main() {
    LLMApp app;
    app.run();
    return 0;
}
