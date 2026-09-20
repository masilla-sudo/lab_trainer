FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12
RUN apt-get update \
&& export DEBIAN_FRONTEND=noninteractive \
&& apt-get install -y --no-install-recommends \
cmake ninja-build git clang clangd clang-format lldb gdb \
pkg-config ca-certificates \
&& rm -rf /var/lib/apt/lists/*
WORKDIR /workspaces/dispatcher_trainer
