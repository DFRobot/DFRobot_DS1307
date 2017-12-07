/**
 * I2Ctest.pde -- Tiny RTC module for Arduino
 *
 * 2013, Waiman Zhao, http://waiman.taobao.com/
 *
 */
#include <OneWire.h>
#include "Wire.h"
#include "Arduino.h" 
#include <DS1307.h>
#include <avr/io.h>
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}


byte start_address = 1;
byte end_address = 127;
OneWire  ds(2);  // on pin 2
byte Tdata[12];
int sensorValue = 0;        // value read from the pot
int rtc[7];
float TT=0.0;
int val;


//*****************************************
//
//      I2C Scaner code
//
//*****************************************
// Scan the I2C bus between addresses from_addr and to_addr.
// On each address, call the callback function with the address and result.
// If result==0, address was found, otherwise, address wasn't found
// (can use result to potentially get other status on the I2C bus, see twi.c)
// Assumes Wire.begin() has already been called
void scanI2CBus(byte from_addr, byte to_addr, 
                void(*callback)(byte address, byte result) ) 
{
  byte rc;
  byte data = 0; // not used, just an address to feed to twi_writeTo()
  byte ti=0;
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr, &data, 0, 1,1);
    if(rc==0){
		callback( addr, rc );
		ti++;
	}
  }
  if(ti<2) error_watting_anykey();
}


// Called when address is found in scanI2CBus()
// Feel free to change this as needed
// (like adding I2C comm code to figure out what kind of I2C device is there)
void scanFunc( byte addr, byte result ) {
  Serial.print("addr: ");
  Serial.print(addr,DEC);
  addr = addr<<1;
  Serial.print("\t HEX: 0x");
  Serial.print(addr,HEX);
  Serial.println( (result==0) ? "\t found!":"   ");
//  Serial.print( (addr%4) ? "\t":"\n");
}


//*****************************************
//
//      24Cxx I2C EEPROM code
//
//*****************************************
  void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
  void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }


  byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }



  // maybe let's not read more than 30 or 32 bytes at a time!
  void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  }


//*****************************************
//
//      DS1302 RTC code
//
//*****************************************

void DS1302_SetOut(byte data ) {
    Wire.beginTransmission(B1101000);
    Wire.write(7); // LSB
    Wire.write(data);
    Wire.endTransmission();
}


byte DS1302_GetOut(void) {
    byte rdata = 0xFF;
    Wire.beginTransmission(B1101000);
    Wire.write(7); // LSB
    Wire.endTransmission();
    Wire.requestFrom(B1101000,1);
    if (Wire.available()) {
      rdata = Wire.read();
      Serial.println(rdata,HEX);
    }
    return rdata;
}


void showtime(void){
  byte i;
  Serial.print("Time=");
  DS1302_SetOut(0x00);
  RTC.get(rtc,true);  
  for(int i=0; i<7; i++){
    Serial.print(rtc[i]);
    Serial.print(" ");
  }
}


//*****************************************
//
//      measure battery function
//
//*****************************************
float readBatVcc(void){
    sensorValue = analogRead(A1);
    TT = sensorValue*0.0047;
    Serial.print("Battery: ");
    Serial.print(TT);
    Serial.print("V");
    if(TT<2.3) error_watting_anykey();
    return TT;
}
    
//*****************************************
//
//      standard Arduino setup()
//
//*****************************************
void setup()
{
    DDRC|=_BV(2) |_BV(3);
    PORTC |=_BV(3);
    Wire.begin();
    pinMode(8, INPUT);
    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    Serial.begin(19200);
	
	
    Serial.println("--- I2C Bus Scanner Test V1.2---");
    Serial.print("starting scanning of I2C bus from ");
    Serial.print(start_address,DEC);
    Serial.print(" to ");
    Serial.print(end_address,DEC);
    Serial.println("...");
    // start the scan, will call "scanFunc()" on result from each address
    scanI2CBus( start_address, end_address, scanFunc );

	
    Serial.println("\n");
    Serial.println("--- EEPROM Test---");
    char somedata[] = "this is data from the eeprom"; // data to write
    i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM 
    delay(100); //add a small delay
    Serial.println("Written Done");    
    delay(10);
    Serial.print("Read EERPOM:");
    byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory
    int addr=0; //first address
    while (b!=0 && b!=0xff) 
    {
      Serial.print((char)b); //print content to serial port
      addr++; //increase address
      b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    }
	
   Serial.println("\n");
  Serial.println("");
  Serial.println("--- DS11307 RTC Test---");  
  showtime();
  
  if(rtc[6]<2012){
    RTC.stop();
    RTC.set(DS1307_SEC,1);
    RTC.set(DS1307_MIN,0);
    RTC.set(DS1307_HR,12);
    RTC.set(DS1307_DOW,5);
    RTC.set(DS1307_DATE,1);
    RTC.set(DS1307_MTH,3);
    RTC.set(DS1307_YR,13);
    RTC.start();
    Serial.println(" SetTime:");
    showtime();    
  }
  byte oldsec =rtc[0];
  
  Serial.println("\n\n");
  val = digitalRead(8);   // read the input pin
  if(val==HIGH){
    Serial.println("--- Reserve Power Test---");
    Serial.println("  Close POWER!:");
     PORTC &=~_BV(3);
     byte time;
     for(time=0;time<3;time++){
       digitalWrite(13,HIGH);
       delay(500);
       digitalWrite(13,LOW);
       delay(500);     
       readBatVcc();
       Serial.println(""); 
     }
      PORTC |=_BV(3);
     Serial.println("\n  POWER On!");
  }
    delay(500);
    showtime();
	
	if(rtc[0]>oldsec){
		if((rtc[0]-oldsec)<2) error_watting_anykey();
	}else if(rtc[0]<oldsec){
		rtc[0] += 60;
		if((rtc[0]-oldsec)<2) error_watting_anykey();
	}else{
		error_watting_anykey();
	}
	
    Serial.println("\n");
    Serial.println("===  Done   ===");
    Serial.println("\n");
}

// standard Arduino loop()
void loop() 
{
    byte i;
    byte present = 0;
    unsigned int Temper=0;

    
	readBatVcc();
    
    ds.reset();
    ds.write(0xCC,1);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500);
    present = ds.reset();
    ds.write(0xCC,1);    
    ds.write(0xBE);         // Read Scratchpad
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      Tdata[i] = ds.read();
    }    
    Temper = (Tdata[1]<<8 | Tdata[0]);
    TT =Temper*0.0625;
    if(TT>200){
     Serial.println("\t DS18B20 Not installed!");
    }else{
      Serial.print("\t Temperature=");
      Serial.println(TT);
 
      
    }
    val = digitalRead(8);   // read the input pin
    if(val==LOW){
        showtime(); 
    }
    Serial.println("");
}

void error_watting_anykey()
{
  Serial.println("\n\r <--- Error there --->");
  while(1){
    while( Serial.available() == 0 ); //wait for input
    byte tmp =Serial.read();                    //empty input buffer    
    if(tmp=='#') software_Reset();
  }
  return;
}

void software_Reset() {
   asm volatile ("  jmp 0");
}  
