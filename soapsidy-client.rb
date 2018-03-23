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
    # When C is read from Serial, a card is detected
    #   It then writes a 'C' to Serial if card does have a subsidy
    #   It writes 'c' otherwise
    # When P is read from Serial, the soap was pumped
    #   It then writes a 'P' to Serial if the transaction of subsidy was
    #     successful
    #   It writes 'p' otherwise

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
      if res.body.to_i != 0
        puts "Card has a subsidy."
        sp.write('C')
      else
        puts "Card has no subsidy."
        sp.write('c')
      end

      # Write serial bit to Arduino
    elsif sp_input == 'P'
      # Pump is activated
      # Post flag, server resets discount and adds subsidy, returns response
      puts "Pump activated."
      uri = URI.parse("http://localhost:3000/cards/pump/")
      header = {'Content-Type': 'text/json'}

      data = { sid: read_sid(sp) }
      response = Net::HTTP.post_form(uri, data)
      status = JSON.parse(response.body)["status"]

      # Write serial bit to Arduino
      if status == "success"
        puts "Subsidy complete."
        sp.write('P')
      elsif status == "failure"
        puts "No subsidy remaining."
        sp.write('N')
      else
        puts "An error occurred with the subsidy."
        sp.write('p')
      end
    end
  end
else
  puts "Port not connected."
end
