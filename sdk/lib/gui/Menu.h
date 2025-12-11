#pragma once

#include <string>
#include <vector>
#include <functional>
#include <Menus.h>
#include <cstring>

namespace MacModern {
namespace GUI {

class Menu {
public:
    Menu(int id, const std::string& title) : id(id) {
        unsigned char pTitle[256];
        pTitle[0] = title.length();
        std::memcpy(&pTitle[1], title.c_str(), title.length());

        handle = NewMenu(id, pTitle);
    }

    void addItem(const std::string& text, std::function<void()> callback, char cmdKey = 0) {
        unsigned char pText[256];
        pText[0] = text.length();
        std::memcpy(&pText[1], text.c_str(), text.length());

        AppendMenu(handle, pText);

        if (cmdKey) {
            // SetCmdChar(handle, CountMItems(handle), cmdKey); // Standard API?
            // AppendMenu format "Item/K" sets key.
            // We'll stick to simple Append for now, manual set command char if needed.
            SetItemCmd(handle, CountMItems(handle), cmdKey);
        }

        callbacks.push_back(callback);
    }

    void addSeparator() {
        AppendMenu(handle, (unsigned char*)"\p(-");
        callbacks.push_back(nullptr);
    }

    void handleItem(int itemIndex) {
        if (itemIndex > 0 && itemIndex <= callbacks.size()) {
            if (callbacks[itemIndex - 1]) {
                callbacks[itemIndex - 1]();
            }
        }
    }

    MenuHandle getHandle() const { return handle; }
    int getID() const { return id; }

private:
    int id;
    MenuHandle handle;
    std::vector<std::function<void()>> callbacks;
};

}
}
