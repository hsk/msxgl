#!/bin/bash
# ____________________________
# ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
# ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
# █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
# ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
#  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
#────────────────────────────────────────────────────────────────────
clear

if type -P node; then
	node ../../MSXgl/engine/script/js/build.js projname=$1 target=$2
else
	../../MSXgl/tools/build/Node/node ../../MSXgl/engine/script/js/build.js projname=$1 target=$2
fi
