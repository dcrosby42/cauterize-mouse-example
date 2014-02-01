$LOAD_PATH << "./ruby_mouse_comm"

require 'mouse_comm'

filename = "mouse_message.dat"
packed_data = File.read(filename)
puts "Read packed data from '#{filename}': #{packed_data.inspect}"

mouse_message = MouseMessage.unpack(packed_data)
puts "Unpacked message of class: #{mouse_message.class}"
puts "mouse_message.to_ruby.inspect: #{mouse_message.to_ruby.inspect}"
# $res = MouseResponse.unpack(dat)
# p $res



# res = MouseResponse.new(
#   tag: :SAMPLE_MOUSE,
#   data: MouseSample.new(
#     resolution: Resolution.new(width: 1024, height: 768),
#     position: Position.new(x: 500,y: 500),
#     cursor_type: CursorType::IBEAM
#   )
# )
# 
# p res
# p res.pack
