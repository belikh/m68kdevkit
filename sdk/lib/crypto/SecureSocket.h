#pragma once

#include "../net/Socket.h"
#include "monocypher.h"
#include <vector>
#include <array>

namespace MacModern {
namespace Crypto {

class SecureSocket {
public:
    SecureSocket(Net::Socket* underlyingSocket, const std::array<uint8_t, 32>& psk);
    ~SecureSocket();

    bool connect(const std::string& host, uint16_t port);
    bool send(const std::vector<uint8_t>& data);
    std::vector<uint8_t> receive();
    void close();

private:
    Net::Socket* socket;
    std::array<uint8_t, 32> key;
    uint64_t send_ctr;
    uint64_t recv_ctr;
    uint8_t session_prefix[16];
};

}
}
