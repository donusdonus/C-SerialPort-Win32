
#include <stdio.h>
#include "SerialPort_Win32.h"
#include "FreeRTOS.h"

SerialPort_Win32 Serial;

int main()
{
    Serial.begin("COM7",9600);

    Serial.SetTimeout(20,10,50);

    Serial.print("Hello World\r\n");

    while(true)
    {
        int bufferIncome = Serial.available();
        Sleep(100);
        if(bufferIncome > 0)
        {
            char rx_data[bufferIncome];

            memset(&rx_data[0],0,bufferIncome);

            Serial.read((uint8_t*)&rx_data[0],bufferIncome);
            
            printf("Reviced Buffer %d Bytes : ",bufferIncome);

            for(int n = 0 ; n < bufferIncome ; n++)
            {
                printf("%c",rx_data[n]);
            }

            printf("\r\n");
        }

    }



    Serial.end();
    return 0;
}

        