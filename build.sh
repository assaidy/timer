#!/bin/bash

set -x

mkdir -p ~/.local/share/timer/
cp share/bell.wav ~/.local/share/timer/

g++ ./timer.cpp -o timer -Wall -lncursesw -lsfml-audio -lsfml-system
sudo cp timer /usr/bin/timer
