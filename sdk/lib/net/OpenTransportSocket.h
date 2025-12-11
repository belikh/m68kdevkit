#pragma once

#include "Socket.h"
#include <OpenTransport.h>
#include <OpenTptInternet.h>

namespace MacModern {
namespace Net {

class OpenTransportSocket : public Socket {
public:
    OpenTransportSocket();
    virtual ~OpenTransportSocket();

    bool connect(const std::string& host, uint16_t port) override;
    bool send(const void* data, size_t size) override;
    int receive(void* buffer, size_t maxSize) override;
    bool isConnected() const override;
    void close() override;

private:
    EndpointRef ep;
    bool connected;
};

}
}
