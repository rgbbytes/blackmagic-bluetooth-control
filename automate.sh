#!/bin/bash
bluetoothctl << EOF
devices
agent on
connect XX:XX:XX:XX:XX:XX
menu gatt
select-attribute 5dd3465f-1aee-4299-8493-d2eca2f8e1bb
#write to set focus position to 0.9995
write "0x01 0x08 0x00 0x00 0x00 0x00 0x80 0x00 0xff 0x07 0x00 0x00"
EOF
