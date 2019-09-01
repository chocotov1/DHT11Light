# ~~DHT11 humidity only~~ DHT11Light

I wanted to be able to read the humidity from a DHT11 sensor on a ATtiny85 based project. When trying out the most common Arduino DHT11 libraries, I noticed the demo sketch alone already consumed in excess of 8 kB.

After I noticed there were false readings in the first version, I discovered some of the DHT11 pulses were not being registered by pulseIn(). Now i've modified the code:
- Check pulse length in loop cycles instead of pulseIn()
- Added temperature
- Added checksum

Tested with UNO and ATtiny85 (8 mhz)

Preview sketch size with [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) with LTO enabled, all serial output disabled:
```
Sketch uses 986 bytes (12%) of program storage space. Maximum is 8192 bytes.
Global variables use 14 bytes (2%) of dynamic memory, leaving 498 bytes for local variables. Maximum is 512 bytes.
```

## TODO
At 1 mhz it almost works but some pulses are not registered. One day i'll look into timers. A interrupt based version might then also work at 1 mhz.

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
