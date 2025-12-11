#pragma once

#include <string>
#include <StandardFile.h>
#include <cstring>

namespace MacModern {
namespace GUI {

class Dialogs {
public:
    static std::string getOpenFilename() {
        StandardFileReply reply;
        SFTypeList types; // All types
        StandardGetFile(NULL, -1, types, &reply);
        if (reply.sfGood) {
            // Convert Str255 to std::string
            unsigned char len = reply.sfFile.name[0];
            return std::string((char*)&reply.sfFile.name[1], len);
        }
        return "";
    }

    static std::string getSaveFilename(const std::string& defaultName) {
        StandardFileReply reply;
        unsigned char pName[256];
        pName[0] = defaultName.length();
        std::memcpy(&pName[1], defaultName.c_str(), defaultName.length());

        StandardPutFile(NULL, pName, &reply);
        if (reply.sfGood) {
            unsigned char len = reply.sfFile.name[0];
            return std::string((char*)&reply.sfFile.name[1], len);
        }
        return "";
    }
};

}
}
