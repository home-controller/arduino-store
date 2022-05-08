#include <Arduino.h>
#include </home/jmnc2/doc/src/Arduino/libraries/store/include/store.h>

#define storageBlocksMax 5
#define eepromBlockStart 15
storageBlock_T storageBlocksA[ storageBlocksMax ];//Mem for sState. Do this here to save the need for acquiring memory in the class at run time.
storageBlock_C sState( storageBlocksMax, storageBlocksA, eepromBlockStart );
//storageBlock_C sState;
#define serial_speed 38400

void setup() {
  // put your setup code here, to run once:
 //   byte id1wire, idGroup3bytes, idGroup10bytes;
 // char tempA[ 10 ];
  //Serial.begin( 9600 );
  Serial.begin( serial_speed );
  Serial.print( "Serial.begin(" );  Serial.print( serial_speed );  Serial.println( ")" );

sState.print(1);
}

void loop() {
  // put your main code here, to run repeatedly:
}
