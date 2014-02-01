#!/bin/sh

LANG=ruby
DIR=ruby_mouse_comm

runtraced() { echo "$@"; "$@"; }

runtraced rm -rf $DIR
runtraced cauterize generate $LANG $DIR
