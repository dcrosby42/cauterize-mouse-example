#!/bin/sh

PROG=write_packet
DIR=c_mouse_comm

runtraced() { echo "$@"; "$@"; }

rm -rf write_packet c_mouse_comm ruby_mouse_comm mouse_message.dat

