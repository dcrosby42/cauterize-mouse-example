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
	
