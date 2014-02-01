#!/bin/sh

PROG=write_packet
DIR=c_mouse_comm

runtraced() { echo "$@"; "$@"; }

rm -f $PROG
rm -f mouse_message.dat
runtraced gcc -I$DIR $DIR/mouse_comm.c $DIR/cauterize.c ./$PROG.c -o $PROG 
if [ "$?" == "0" ]; then
  runtraced ./$PROG
fi

