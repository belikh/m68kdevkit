#include "SecureSocket.h"
#include <cstring>
#include <stdexcept>
#include <Events.h> /* For TickCount */
#include <stdlib.h> /* For rand/srand */

namespace MacModern {
namespace Crypto {

SecureSocket::SecureSocket(Net::Socket* underlyingSocket, const std::array<uint8_t, 32>& psk)
    : socket(underlyingSocket), key(psk), recv_ctr(0) {

    // Initialize a pseudo-random nonce prefix for this session
    // This is not cryptographically secure but prevents trivial nonce reuse across restarts
    // if the clock has advanced.
    srand(TickCount());

    // We use a 24-byte nonce (XChaCha20).
    // We will use the first 16 bytes as a random session prefix,
    // and the last 8 bytes as a counter.
    for(int i=0; i<16; ++i) {
        session_prefix[i] = (uint8_t)(rand() % 256);
    }
    send_ctr = 0;
}

SecureSocket::~SecureSocket() {
    close();
}

bool SecureSocket::connect(const std::string& host, uint16_t port) {
    return socket->connect(host, port);
}

bool SecureSocket::send(const std::vector<uint8_t>& data) {
    if (!socket->isConnected()) return false;

    // Prepare nonce
    uint8_t nonce[24];
    std::memcpy(nonce, session_prefix, 16);
    std::memcpy(nonce + 16, &send_ctr, 8);
    send_ctr++;

    // Prepare buffer
    size_t cipherSize = data.size();
    size_t macSize = 16;
    size_t totalSize = 24 + macSize + cipherSize;

    std::vector<uint8_t> frame(4 + totalSize);

    // Write frame length
    uint32_t lenLE = (uint32_t)totalSize;
    frame[0] = lenLE & 0xFF;
    frame[1] = (lenLE >> 8) & 0xFF;
    frame[2] = (lenLE >> 16) & 0xFF;
    frame[3] = (lenLE >> 24) & 0xFF;

    uint8_t* ptr = frame.data() + 4;

    // Write Nonce (Send full nonce for simplicity, though we could optimize)
    std::memcpy(ptr, nonce, 24);
    ptr += 24;

    // Encrypt
    uint8_t mac[16];
    crypto_aead_lock(ptr + 16, mac, key.data(), nonce, NULL, 0, data.data(), data.size());

    std::memcpy(ptr, mac, 16);

    return socket->send(frame);
}

// Helper to ensure full read
static bool readExact(Net::Socket* sock, uint8_t* buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        int r = sock->receive(buf + total, len - total);
        if (r <= 0) return false;
        total += r;
    }
    return true;
}

std::vector<uint8_t> SecureSocket::receive() {
    if (!socket->isConnected()) return {};

    // Read Length
    uint8_t lenBuf[4];
    if (!readExact(socket, lenBuf, 4)) return {};

    uint32_t len = lenBuf[0] | (lenBuf[1] << 8) | (lenBuf[2] << 16) | (lenBuf[3] << 24);

    if (len > 1024 * 1024) return {};

    std::vector<uint8_t> buffer(len);
    if (!readExact(socket, buffer.data(), len)) return {};

    // Parse
    if (len < 24 + 16) return {};

    uint8_t nonce[24];
    std::memcpy(nonce, buffer.data(), 24);

    uint8_t mac[16];
    std::memcpy(mac, buffer.data() + 24, 16);

    uint8_t* cipherText = buffer.data() + 24 + 16;
    size_t cipherSize = len - 24 - 16;

    std::vector<uint8_t> plainText(cipherSize);

    if (crypto_aead_unlock(plainText.data(), mac, key.data(), nonce, NULL, 0, cipherText, cipherSize) != 0) {
        return {};
    }

    return plainText;
}

void SecureSocket::close() {
    socket->close();
}

}
}
