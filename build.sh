#!/bin/bash

set -x

mkdir -p ~/.local/share/timer/
cp share/bell.wav ~/.local/share/timer/

g++ ./timer.cpp -lncurses -lsfml-audio -lsfml-system -o timer -Wall
sudo cp timer /usr/bin/timer
