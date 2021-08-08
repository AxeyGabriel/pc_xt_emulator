#!/bin/bash

#avrdude -p m2560 -c stk500v2 -P /dev/ttyACM0 -b115200 -F -U flash:w:main.hex
#avrdude -v -F -V -c wiring -p ATmega2560 -P /dev/ttyACM0 -b 115200 -U flash:r:main.hex:i
avrdude -D -F -V -v -c wiring -p ATmega2560 -P /dev/ttyACM0 -U flash:w:main.hex:i
