
#include <stdio.h>
#include "SerialPort_Win32.h"

SerialPort_Win32 Serial;

char input[120];
int inputBuffer = 0;
int bufferRecived = 0;

int main(void)
{
    printf("Software Started\r\n\r\n");
    Serial.begin("COM7",9600,SERIAL_8N1);

    Serial.SetTimeout(20,10,50);

    while(1)
    {
       printf("Send Data : ");
       fgets(input,sizeof(input),stdin);
       inputBuffer = strlen(input);
       printf("User Input Buffer : %d Bytes\n",inputBuffer);
       Serial.write((uint8_t*)&input[0],inputBuffer);

       Sleep(5000);

        bufferRecived = Serial.available();
        if(bufferRecived > 0)
        {
            uint8_t data[bufferRecived];
            Serial.read(&data[0],bufferRecived);
            printf("Recived %d Bytes : %s\n",bufferRecived,(char*)&data[0]);
            Serial.flush();
        }
    }

    return 1 ;
}

        