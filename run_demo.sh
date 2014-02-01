#!/bin/sh

PROG=demo
DIR=c_mouse_comm

(rm -f $PROG) && 
  (gcc -I$DIR $DIR/mouse_comm.c $DIR/cauterize.c ./$PROG.c -o $PROG) &&
  (./$PROG)

