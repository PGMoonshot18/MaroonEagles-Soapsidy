# This Ruby script acts as an interface between the serial port (courtesy of the SerialPort gem) and the website / database system

require 'serialport'
require 'net/http'

def port_connected?(port)
  return true if Dir.glob(port).count == 1
end

def read_sid(sp)
  return "12.34.56.78"
end

port_str = "/dev/ttyUSB0"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

if port_connected?(port_str)
  puts "Port connected."
  sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
  sp.read_timeout = 0
  while true do
    # The program loop
    sp_input = sp.getc

    if sp_input == 'a'
      # Card is detected
      # Send card ID, get status of card
      puts "Card detected."
      url = URI.parse("http://localhost:3000/cards/sid/#{read_sid(sp)}/")
      req = Net::HTTP::Get.new(url.to_s)
      res = Net::HTTP.start(url.host, url.port) {|http|
          http.request(req)
      }
      puts res.body

      # Write serial bit to Arduino
    elsif sp_input == 'b'
      # Pump is activated
      # Post flag, server resets discount and adds subsidy, returns response
      puts "Pump activated."
      url = URI.parse("http://localhost:3000/cards/pump/")
      req = Net::HTTP::Post.new(url.to_s)
      req.body = "{:sid => 'a1b2cd4'}"
      res = Net::HTTP.start(url.host, url.port) {|http|
          http.request(req)
      }
      puts res.body

      # Write serial bit to Arduino
    end
  end
else
  puts "Port not connected."
end
