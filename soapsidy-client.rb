# This Ruby script acts as an interface between the serial port (courtesy of the SerialPort gem) and the website / database system

require 'serialport'
require 'net/http'
require 'json'

def port_connected?(port)
  return true if Dir.glob(port).count == 1
end

def read_sid(sp)
  # Structure of next output: 'ard: XXXXXXXX'
  for i in 1..5 do
    sp.getc
  end
  buf = ""
  for i in 1..8 do
    buf += sp.getc
  end
  return buf
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

    if sp_input == 'C'
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
    elsif sp_input == 'P'
      # Pump is activated
      # Post flag, server resets discount and adds subsidy, returns response
      puts "Pump activated."
      uri = URI.parse("http://localhost:3000/cards/pump/")
      header = {'Content-Type': 'text/json'}

      data = { sid: read_sid(sp) }
      response = Net::HTTP.post_form(uri, data)
      status = JSON.parse(response.body)[:status]
      puts status
      puts response.body

      # Write serial bit to Arduino
    end
  end
else
  puts "Port not connected."
end
