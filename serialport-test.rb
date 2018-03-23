require 'serialport'

def port_connected?(port)
  return true if Dir.glob(port).count == 1
end

port_str = "/dev/ttyUSB0"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

if port_connected?(port_str)
  puts "yes"
  sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
  sp.read_timeout = 0
  #userin = gets.to_i
  userin = gets.strip
  puts userin
  sp.write(userin)
  puts sp.getc
else
  puts "no"
end

