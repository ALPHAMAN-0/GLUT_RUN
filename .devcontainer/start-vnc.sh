#!/usr/bin/env bash
export DISPLAY=:0

pkill Xvfb 2>/dev/null || true
sleep 1

Xvfb :0 -screen 0 1280x800x24 &      # virtual display
sleep 2
fluxbox &                             # window manager (lets you move windows)
x11vnc -display :0 -nopw -forever -shared -bg
websockify --web=/usr/share/novnc 6080 localhost:5900 &   # browser bridge

echo "noVNC ready at http://localhost:6080/vnc.html"