#!/bin/sh

LANG=c
DIR=c_mouse_comm

runtraced() { echo "$@"; "$@"; }

runtraced rm -rf $DIR
runtraced cauterize generate $LANG $DIR
