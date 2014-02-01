#!/bin/sh


runheader() { 
  echo ""
  echo "# $@:"
  $@
}

runheader sh clean.sh
runheader sh cauterize_c.sh
runheader sh cauterize_ruby.sh
runheader sh run_write_packet.sh
runheader ruby read_packet.rb
