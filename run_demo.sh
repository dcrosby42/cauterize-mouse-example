#!/bin/sh

PROG=demo
DIR=c_mouse_comm

runtraced() { echo "$@"; "$@"; }

rm -f $PROG
runtraced gcc -I$DIR $DIR/mouse_comm.c $DIR/cauterize.c ./$PROG.c -o $PROG 
if [ "$?" == "0" ]; then
  runtraced ./$PROG
fi

