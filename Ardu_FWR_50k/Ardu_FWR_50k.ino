#include <SPI.h>
const int cs = 10; //Slave Select
float R=0;
void setup() {
SPI.begin(); 
SPI.setBitOrder(MSBFIRST); // Specifies bit order by MSB for data transmision over SPI
Serial.begin(9600);
pinMode (cs, OUTPUT);
digitalWrite (MOSI, LOW); 
digitalWrite (cs, HIGH);
SPI.setDataMode (SPI_MODE1); // This means that data is sampled on rising edge of the clock
SPI.setClockDivider(SPI_CLOCK_DIV16); // If system clock is 16 MHz (which usually is case) this sets the SPI clock to 1 MHz.
initAFE4300();
delay(1500);
}

// 05/12/2024
// We have 2 resistors of 100 and 1000 ohms, and the lectures are really good based on actual
// calibration done since it reads  101.78 and 1018 respectively

// In case it is not enough with just magnitude, datasheet says...
// Although, with one single frequency or measurement, only the module of the impedance can be obtained; 
// two different frequencies could be used to obtain both the real and the imaginary parts.

void loop() {
word result = readImpedance();
//Serial.print("ADC_DATA_RESULT (DEC): ");
// First result is the data from out of ADC which needs to be modified by R
Serial.print("50kHz");
R=(result-62.3)/12.76;
//Serial.print("  Z = ");
Serial.print("= ");
Serial.println(R);
//Serial.println("ohm");
delay(1000);
}

word readImpedance(){
  digitalWrite (cs, LOW);
  //READ DATA 0X20
  SPI.transfer (0x20); //bit 21 to '1' to read data
  //delay(10);
  //pinMode(MISO, INPUT);
  SPI.setDataMode (SPI_MODE1);
  byte MSB = SPI.transfer (0x00);
  byte LSB = SPI.transfer (0x00);
  //Serial.println("MSB (HEX): ");
  //Serial.println(MSB, HEX);
  //Serial.println("LSB (HEX): ");
  //Serial.println(LSB, HEX);
  word dataResult = MSB << 8;
  dataResult |=LSB;
  return dataResult;
  digitalWrite (cs, HIGH);
  digitalWrite (MOSI, HIGH);
  //delay(1000);
  SPI.end ();
}


// AFE4300 inicialization, everything set for body composition measurement (BCM)
word initAFE4300(){
digitalWrite (cs, LOW);
//ADC_CONTROL_REGISTER1 0x00
//SPI.transfer (0x00); //This is the address to send the data, and it works always the same way
//SPI.transfer (0x00); //MSB [15...8]
//SPI.transfer (0x00); //LSB [7...0]
//ADC_CONTROL_REGISTER1 0x01
SPI.transfer (0x01); 
SPI.transfer (0x41); //MSB [15...8]
SPI.transfer (0x40); //LSB [7...0] Single-Shot cpnversion
//delay(10);
//MISC_REGISTER1 0x02
SPI.transfer (0x02); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]
//delay(10);
//MISC_REGISTER2 0x03
SPI.transfer (0x03); 
SPI.transfer (0xFF); //MSB [15...8]
SPI.transfer (0xFF); //LSB [7...0]
//delay(10);
//DEVICE_CONTROL2 0x0F by default 0x0000 USED FOR IQ MODE config
SPI.transfer (0x0F); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]

//DEVICE_CONTROL1 0x09
SPI.transfer (0x09); 
SPI.transfer (0x60); //MSB [15...8]
SPI.transfer (0x06); //LSB [7...0]
//delay(10);

//ISW_MUX 0x0A
SPI.transfer (0x0A); 
SPI.transfer (0x04); //MSB [15...8] IOUTP0 Closed RP open
SPI.transfer (0x20); //LSB [7...0] IOUTN3 Closed RN open
//delay(10);
//VSENSE_MUX 0x0B
SPI.transfer (0x0B); 
SPI.transfer (0x04); //MSB [15...8] VSENSEP0 closed
SPI.transfer (0x20); //LSB [7...0] VSENSEN3 closed
//delay(10);
//IQ_MODE_ENABLE 0x0C
SPI.transfer (0x0C); //This is using FWR mode. 0x8000 for IQ ON
SPI.transfer (0x00); //MSB [15...8] 
SPI.transfer (0x00); //LSB [7...0]
//delay(10);
//WEIGHT_SCALE_CONTROL 0x0D
SPI.transfer (0x0D); //Gain is 1 and there is no offset in DAC for correction
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]
//delay(10);
//BCM_DAC_FREQ 0x0E
SPI.transfer (0x0E); //Sets actual frequency to 48825 Hz
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x32); //LSB [7...0]
//delay(10);
//ADC_CONTROL_REGISTER2 0x10
SPI.transfer (0x10); //ADCREF connected to VREF, for impedance measurement
SPI.transfer (0x00); //MSB [15...8] Output of the body composition measurement front-end (OUTP_FILT/OUTM_FILT)
SPI.transfer (0x63); //LSB [7...0]
//delay(10);
//MISC_REGISTER3 0x1A
SPI.transfer (0x1A); //Default configuration
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x30); //LSB [7...0]
//delay(10);

//delay(8);
//READ DATA 0X20
SPI.transfer (0x20); //bit 21 to '1' to read data
//delay(10);
//pinMode(MISO, INPUT);
SPI.setDataMode (SPI_MODE1);
byte MSB = SPI.transfer (0x00);
byte LSB = SPI.transfer (0x00);
//Serial.println("MSB (HEX): ");
//Serial.println(MSB, HEX);
//Serial.println("LSB (HEX): ");
//Serial.println(LSB, HEX);
word dataResult = MSB << 8;
dataResult |=LSB;
return dataResult;
digitalWrite (cs, HIGH);
digitalWrite (MOSI, HIGH);
//delay(1000);
SPI.end ();
}
