
// DHT11 info:
// https://www.electronicwings.com/sensors-modules/dht11
// http://www.ocfreaks.com/basics-interfacing-dht11-dht22-humidity-temperature-sensor-mcu/

//#define DEBUG;

byte dht11_pin = 2;

const byte dht11_pulse_bits = 40;
#ifdef DEBUG
byte pulses[dht11_pulse_bits];
#endif DEBUG

// dht11: identify long pulses
//
// typical number of measured cycles during dht11 pulse seen during testing: 
// 16 mhz uno                  : long: 180, short: 60
// 16 mhz uno / digitalRead()  : long: 16,  short: 5
// 8 mhz attiny                : long: 90,  short: 30
// 8 mhz attiny / digitalRead(): long: 8,   short: 2
// 1 mhz attiny                : doesn't work, some pulses are not registered

//byte long_cycle_threshold = 100; // 16 mhz
//byte long_cycle_threshold = 50;  // 8 mhz
byte long_cycle_threshold = 70;    // works most of the time at both 8 mhz and 16 mhz

// humidity: dht11_data[0], temperatur: dht11_data[2]
byte dht11_data[5];

void setup(){
  // it seems to work without any form of pullup (4 pin DHT11 version)
  //pinMode(dht11_pin, INPUT);
  // use INPUT_PULLUP nevertheless
  pinMode(dht11_pin, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("DHT11Light demo");
}

bool read_dht11(){
   Serial.print("reading dht11.. ");

   dht11_start();

   if (!dht11_wait_start_pulse()){
      return false;
   }

   for (byte i = 0; i < dht11_pulse_bits; i++){
      byte high_pulse_cycles = read_high_pulse();
      
      byte current_byte = i / 8; 
      
      //  pulse length debugging
      #ifdef DEBUG
      pulses[i] = high_pulse_cycles; 
      #endif DEBUG
                  
      // always shift all bits by one
      dht11_data[current_byte] <<= 1;
      
      // add the bit if it was a long pulse
      if (high_pulse_cycles > long_cycle_threshold){
         dht11_data[current_byte]++;
      }
   }

   // checksum
   if (((dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF) == dht11_data[4]){
      return true;
   }

   // checksum failed, don't reset when debugging
   #ifndef DEBUG
   dht11_data[0] = 0;
   dht11_data[2] = 0;
   #endif
   return false;
}

bool dht11_pin_state() {
   // when using digitalRead() the number of captured cycles decreases: long_cycle_threshold must be lowered
   //return digitalRead(dht11_pin);

   // use better performing PIN register lookup by default: 
   #if defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      // attiny only has PORTB
      return PINB & 1<<dht11_pin;
   #else
      // atmega: pins 0 to 7: PORTD 
      return PIND & 1<<dht11_pin;
   #endif
}

void dht11_start(){
   // wake up dht11: pull low 18 ms 
   pinMode(dht11_pin, OUTPUT);
   digitalWrite(dht11_pin, 0);
   delay(18);
   // signal rises quicker by driving high, but it also works without doing this:
   //digitalWrite(dht11_pin, 1); 
   
   pinMode(dht11_pin, INPUT_PULLUP);  
}

byte read_high_pulse(){
   // counts number of high cycles
   
   // returns 0 when no high signal comes within 255 cycles
   if (!dht11_wait_high_signal()){
      return 0;
   }

   byte high_cycles = 0;

   // ends when i overflows
   for (byte i = 1; i && dht11_pin_state(); i++){
      high_cycles++;
      //cycle_counter++;
   }

   return high_cycles;
   //return cycle_counter;
}

bool dht11_wait_high_signal(){
   // wait for pin to become high

   // ends when i overflows
   for (byte i = 1; i; i++){
       if (dht11_pin_state()){
          return true;
       }
   }

   return false;
}

bool dht11_wait_start_pulse(){
  // the first long pulse marks the beginning of the sequence
  // during testing there always came a short pulse first before the long pulse
  for (byte i = 0; i < 10; i++){
    if (read_high_pulse() > long_cycle_threshold){
      return true;
    }
  }

  return false;
}

#ifdef DEBUG
void print_pulses(){
   for (byte i = 0; i < dht11_pulse_bits; i++){
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(pulses[i]);
      Serial.println();
   }
}
#endif

#ifdef DEBUG
void reset_pulses(){
   for (byte i = 0; i < dht11_pulse_bits; i++){
      pulses[i] = 0;
   }
}
#endif

void loop(){
   if (read_dht11()){
      Serial.print("success: humidity: ");
      Serial.print(dht11_data[0]);
      Serial.print(", temperatur: ");
      Serial.println(dht11_data[2]);   
   } else {
      Serial.println("failed");
   }

   #ifdef DEBUG
   Serial.print("dht11_data[1]: ");
   Serial.println(dht11_data[1]);
   Serial.print("dht11_data[3]: ");
   Serial.println(dht11_data[3]);
   Serial.print("dht11_data[4]: ");
   Serial.print(dht11_data[4]);
   Serial.println(" (checksum)");
   print_pulses();
   reset_pulses();
   #endif DEBUG
   delay(5000); 
}
