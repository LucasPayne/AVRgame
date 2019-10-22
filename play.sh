#!/bin/sh

if [ $(lsusb | grep atmega32u2 | wc -l) -ne 2 ]
then
    echo "Two UCFKs must be connected."
    exit 1
fi

lsusb | grep atmega32u2 | while read -r line ; do
    echo "Found UCFK:\n\t $line"
    bus_num=$(expr $(echo $line | sed 's/://' | awk '{print $2}') + 0)
    device_num=$(expr $(echo $line | sed 's/://' | awk '{print $4}') + 0)
    echo "\t  Bus number: $bus_num"
    echo "\t  Device number: $device_num"

    if [ ! -f game.hex ]
    then
        echo "ERROR: game.hex file not found."
        exit 1
    fi

    dfu-programmer atmega32u2:$bus_num,$device_num erase
    dfu-programmer atmega32u2:$bus_num,$device_num flash game.hex
    dfu-programmer atmega32u2:$bus_num,$device_num start
done

