#!/usr/bin/env bash
export DISPLAY=:0

pkill Xvfb 2>/dev/null || true
pkill x11vnc 2>/dev/null || true
pkill websockify 2>/dev/null || true
sleep 1

Xvfb :0 -screen 0 1280x800x24 >/tmp/xvfb.log 2>&1 &
sleep 2
fluxbox >/tmp/fluxbox.log 2>&1 &
x11vnc -display :0 -nopw -forever -shared -bg -quiet >/tmp/x11vnc.log 2>&1
websockify --web=/usr/share/novnc 6080 localhost:5900 >/tmp/websockify.log 2>&1 &

echo "noVNC ready at http://localhost:6080/vnc.html"