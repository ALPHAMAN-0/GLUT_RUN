FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential gdb \
    freeglut3-dev libglu1-mesa-dev mesa-utils \
    xvfb x11vnc fluxbox \
    novnc websockify \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app