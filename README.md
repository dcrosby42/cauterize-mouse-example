## Problem

Define a message protocol for synchronizing mouse state over a network by communicating:

* latest mouse location, screen resolution and cursor state
* ...or that something has gone wrong.

## Design
	
Conceptually we could use a data structure like this:

	position:
		x: 123
		y: 456
	screen_size:
		width: 1920
		height: 1089
	cursor_type: IBEAM
	
There's really only 5 pieces of info here, and we could easily represent them as an array of 5 unsigned integer values.  But a number of potential issues arrise with that fairly simple approach:

* Integer encoding issues / endieness differences of sender/receiver.
* Which of the 5 values is X, and which is the vertical component of the screen size?  This is easy to mess up *the first time.*
* What if we need to extend the protocol or create variance in the structure? (For example, to occasionally send bitmap data to update the receiver's cache of cursor images?)

**Gotcha:** we want to encode and send these messages using C, but receive and decode them using Ruby.

## Cauterize

### Install the Cauterize Rubygem

	gem install cauterize
	
or

	git clone https://github.com/sw17ch/cauterize.git
	cd cauterize
	bundle install
	rake install
	rbenv rehash # if using rbenv
	
Check installation:

	cauterize help
	
### Define our Cauterize schema

Create a file named "Cauterize":

    set_name("mouse_comm")
    set_version("0.0.1")

    enumeration(:cursor_type) do |c|
      c.value :normal
      c.value :cross
      c.value :hand
      c.value :resize
      c.value :ibeam, 1024
    end

    composite(:resolution) do |c|
      c.field :width, :uint16
      c.field :height, :uint16
    end

    composite(:position) do |c|
      c.field :x, :uint16
      c.field :y, :uint16
    end

    composite(:mouse_sample) do |c|
      c.field :cursor_type, :cursor_type
      c.field :resolution, :resolution
      c.field :position, :position
    end

    group(:sync_mouse) do |c|
      c.field :sample_mouse, :mouse_sample
      c.field :error, :error_information
    end

    variable_array(:error_information) do |c|
      c.size_type :uint8
      c.array_type :uint8
      c.array_size 255
    end

### Generate the C library code for this schema

	cauterize generate c c_mouse_comm
	
This will create the subdir `c_mouse_com/` containing:

* `mouse_comm.h` - you include this to build and interpret data structures from your schema
* `mouse_comm.c` - the implementation of the 'mouse_comm' schema
* (Cauterize .h and .c deps used by mouse_comm schema code.)

## The "writer"

### Write a C program to create and serialize data

Include the header:

	#include <mouse_comm.h>
	
You can know in advance how big a buffer you'll need to serialize mouse_message objects into:

	uint8_t message_buffer[MAX_ENCODED_LENGTH_mouse_message];
	
You can build instances of your objects using struct literals in your code:

    struct mouse_message msg_obj = {
      .tag = GROUP_MOUSE_MESSAGE_TYPE_MOUSE_UPDATE,
      .data = {
        .mouse_update = {
          .cursor_type = IBEAM,
          .resolution = {
            .width = 1024,
            .height = 768,
          },
          .position = {
            .x = 500,
            .y = 500,
          },
        },
      },
    };

You can pack that whole object into a buffer with a single call:

    struct Cauterize c;
    CAUTERIZE_STATUS_T err = CA_OK;
    err = CauterizeInitAppend(&c, message_buffer, sizeof(message_buffer));
    err = Pack_mouse_message(&c, &msg_obj);


### Sample program: `write_packet.c`

You can view, compile and run the `write_packet.c` program in this repository.  (When tinkering, run the helper script `./run_write_packet.h`)

Compile:

	gcc -Ic_mouse_comm c_mouse_comm/mouse_comm.c c_mouse_comm/cauterize.c write_packet.c -o write_packet
	
Run:

	./write_packet
	
See output:

	hexdump mouse_message.dat
	
You should see binary output indicating you serialized `00 00 04 00 04 00 03 F4 01 F4 01`.

## The "reader"

### Write a Ruby program to read the cauterized data

Make sure the `ruby_mouse_comm/` dir is in Ruby's `$LOAD_PATH`, and you can require your Ruby library for handling `mouse_comm` messages:

	require 'mouse_comm'
	
	packed_data = File.read("mouse_message.dat")
	mouse_message = MouseMessage.unpack(packed_data)

Calling `#to_ruby` on a Cauterize structure will give its Ruby-ified equivalent:

    {
         :tag => :MOUSE_UPDATE,
        :data => {
            :cursor_type => :IBEAM,
             :resolution => {
                :width => 1024,
                :height => 768
            },
               :position => {
                :x => 500,
                :y => 500
            }
        }
    }

### Sample program: `read_packet.rb`

You can view run the `read_packet.rb` program in this repository.

	ruby read_packet.rb
	
See output:

	Read packed data from 'mouse_message.dat': "\x00\x00\x04\x00\x04\x00\x03\xF4\x01\xF4\x01"
	Unpacked message of class: MouseMessage
	mouse_message.to_ruby.inspect: {:tag=>:MOUSE_UPDATE, :data=>{:cursor_type=>:IBEAM, :resolution=>{:width=>1024, :height=>768}, :position=>{:x=>500, :y=>500}}}

