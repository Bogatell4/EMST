#include <SPI.h>
const int cs = 10; //Slave Select
float R=0;
void setup() {
SPI.begin(); 
SPI.setBitOrder(MSBFIRST);
Serial.begin(9600);
pinMode (cs, OUTPUT);
digitalWrite (MOSI, LOW); 
digitalWrite (cs, HIGH);
SPI.setDataMode (SPI_MODE1);
SPI.setClockDivider(SPI_CLOCK_DIV16); 
initAFE4300();
delay(1500);
}
void loop() {
word result = initAFE4300();
//Serial.print("ADC_DATA_RESULT (DEC): ");
Serial.print(result);
//vella
//R=(result-123.7)/13.248;
//nova
R=(result-62.3)/12.76;
//Serial.print("  Z = ");
Serial.print(";");
Serial.println(R);
//Serial.println("ohm");
delay(1000);
}//void loop() end

// AFE4300 inicialization
word initAFE4300(){
digitalWrite (cs, LOW);
//ADC_CONTROL_REGISTER1 0x00
SPI.transfer (0x00); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]
//ADC_CONTROL_REGISTER1 0x01
SPI.transfer (0x01); 
SPI.transfer (0x41); //MSB [15...8]
SPI.transfer (0x40); //LSB [7...0]
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
//DEVICE_CONTROL1 0x09
SPI.transfer (0x09); 
SPI.transfer (0x60); //MSB [15...8]
SPI.transfer (0x06); //LSB [7...0]
//delay(10);
//ISW_MUX 0x0A
SPI.transfer (0x0A); 
SPI.transfer (0x08); //MSB [15...8]
SPI.transfer (0x04); //LSB [7...0]
//delay(10);
//VSENSE_MUX 0x0B
SPI.transfer (0x0B); 
SPI.transfer (0x08); //MSB [15...8]
SPI.transfer (0x04); //LSB [7...0]
//delay(10);
//IQ_MODE_ENABLE 0x0C
SPI.transfer (0x0C); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]
//delay(10);
//WEIGHT_SCALE_CONTROL 0x0D
SPI.transfer (0x0D); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x00); //LSB [7...0]
//delay(10);
//ADC_CONTROL_REGISTER2 0x10
SPI.transfer (0x10); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x63); //LSB [7...0]
//delay(10);
//MISC_REGISTER3 0x1A
SPI.transfer (0x1A); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x30); //LSB [7...0]
//delay(10);
//BCM_DAC_FREQ 0x0E
SPI.transfer (0x0E); 
SPI.transfer (0x00); //MSB [15...8]
SPI.transfer (0x32); //LSB [7...0]
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
