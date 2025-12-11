#include "OpenTransportSocket.h"
#include <cstring>

namespace MacModern {
namespace Net {

OpenTransportSocket::OpenTransportSocket() : ep(kOTInvalidEndpointRef), connected(false) {
    OSStatus err = InitOpenTransport();
    // In a real app we'd handle error
}

OpenTransportSocket::~OpenTransportSocket() {
    close();
    CloseOpenTransport();
}

bool OpenTransportSocket::connect(const std::string& host, uint16_t port) {
    if (connected) close();

    OSStatus err;
    ep = OTOpenEndpointInContext(OTCreateConfiguration(kTCPName), 0, NULL, NULL);
    if (ep == kOTInvalidEndpointRef) return false;

    InetAddress addr;
    OTInitInetAddress(&addr, port, (char*)host.c_str());

    TCall sndCall;
    OTMemzero(&sndCall, sizeof(TCall));
    sndCall.addr.buf = (UInt8*)&addr;
    sndCall.addr.len = sizeof(InetAddress);

    err = OTConnect(ep, &sndCall, NULL);

    if (err == noErr) {
        connected = true;
    } else {
        OTCloseProvider(ep);
        ep = kOTInvalidEndpointRef;
    }

    return connected;
}

bool OpenTransportSocket::send(const void* data, size_t size) {
    if (!connected) return false;

    // Loop to ensure all data is sent
    size_t totalSent = 0;
    const uint8_t* bytes = (const uint8_t*)data;

    while (totalSent < size) {
        OTResult result = OTSnd(ep, (void*)(bytes + totalSent), size - totalSent, 0);
        if (result < 0) return false; // Error
        totalSent += result;
    }
    return true;
}

int OpenTransportSocket::receive(void* buffer, size_t size) {
    if (!connected) return -1;

    // The interface expects us to try to read 'size' bytes?
    // Or just "up to"? The base class signature implies "up to".
    // However, SecureSocket needs "exactly".
    // Let's make `receive` return what it gets (standard recv behavior),
    // and let the caller loop if they need exact amounts.

    OTResult result = OTRcv(ep, buffer, size, NULL);
    if (result < 0) {
        if (result == kOTNoDataErr) return 0; // Non-blocking check usually, but we are blocking
        return -1;
    }
    return result;
}

bool OpenTransportSocket::isConnected() const {
    return connected;
}

void OpenTransportSocket::close() {
    if (ep != kOTInvalidEndpointRef) {
        OTSndDisconnect(ep, NULL);
        OTCloseProvider(ep);
        ep = kOTInvalidEndpointRef;
    }
    connected = false;
}

}
}
