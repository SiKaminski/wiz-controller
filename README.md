# room-lights

## Light information

A command I stumbled upon to get the information from a light on a network can be found below. 
It basically asks the LED light at ip {x.x.x.x} on port 38899 (default for WiZ lights) for state information 
and if the command returns with something readable I should be able to use it in the program for dynamically 
finding WiZ lights on a network

echo '{"method":"getPilot","params":{}}' | socat - UDP-DATAGRAM:255.255.255.255:38899,broadcast

## Dependencies

jansson (https://github.com/akheron/jansson)

jansson might be loaded under /usr/local/lib so making it work with the current
build system make sure to run the following command

```bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

## CLI Flags

TODO

## Functionality

TODO
