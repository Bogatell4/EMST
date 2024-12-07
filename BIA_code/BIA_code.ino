#include <SPI.h>
// pins
const int dataReadyPin = 12;
const int dataResetPin = 13;
const int slaveSelectPin = 10;
int result;
int z;
/***************************
AFE4300 register address definitions
****************************/
#define ADC_DATA_RESULT 0x00
#define ADC_CONTROL_REGISTER 0x01
#define MISC1_REGISTER 0x02
#define MISC2_REGISTER 0x03
#define DEVICE_CONTROL_1 0x09
#define ISW_MATRIX 0x0A
#define VSW_MATRIX 0x0B
#define IQ_MODE_ENABLE 0x0C
#define WEIGHT_SCALE_CONTROL 0x0D
#define BCM_DAC_FREQ 0x0E
#define DEVICE_CONTROL_2 0x0F
#define ADC_CONTROL_REGISTER_2 0x10
#define MISC3_REGISTER 0x1A

void resetAFE4300();
void writeRegister(unsigned char address, unsigned int data); //Writes to a register on the AFE4300
int readRegister(unsigned char address); //Reads from a register on the AFE4300
void initAFE4300(); //intialize AFE

void setup() {
Serial.begin(9600);
Serial.flush();
pinMode(slaveSelectPin, OUTPUT);
pinMode(dataReadyPin, INPUT);
pinMode(dataResetPin, OUTPUT);
SPI.begin();
SPI.setBitOrder(MSBFIRST); // Segun DATASHET
SPI.setDataMode(SPI_MODE1);
SPI.setClockDivider(SPI_CLOCK_DIV16);
}

void loop() {
resetAFE4300();
initAFE4300();
delay(1000);
result = readRegister(ADC_DATA_RESULT);
Serial.print("Mesura = ");
Serial.print(result);
Serial.print('\t');
Serial.print("|Z| = ");
z = (result-142.89)/12.99;
Serial.print(z);
Serial.print('\n');
delay(1000);
}
/***************************
Functions
****************************/
void writeRegister(unsigned char address, unsigned int data) {
unsigned char firstByte = (unsigned char)(data >> 8);
unsigned char secondByte = (unsigned char)data;
SPI.transfer(address);
//Send 2 bytes to be written
SPI.transfer(firstByte);
SPI.transfer(secondByte);
}
int readRegister(unsigned char address) {
int spiReceive = 0;
unsigned char spiReceiveFirst = 0;
unsigned char spiReceiveSecond = 0;
address = address & 0x1F; //Last 5 bits specify address
address = address | 0x20; //First 3 bits need to be 001 for read opcode
SPI.transfer(address);
spiReceiveFirst = SPI.transfer(0x00);
spiReceiveSecond = SPI.transfer(0x00);
//Combine the two received bytes into a signed int
spiReceive = (spiReceiveFirst << 8);
spiReceive |= spiReceiveSecond;
return spiReceive;
}
void resetAFE4300() {
digitalWrite(dataResetPin, LOW);
delay(100);
digitalWrite(dataResetPin, HIGH);
}


void initAFE4300() {
writeRegister(ADC_CONTROL_REGISTER,0x4140);
delay(10);
writeRegister(MISC1_REGISTER,0x0000);
delay(10);
writeRegister(MISC2_REGISTER,0xFFFF);
delay(10);
writeRegister(DEVICE_CONTROL_1,0x6006); //Power up BCM
delay(10);
writeRegister(ISW_MATRIX,0x0804);
delay(10);
writeRegister(VSW_MATRIX,0x0804);
delay(10);
writeRegister(BCM_DAC_FREQ,0x0032);
delay(10);
writeRegister(WEIGHT_SCALE_CONTROL,0x0000);
delay(10);
writeRegister(IQ_MODE_ENABLE,0x0000);
delay(10);
writeRegister(DEVICE_CONTROL_2,0x0000);
delay(10);
writeRegister(ADC_CONTROL_REGISTER_2,0x0063);
delay(10);
writeRegister(MISC3_REGISTER,0x0030);
delay(10);
}
