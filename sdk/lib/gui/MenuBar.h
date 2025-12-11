#pragma once

#include "Menu.h"
#include <vector>
#include <memory>
#include <Menus.h>

namespace MacModern {
namespace GUI {

class MenuBar {
public:
    static void init() {
        ClearMenuBar();
    }

    static void addMenu(std::shared_ptr<Menu> menu) {
        InsertMenu(menu->getHandle(), 0); // 0 = append
        menus.push_back(menu);
        DrawMenuBar();
    }

    static void handleSelect(long menuResult) {
        short menuID = HiWord(menuResult);
        short itemID = LoWord(menuResult);

        if (menuID == 0) return;

        for (auto& m : menus) {
            if (m->getID() == menuID) {
                m->handleItem(itemID);
                break;
            }
        }

        HiliteMenu(0);
    }

private:
    static std::vector<std::shared_ptr<Menu>> menus;
};

}
}
