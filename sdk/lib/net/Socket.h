#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace MacModern {
namespace Net {

class Socket {
public:
    virtual ~Socket() = default;

    // Connect to a remote host (blocking for simplicity on this platform)
    virtual bool connect(const std::string& host, uint16_t port) = 0;

    // Send data
    virtual bool send(const void* data, size_t size) = 0;
    virtual bool send(const std::vector<uint8_t>& data) {
        return send(data.data(), data.size());
    }

    // Receive data
    // Returns number of bytes read, or -1 on error
    virtual int receive(void* buffer, size_t maxSize) = 0;

    // Check if connected
    virtual bool isConnected() const = 0;

    // Close the connection
    virtual void close() = 0;
};

}
}
