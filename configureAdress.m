function [] = configureAdress()

     ADC_DATA_RESULT =      0x00;
     ADC_CONTROL_REGISTER = 0x01;
     MISC_REGISTER_1 =       0x02;
     MISC_REGISTER_2 =       0x03;
     DEVICE_CONTROL_1 =     0x09;
     ISW_MATRIX =           0x0A;
     VSW_MATRIX =           0x0B;
     IQ_MODE_ENABLE =       0x0C;
     WEIGHT_SCALE_CONTROL = 0x0D;
     BCM_DAC_FREQ =         0x0E;
     DEVICE_CONTROL_2 =     0x0F;
     ADC_CONTROL_REGISTER_2 = 0x10;
     MISC3_REGISTER =       0x1A;


     writeData(hex2dec(ADC_CONTROL_REGISTER),hex2dec(0x5120)); %Adc configuration, copied from previous 32-SPS
     writeData(hex2dec(MISC_REGISTER_1),hex2dec(0x0000)); %Not used, always write 0x0000
     writeData(hex2dec(MISC_REGISTER_2),hex2dec(0xFFFF)); %allways write 0xFFFF by datasheet
     writeData(hex2dec(DEVICE_CONTROL_1),hex2dec(0x600A)); % Power ON DAC, device and BCM
     writeData(hex2dec(IQ_MODE_ENABLE),hex2dec(0x8000)); %Enable I/Q Demodulator mode

     writeData(hex2dec(DEVICE_CONTROL_2),hex2dec(0x1000)); %Need to configure so IQdemod freq is BCM_DAC_freq x4
                                                     % (fclk/DIVFACT)=BCMx4
     writeData(hex2dec(BCM_DAC_FREQ),hex2dec(0x003E)); %Set to 62kHz BCM measurements, 0x00FF=255kHz ; 0x0001=1kHz
                                                        %dependency on iq
                                                        %div factor


end


function [state] = configureFreq(freq)
    int state, freq;
    
end
