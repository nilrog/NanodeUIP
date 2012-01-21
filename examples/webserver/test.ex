#/usr/bin/expect

set timeout 100
spawn picocom -b 38400 /dev/ttyUSB1
expect -re IP:(.*)\r
set ip $expect_out(1,string)
expect "+READY"
spawn curl -v http://$ip/digital.shtml
expect "</html>"
spawn curl -v http://$ip/toggle_pin.shtml?pin=1
expect "LOW|HIGH"
puts ""
spawn curl -v http://$ip/ok.txt
expect "+OK"
