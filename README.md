# DHT11 humidity only

I wanted to be able to read the humidity from a DHT11 sensor on a ATtiny85 based project. When trying out the most common Arduino DHT11 libraries, i noticed the demo sketch alone already consumed in excess of 8 kB.

As of yet i've only testet in on an UNO, but it should work with little modifications on a ATtiny.

Preview sketch size with [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) with LTO enabled, all serial output disabled:
```
Sketch uses 1224 bytes (14%) of program storage space. Maximum is 8192 bytes.
Global variables use 10 bytes (1%) of dynamic memory, leaving 502 bytes for local variables. Maximum is 512 bytes.
```
## Caveat: No checksum check
The DHT11 output reponse contains a checksum. This sketch doesn't check the checksum.

## DHT11 sleep power consumption

When not reading the humidity, the sleep current of the DHT11 was 5 uA at 5 V.

<img src="https://raw.githubusercontent.com/chocotov1/DHT11_humidity_only/master/media/DHT11_sleep_current.jpg" width=640>

## Traces
Overview:<br>
<img src="https://raw.githubusercontent.com/chocotov1/DHT11_humidity_only/master/media/DHT11_scope_overview.jpg" width=640>

Zoom:<br>
<img src="https://raw.githubusercontent.com/chocotov1/DHT11_humidity_only/master/media/DHT11_scope_zoom.jpg" width=640>

After the 18 ms wake up pulse, the signal only rises slowly when not actively driven high:<br>
<img src="https://raw.githubusercontent.com/chocotov1/DHT11_humidity_only/master/media/DHT11_scope_start.jpg" width=640>
