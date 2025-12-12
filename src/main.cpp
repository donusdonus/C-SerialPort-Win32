
#include <stdio.h>
#include "SerialPort_Win32.h"

SerialPort_Win32 Serial;


char input[120];
int inputBuffer = 0;
int bufferRecived = 0;


void Serial_Receive()
{
    bufferRecived = Serial.available();
    if(bufferRecived > 0)
    {
    uint8_t data[bufferRecived];
    Serial.read(&data[0],bufferRecived);
    printf("Recived %d Bytes : %s\n",bufferRecived,(char*)&data[0]);
    Serial.flush();
    }
}

int main(void)
{
    printf("Software Started\r\n\r\n");
    Serial.begin("COM7",9600,SERIAL_8N1);

    Serial.SetTimeout(20,10,50);

    Serial.onReceive(Serial_Receive);

    vTaskStartScheduler();

    return 1 ;
}

        