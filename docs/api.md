# API Reference

## GUI

### `MacModern::GUI::Application`

*   `static void init()`: Initializes Mac Toolbox.
*   `static void run()`: Starts the event loop.
*   `static void addWindow(shared_ptr<Window> win)`: Registers a window.

### `MacModern::GUI::Window`

*   `static create(title, w, h)`: Factory method.
*   `void add(shared_ptr<Widget>)`: Adds a control.

## Networking

### `MacModern::Crypto::SecureSocket`

*   `connect(host, port)`: Connects to the Bridge.
*   `send(data)`: Encrypts and sends.
*   `receive()`: Receives and decrypts.

**Note**: You must define the PSK to match the Bridge Server.
