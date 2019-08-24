
// DHT11 info:
// https://www.electronicwings.com/sensors-modules/dht11
// http://www.ocfreaks.com/basics-interfacing-dht11-dht22-humidity-temperature-sensor-mcu/

byte dht11_pin      = 3;
byte dht11_humidity = 0;

//#define DEBUG;

void setup(){
  // it seems to work without any form of pullup (4 pin DHT11 version)
  //pinMode(dht11_pin, INPUT);
  // use INPUT_PULLUP nevertheless
  pinMode(dht11_pin, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("DHT11_humidity_only demo");
}

bool read_dht11_humidity(){
   Serial.println("reading dht11 humidity..");

   // wake up dht11: pull low 18 ms 
   pinMode(dht11_pin, OUTPUT);
   digitalWrite(dht11_pin, 0);
   delay(18);
   // signal rises quicker by driving high, but it also works without doing this:
   //digitalWrite(dht11_pin, 1); 
   
   pinMode(dht11_pin, INPUT_PULLUP);

   // wait a short while before reading the next pulse
   // when not driving the pin high, the initial rising edge after the 18 ms start pulse will otherwise be captured (sometimes)
   delayMicroseconds(40);  

   // expect dht11 get ready high pulse of 80 us (measured 87 us)
   if (!read_high_pulse_dht11(80, 100)){
      return false;
   }

   // compile dht11_humidity, it's in the first 8 bits of the response (40 bits total)
   // shift in the new bits, only reset to 0 if the pulses were of unexpected length
   
   for (byte i = 0; i < 8; i++){
      // length of high pulse:
      // 0: 26-28 us
      // 1: 70 us

      // measured short pulses are occasionally as low as 14, scope revealed the pulses were in reality 22 us
      
      // read_high_pulse_dht11() returns 0 if the min / max condition was not met
      byte bit_pulse = read_high_pulse_dht11(10, 100);

      if (!bit_pulse){
         dht11_humidity = 0;
         return false;
      }
      
      // always shift all bits by one
      dht11_humidity <<= 1;
      
      if (bit_pulse > 50){
         // add the bit if it was a long pulse
         dht11_humidity++;
      }
   }
   
   return true;
}

byte read_high_pulse_dht11(byte min_us, byte max_us){
   byte pulse = pulseIn(dht11_pin, HIGH, 500);
   if (pulse >= min_us && pulse <= max_us){
      return pulse;
   }

#ifdef DEBUG
   Serial.print("expect high pulse min ");
   Serial.print(min_us);
   Serial.print(", max ");
   Serial.print(max_us);
   Serial.print(", got ");
   Serial.println(pulse);
#endif

   return 0;
}

void loop(){
   if(read_dht11_humidity()){
      Serial.print("success, dht11_humidity: ");
      Serial.println(dht11_humidity);
   } else {
      Serial.println("failed");    
   }
   
   delay(5000); 
}
