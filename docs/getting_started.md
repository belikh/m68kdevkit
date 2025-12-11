# Getting Started

## Prerequisites

1.  **VS Code**
2.  **Docker Desktop**
3.  **BasiliskII** (installed on your host machine)

## Setup

1.  Clone this repository.
2.  Open the folder in VS Code.
3.  When prompted, click **"Reopen in Container"**. This will build the development environment with the Retro68 toolchain.

## Building

Open the Command Palette (`Ctrl+Shift+P` or `Cmd+Shift+P`) and run **"Tasks: Run Build Task"**.

This will compile all applications in `apps/`. The resulting binaries (e.g., `Hello World.dsk` or `.bin`) will be in the `build/` directory.

## Running

To test an application:
1.  Configure your local BasiliskII to mount the `build/` folder as a shared drive ("Unix Root" or standard Mac drive).
2.  Launch BasiliskII.
3.  Navigate to the drive and double-click the application.

## Connected Apps (Bridge)

To run apps that require internet access (like `llm_client`):

1.  Start the Bridge Server on your host (or in a separate container/terminal):
    ```bash
    cd bridge
    python3 server.py
    ```
2.  Ensure your emulated Mac has TCP/IP configured (MacTCP or OpenTransport) and can reach the host (often `192.168.x.x` depending on your network bridging setup).
3.  Launch the app on the Mac.
