# Macintosh SE/30 Modern SDK

Welcome to the future of the past. This SDK allows you to build modern C++ applications for the Macintosh SE/30 (System 7.5) using a containerized workflow.

## Repository Structure

*   `sdk/`: The core SDK libraries.
    *   `lib/gui`: A declarative, object-oriented GUI framework wrapping the Mac Toolbox.
    *   `lib/net`: Networking abstractions (OpenTransport).
    *   `lib/crypto`: Modern cryptography (Monocypher) for the Secure Bridge.
*   `bridge/`: The Python Bridge Server that acts as a secure proxy for your Mac apps.
*   `apps/`: Example applications.
    *   `hello_world`: Basic GUI test.
    *   `animation`: Demonstrates double-buffered `CopyBits` sprite rendering.
    *   `notes`: Simple file I/O text editor.
    *   `llm_client`: Connects to OpenAI via the Bridge.
    *   `hass_dashboard`: Mock Home Assistant interface.
*   `docs/`: Documentation (MkDocs).

## Getting Started

1.  **Open in VS Code**: Use the "Dev Containers" extension to reopen this folder in the container.
2.  **Build**: Run the "Build All" task (`Ctrl+Shift+P` -> "Run Task").
3.  **Run**: artifacts are generated in `build/`. Mount this folder in BasiliskII.

## Secure Bridge

To run connected apps:
1.  Run `python3 bridge/server.py` on your host.
2.  Configure your Mac TCP/IP to point to your host IP.
3.  Launch the app on the Mac.

See `docs/` for detailed API reference.
