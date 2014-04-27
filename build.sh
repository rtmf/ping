#!/bin/sh
gcc $(sdl-config --cflags --libs) ping.c -o ping
