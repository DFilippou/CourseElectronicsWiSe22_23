/****
 Course:Practical Measurements and Electronics Interfaces in Ocean Sciences
 Author:Danai Filippou
 e-mail:danaefil@outlook.com
 Date:01/03/2023
 Description: This is a sketch to set up a temperature measurement experiment using an Arduino Uno 3 and a DS18B20 temperature sensor. More information on the experiment can be found in the relevant report. 
*****/

#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include <OneWire.h>
#include "ds18b20_commands.h"
#include "helper.ino"

const String logfile = "logfile";

RTC_DS1307 rtc;

OneWire ow(4); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(!rtc.begin()) {
    Serial.println("RTC is NOT running :( Let's set the time now! :) ");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  if (!SD.begin()) {
    Serial.println("SD module initialization failed or Card is not present :( ");
    return;
  }
  
}

void loop() {
  byte rom_code[8]; // create an array containing 8 elements of type byte for the rom command
  byte sp_data[9]; // array of 9 bytes
  // Start 1st sequence to read out the rom code 
  ow.reset();
  ow.write(READ_ROM); // skip ROM
  for (int i=0; i<8; i++) {
    rom_code[i] = ow.read();
  }
  if(rom_code[0] != IS_DS18B20_SENSOR){
    Serial.println("Sensor is not a DS18B20 sensor! :( ");  
  }
  // store next 6 bytes in a string 
  String registration_number;
  for (int i=1; i<7; i++) {
    registration_number += String(rom_code[i],HEX);
  }

  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(CONVERT_T);
  
  //Start sequence for converting temperatures 
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(READ_SCRATCHPAD);
  for (int i=0; i<9; i++){ 
    sp_data[i] = ow.read();
        
  } 
  int16_t tempRead = sp_data[1] << 8| sp_data[0];  // need to have a 16 bit integer according to the datasheet

  float tempCelsius = tempRead / 16.0; // divide by 2^4 = 16 to keep 4 digits after the comma
  
  printOutput(getISOtime());
  printOutput(", ");
  printOutput((String)millis());
  printOutput(", ");
  printOutput(registration_number);
  printOutput(", ");
  printOutputln((String)tempCelsius);
  
  delay(1000);
}
