FROM autc04/retro68:latest

# Install additional tools for modern development
# ninja-build for faster cmake builds
# python3/pip for the bridge server/docs if run inside
# git for version control
RUN apt-get update && apt-get install -y \
    ninja-build \
    python3 \
    python3-pip \
    python3-venv \
    git \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Install python dependencies for bridge/docs (optional, but good to have cached)
# We will also install them in the specific services, but having them here helps VS Code intelligence
COPY bridge/requirements.txt /tmp/bridge_requirements.txt
RUN python3 -m pip install -r /tmp/bridge_requirements.txt --break-system-packages || true

# Set up the workspace
WORKDIR /workspace
