# Architecture

## Secure Bridge Model

The Macintosh SE/30 (68030 CPU) is too slow to handle modern TLS 1.3 handshakes and encryption. To solve this, we use a **Bridge Server**.

### Components

1.  **Macintosh App**: Uses `SecureSocket` to communicate with the Bridge.
2.  **Bridge Server**: A Python daemon running on a modern machine (Host).
3.  **The Internet**: The Bridge proxies requests to standard HTTPS APIs (OpenAI, Home Assistant, etc.).

### Protocol

Communication between Mac and Bridge uses **Monocypher** (ChaCha20-Poly1305) for authenticated encryption.

*   **Encryption**: ChaCha20 (High performance stream cipher).
*   **Authentication**: Poly1305 (Ensures data integrity).
*   **Key Exchange**: Pre-Shared Key (PSK) for simplicity and speed.

Packet Format:
```
[Length (4 bytes)] [Nonce (24 bytes)] [MAC (16 bytes)] [Ciphertext (N bytes)]
```

### Advantages

*   **Speed**: The 68k Mac only does lightweight symmetric crypto.
*   **Security**: Plaintext never hits the local LAN.
*   **Compatibility**: The Bridge handles complex TLS certificates and HTTP/2.
