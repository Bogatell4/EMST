#include <SPI.h>

// Pins definitions
#define slaveSelectPin 10
#define dataReadyPin 12
#define dataResetPin 13

double result_f1, result_f2;
double z_f1, z_f2;

// AFE4300 register address definitions
#define ADC_DATA_RESULT         0x00
#define ADC_CONTROL_REGISTER1   0x01
#define MISC_REGISTER1          0x02
#define MISC_REGISTER2          0x03
#define DEVICE_CONTROL1         0x09
#define VSM_MATRIX              0x0A
#define ISW_MATRIX              0x0B
#define IM_MODE_ENABLE          0x0C
#define WEIGHT_SCALE_CONTROL    0x0D
#define BCM_DAC_FREQ            0x0E
#define DEVICE_CONTROL2         0x0F
#define ADC_CONTROL_REGISTER2   0x10
#define MISC_REGISTER3          0x1A

void resetAFE4300();
void writeRegister(unsigned char address, unsigned int data); // Write register
unsigned int readRegister(unsigned char address);              // Read register
void initAFE4300();

void setup() {
    Serial.begin(9600);
    Serial.flush();
    pinMode(slaveSelectPin, OUTPUT);
    pinMode(dataReadyPin, INPUT);
    pinMode(dataResetPin, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);          // According to DATASHEET
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    resetAFE4300();
    initAFE4300();
}

void loop() {
    while (millis() < 1000);            // Wait 1 second to start the first time
    delay(50);                          // 10 samples per second
    writeRegister(BCM_DAC_FREQ,0x0032); // 50kHz (for 2 freq measurement)
    result_f1 = readRegister(ADC_DATA_RESULT);
    z_f1 = 1.2958*result_f1 + 2660.2;   // Conversion not needed because changes are measured, not impedance itself
    Serial.print("result_f1: ");
    Serial.println(result_f1);
    Serial.print("z_f1: ");
    Serial.println(z_f1);
    Serial.print("\n");

    writeRegister(BCM_DAC_FREQ,0x00CB); // 200kHz (for 2 freq measurement)
    delay(250);
    result_f2 = readRegister(ADC_DATA_RESULT);
    z_f2 = 2.9258*result_f2 + 5065.5;   // result_f2
    Serial.print("result_f2: ");
    Serial.println(result_f2);
    Serial.print("z_f2: ");
    Serial.println(z_f2);
    Serial.print("\n");
}

// Functions
void writeRegister(unsigned char address, unsigned int data) {
    unsigned char firstByte = (unsigned char)(data >> 8);
    unsigned char secondByte = (unsigned char)data;

    SPI.transfer(address);

    // Send 2 bytes to be written
    SPI.transfer(firstByte);
    SPI.transfer(secondByte);
}

unsigned int readRegister(unsigned char address) {
    int spiReceive = 0;
    unsigned char spiReceiveFirst = 0;
    unsigned char spiReceiveSecond = 0;
    address = address & 0x1F;   // Last 5 bits specify address
    address = address | 0x20;   // First 3 bits need to be 001 for read opcode
    SPI.transfer(address);
    spiReceiveFirst = SPI.transfer(0x00);
    spiReceiveSecond = SPI.transfer(0x00);
    // Combine the two received bytes into a signed int
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
    writeRegister(ADC_CONTROL_REGISTER1, 0x5120);  // 32 SPS
    delay(10);
    writeRegister(MISC_REGISTER1, 0x0000);
    delay(10);
    writeRegister(MISC_REGISTER2, 0xFFFF);
    delay(10);
    writeRegister(DEVICE_CONTROL1, 0x0008);        // Power up BCM
    delay(10);
    writeRegister(ISW_MATRIX, 0x8040);
    delay(10);
    writeRegister(VSM_MATRIX, 0x8040);
    delay(10);
    writeRegister(BCM_DAC_FREQ, 0x0032);           // 50kHz
    delay(10);
    writeRegister(WEIGHT_SCALE_CONTROL, 0x0000);
    delay(10);
    writeRegister(IM_MODE_ENABLE, 0x0000);
    delay(10);
    writeRegister(DEVICE_CONTROL2, 0x0000);
    delay(10);
    writeRegister(ADC_CONTROL_REGISTER2, 0x0063);
    delay(10);
    writeRegister(MISC_REGISTER3, 0x0030);
    delay(10);
}
